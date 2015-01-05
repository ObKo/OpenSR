/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QString>
#include <QUrl>
#include <QMetaObject>
#include <QDirIterator>

#include <QDebug>

namespace OpenSR
{
ResourceReply::ResourceReply(const QUrl& url, QIODevice *device, QObject *parent)
{
    m_device = device;
    if (!m_device || !m_device->isOpen())
    {
        setError(QNetworkReply::UnknownContentError, QString("Cannot open resource file: %1").arg(url.path()));
        m_device = 0;
    }
    if (m_device)
        m_device->setParent(this);
    setFinished(true);
    QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
}

ResourceReply::~ResourceReply()
{
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
    QIODevice::close();

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

qint64 ResourceReply::readData(char * data, qint64 maxSize)
{
    if (!m_device)
        return -1;
    if (!maxSize)
        return 0;
    return m_device->read(data, maxSize);
}

class ResourceManagerNAM: public QNetworkAccessManager
{
public:
    ResourceManagerNAM(ResourceManager *manager, QObject *parent): QNetworkAccessManager(parent),
        m_manger(manager)
    {
    }

    virtual ~ResourceManagerNAM()
    {
    }

    virtual QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice *outgoingData)
    {
        qDebug() << op << req.url();
        if ((req.url().scheme().compare("res", Qt::CaseInsensitive) == 0) && (op == QNetworkAccessManager::GetOperation))
        {
            QString path = req.url().adjusted(QUrl::NormalizePathSegments).path();
            QIODevice *dev = m_manger->getIODevice(path);
            ResourceReply *reply = new ResourceReply(req.url(), dev, this);
            reply->setOperation(op);
            reply->setRequest(req);
            reply->setUrl(req.url());
            reply->open(QIODevice::ReadOnly);
            emit(finished(reply));
            return reply;
        }
        else
            return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }

private:
    ResourceManager *m_manger;
};

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
        if (child.isFile())
        {
            ResourceNode node(child.fileName(), ResourceNode::FILE, &current,
                              QSharedPointer<ResourceInfo>(new ResourceInfo()));
            node.info->provider = this;
            auto it = current.children.find(child.fileName());
            if (it != current.children.end())
            {
                if (it->type == ResourceNode::FILE)
                    *it = node;
            }
            else
                current.children.insert(child.fileName(), node);
        }
        else if (child.isDir())
        {
            ResourceNode node(child.fileName(), ResourceNode::DIRECTORY, &current);

            auto it = current.children.find(child.fileName());
            if (it == current.children.end())
                it = current.children.insert(child.fileName(), node);

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

QIODevice *ResourceManager::getIODevice(const QString& path, QObject *parent)
{
    QFileInfo fi(path);
    QStringList p = path.split('/', QString::SkipEmptyParts);

    ResourceNode *current = &m_root;

    for (const QString node : p)
    {
        auto it = current->children.find(node);
        if (it == current->children.end())
            return 0;
        current = &(it.value());
    }
    if (!current->info || !current->info->provider)
        return 0;
    return current->info->provider->getDevice(*current, parent);
}

void ResourceManager::addFileSystemPath(const QString& path)
{
    FSProvider *prov = new FSProvider(path);
    addProvider(prov);
}

void ResourceManager::addProvider(ResourceProvider *provider)
{
    provider->load(m_root);
    m_dataProviders.append(provider);
}
}


