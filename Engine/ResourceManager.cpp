/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 - 2017 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenSR/ResourceManager.h"
#include "ResourceManager_p.h"

#include "OpenSR/Engine.h"

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QString>
#include <QUrl>
#include <QMetaObject>
#include <QDirIterator>
#include <QBuffer>

#include <QDebug>

namespace OpenSR
{
ResourceReply::ResourceReply(const QUrl& url, QIODevice *device, QObject *parent): QNetworkReply(parent)
{
    m_device = device;
    if (!m_device || !m_device->isOpen())
    {
        setError(QNetworkReply::UnknownContentError, QString("Cannot open resource file: %1").arg(url.path()));
        m_device = 0;
    }
    if (m_device)
        m_device->setParent(this);

    QMetaObject::invokeMethod(this, "emitFinished", Qt::QueuedConnection);
}

void ResourceReply::emitFinished()
{
    setFinished(true);
    emit(finished());
}

ResourceReply::~ResourceReply()
{
    close();
}

void ResourceReply::abort()
{
    close();
}

qint64 ResourceReply::bytesAvailable() const
{
    if (!m_device)
        return QIODevice::bytesAvailable();
    return QIODevice::bytesAvailable() + m_device->bytesAvailable();
}

bool ResourceReply::canReadLine() const
{
    if (!m_device)
        return QIODevice::canReadLine();
    return QIODevice::canReadLine() || m_device->canReadLine();
}

void ResourceReply::close()
{
    QNetworkReply::close();

    if (!m_device)
        return;
    m_device->close();
    m_device = 0;
}

bool ResourceReply::isSequential() const
{
    return false;
}

qint64 ResourceReply::size() const
{
    if (!m_device)
        return -1;
    qint64 size = m_device->size();
    return size;
}

bool ResourceReply::seek(qint64 pos)
{
    if (!m_device)
        return false;
    QIODevice::seek(pos);
    return m_device->seek(pos);
}

qint64 ResourceReply::readData(char * data, qint64 maxSize)
{
    if (!m_device)
        return -1;
    if (!maxSize)
        return 0;
    return m_device->read(data, maxSize);
}

ResourceManagerNAM::ResourceManagerNAM(ResourceManager *manager, QObject *parent): QNetworkAccessManager(parent),
    m_manger(manager)
{
}

ResourceManagerNAM::~ResourceManagerNAM()
{
}

QNetworkReply* ResourceManagerNAM::createRequest(Operation op, const QNetworkRequest & req, QIODevice *outgoingData)
{
    QUrl url = req.url();
    QString scheme = url.scheme();
    if ((scheme.compare("res", Qt::CaseInsensitive) == 0) ||
        (scheme.compare("dat", Qt::CaseInsensitive) == 0) &&
            (op == QNetworkAccessManager::GetOperation))
    {
        QIODevice *dev = m_manger->getIODevice(url);
        ResourceReply *reply = new ResourceReply(url, dev, this);
        connect(reply, SIGNAL(finished()), this, SLOT(emitReplyFinished()));
        reply->setOperation(op);
        reply->setRequest(req);
        reply->setUrl(url);
        reply->open(QIODevice::ReadOnly);
        return reply;
    }
    else
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
}

void ResourceManagerNAM::emitReplyFinished()
{
    QNetworkReply *r = static_cast<QNetworkReply*>(sender());
    emit(finished(r));
}

class ResourceManagerNAMFactory: public QQmlNetworkAccessManagerFactory
{
public:
    ResourceManagerNAMFactory(ResourceManager* manager): QQmlNetworkAccessManagerFactory(),
        m_manager(manager)
    {
    }

