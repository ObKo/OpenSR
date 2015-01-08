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

#ifndef OPENSR_RESOURCEMANAGER_H
#define OPENSR_RESOURCEMANAGER_H

#include <OpenSR/OpenSR.h>
#include <QObject>
#include <QHash>
#include <QSharedPointer>

class QQmlNetworkAccessManagerFactory;
class QIODevice;

namespace OpenSR
{
class ResourceManagerNAMFactory;
class ResourceNode;

class ResourceProvider
{
public:
    virtual ~ResourceProvider();

    virtual void load(ResourceNode& root) = 0;
    virtual QIODevice *getDevice(const ResourceNode& node, QObject *parent = 0) = 0;
};

class ResourceInfo
{
public:
    ResourceInfo();
    virtual ~ResourceInfo();

    ResourceProvider *provider;
};

class ResourceNode
{
public:
    enum Type {FILE, DIRECTORY};

    ResourceNode(const QString& name = QString(), Type type = ResourceNode::DIRECTORY,
                 ResourceNode *parent = 0, QSharedPointer<ResourceInfo> info = QSharedPointer<ResourceInfo>());
    virtual ~ResourceNode();

    ResourceNode *parent;
    Type type;
    QString name;
    QHash<QString, ResourceNode> children;
    QSharedPointer<ResourceInfo> info;
};

class ENGINE_API ResourceManager : public QObject
{
    Q_OBJECT
public:
    ResourceManager(QObject *parent = 0);
    virtual ~ResourceManager();

    QQmlNetworkAccessManagerFactory *qmlNAMFactory() const;

    bool fileExists(const QString& path) const;
    QIODevice *getIODevice(const QString& path, QObject *parent = 0);
    QIODevice *getIODevice(const QUrl& path, QObject *parent = 0);

public Q_SLOTS:
    void addFileSystemPath(const QString& path);
    void addPKGArchive(const QString& path);
    void addProvider(ResourceProvider* provider);

private:
    ResourceNode m_root;
    ResourceManagerNAMFactory *m_namFactory;
    QList<ResourceProvider*> m_dataProviders;
};
}

#endif // OPENSR_RESOURCEMANAGER_H