    virtual QNetworkAccessManager *create(QObject *parent)
    {
        return new ResourceManagerNAM(m_manager, parent);
    }

private:
    ResourceManager *m_manager;
};

ResourceProvider::~ResourceProvider()
{
}

FSProvider::FSProvider(const QString& dir): m_dir(dir)
{
}

FSProvider::~FSProvider()
{
}

void FSProvider::load(ResourceNode& current, const QDir& dir)
{
    if (!dir.exists())
        return;

    QFileInfoList sub = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo& child : sub)
    {
        QString lowerName = child.fileName().toLower();
        if (child.isFile())
        {
            ResourceNode node(child.fileName(), ResourceNode::FILE, &current,
                              QSharedPointer<ResourceInfo>(new ResourceInfo()));
            node.info->provider = this;
            auto it = current.children.find(lowerName);
            if (it != current.children.end())
            {
                if (it->type == ResourceNode::FILE)
                    *it = node;
            }
            else
                current.children.insert(lowerName, node);
        }
        else if (child.isDir())
        {
            ResourceNode node(child.fileName(), ResourceNode::DIRECTORY, &current);

            auto it = current.children.find(lowerName);
            if (it == current.children.end())
                it = current.children.insert(lowerName, node);

            load(*it, QDir(child.absoluteFilePath()));
        }
    }
}

void FSProvider::load(ResourceNode& root)
{
    load(root, QDir(m_dir));
}

QIODevice *FSProvider::getDevice(const ResourceNode& node, QObject *parent)
{
    QString relativePath = node.name;
    const ResourceNode *current = node.parent;
    while (current->parent)
    {
        relativePath.prepend(current->name + "/");
        current = current->parent;
    }
    QFileInfo fi(m_dir + '/' + relativePath);

    if (!fi.exists())
        return 0;

    QFile *result = new QFile(fi.absoluteFilePath(), parent);
    result->open(QIODevice::ReadOnly);

    return result;
}

ResourceInfo::ResourceInfo()
{
    provider = 0;
}

ResourceInfo::~ResourceInfo()
{
}

ResourceNode::ResourceNode(const QString& name_, Type type_, ResourceNode *parent_, QSharedPointer<ResourceInfo> info_):
    name(name_), type(type_), parent(parent_), info(info_)
{
}

ResourceNode::~ResourceNode()
{
}

ResourceManager::ResourceManager(QObject *parent): QObject(parent)
{
    m_namFactory = new ResourceManagerNAMFactory(this);
}

ResourceManager::~ResourceManager()
{
    for (ResourceProvider* p : m_dataProviders)
        delete p;
}


QQmlNetworkAccessManagerFactory *ResourceManager::qmlNAMFactory() const
{
    return m_namFactory;
}

bool ResourceManager::fileExists(const QString& path) const
{
    QStringList p = path.split('/', QString::SkipEmptyParts);

    const ResourceNode *current = &m_root;

    for (const QString node : p)
    {
        auto it = current->children.find(node.toLower());
        if (it == current->children.end())
            return false;
        current = &(it.value());
    }
    return true;
}

QIODevice *ResourceManager::getIODevice(const QString& path, QObject *parent)
{
    QStringList p = path.split('/', QString::SkipEmptyParts);

    ResourceNode *current = &m_root;

    for (const QString node : p)
    {
        auto it = current->children.find(node.toLower());
        if (it == current->children.end())
            return 0;
        current = &(it.value());
    }
    if (!current->info || !current->info->provider)
        return 0;
    return current->info->provider->getDevice(*current, parent);
}

QIODevice *ResourceManager::getIODevice(const QUrl& path, QObject *parent)
{
    if (path.scheme().compare("res", Qt::CaseInsensitive) &&
        path.scheme().compare("dat", Qt::CaseInsensitive))
        return 0;

    QString p = path.path();
    if (!path.scheme().compare("dat", Qt::CaseInsensitive))
    {
        if (p.at(0) == '/')
            p.remove(0, 1);
        p = qobject_cast<Engine*>(qApp)->datValue(p).toString();
        p = p.replace("\\", "/");
    }
    return getIODevice(p, parent);
}

void ResourceManager::addFileSystemPath(const QString& path)
{
    FSProvider *prov = new FSProvider(path);
    addProvider(prov);
}

void ResourceManager::addPKGArchive(const QString& path)
{
    PKGProvider *prov = new PKGProvider(path);
    addProvider(prov);
}

void ResourceManager::addProvider(ResourceProvider *provider)
{
    provider->load(m_root);
    m_dataProviders.append(provider);
}


PGKResourceInfo::PGKResourceInfo(PKGItem *item_): ResourceInfo(),
    item(item_)
{
}

PGKResourceInfo::~PGKResourceInfo()
{
}

PKGIODevice::PKGIODevice(const PKGItem& item, QIODevice *archiveDev, QObject *parent): QIODevice(parent),
    m_buffer(0)
{
    m_data = extractFile(item, archiveDev);
    if (!m_data.isEmpty())
    {
        m_buffer = new QBuffer(&m_data);
        m_buffer->open(QIODevice::ReadOnly);
    }
}

PKGIODevice::~PKGIODevice()
{
    if (m_buffer)
    {
        m_buffer->close();
        delete m_buffer;
        m_buffer = nullptr;
    }
}

qint64 PKGIODevice::bytesAvailable() const
{
    if (!m_buffer)
        return -1;
    return QIODevice::bytesAvailable() + m_buffer->bytesAvailable();
}

bool PKGIODevice::canReadLine() const
{
    if (!m_buffer)
        return false;
    return QIODevice::canReadLine() || m_buffer->canReadLine();
}

bool PKGIODevice::isSequential() const
{
    return false;
}

qint64 PKGIODevice::size() const
{
    return m_data.size();
}

void PKGIODevice::close()
{
    if (m_buffer)
    {
        m_buffer->close();
        delete m_buffer;
        m_buffer = 0;
    }
    m_data = QByteArray();
}


bool PKGIODevice::seek(qint64 pos)
{
    if (!m_buffer)
        return false;
    QIODevice::seek(pos);
    return m_buffer->seek(pos);
}

qint64 PKGIODevice::readData(char * data, qint64 maxSize)
{
    if (!m_buffer)
        return -1;
    return m_buffer->read(data, maxSize);
}

qint64 PKGIODevice::writeData(const char * data, qint64 maxSize)
{
    return -1;
}

PKGProvider::PKGProvider(const QString& file): m_root(0)
{
    QFileInfo fi(file);
    if (fi.exists())
    {
        m_path = fi.absoluteFilePath();
    }
}

PKGProvider::~PKGProvider()
{
    if (m_root)
        cleanup(m_root);
    delete m_root;
}

void PKGProvider::load(ResourceNode& root)
{
    if (m_path.isEmpty())
        return;

    QFile archive(m_path);
    archive.open(QIODevice::ReadOnly);
    if (!archive.isOpen())
        return;

    m_root = loadPKG(&archive);

    if (!m_root)
        return;

    archive.close();

    load(root, m_root);
}

QIODevice *PKGProvider::getDevice(const ResourceNode& node, QObject *parent)
{
    if (m_path.isEmpty())
        return 0;


    QFile archive(m_path);
    archive.open(QIODevice::ReadOnly);
    if (!archive.isOpen())
        return 0;

    PGKResourceInfo *info = static_cast<PGKResourceInfo*>(node.info.data());

    PKGIODevice *dev = new PKGIODevice(*info->item, &archive, parent);
    archive.close();
    dev->open(QIODevice::ReadOnly);
    return dev;
}

void PKGProvider::load(ResourceNode& current, PKGItem* item)
{
    for (int i = 0; i < item->childCount; i++)
    {
        PKGItem* child = &item->childs[i];
        QString lowerName = QString(child->name).toLower();
        if (child->dataType == 3)
        {
            ResourceNode node(child->name, ResourceNode::DIRECTORY, &current);

            auto it = current.children.find(lowerName);
            if (it == current.children.end())
                it = current.children.insert(lowerName, node);

            load(*it, child);
        }
        else
        {
            ResourceNode node(child->name, ResourceNode::FILE, &current,
                              QSharedPointer<ResourceInfo>(new PGKResourceInfo(child)));
            node.info->provider = this;
            auto it = current.children.find(lowerName);
            if (it != current.children.end())
            {
                if (it->type == ResourceNode::FILE)
                    *it = node;
            }
            else
                current.children.insert(lowerName, node);
        }
    }
}

void PKGProvider::cleanup(PKGItem *item)
{
    for (int i = 0; i < item->childCount; i++)
        cleanup(&item->childs[i]);
    delete[] item->childs;
}
}


