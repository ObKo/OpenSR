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

#include "ResourceManager.h"
#include "WorldManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 ResourceManager::m_staticTypeId = typeIdFromClassName(ResourceManager::staticMetaObject.className());

template<>
void WorldObject::registerType<ResourceManager>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<Resource>();
    qRegisterMetaTypeStreamOperators<Resource>();
    qmlRegisterType<ResourceManager>("OpenSR.World", 1, 0, "ResourceManager");
}

template<>
ResourceManager* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new ResourceManager(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<ResourceManager>()
{
    return ResourceManager::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<ResourceManager>()
{
    return &ResourceManager::staticMetaObject;
}

ResourceManager::ResourceManager(WorldObject *parent, quint32 id): WorldObject(parent, id), m_idPool(0)
{
}

ResourceManager::~ResourceManager()
{
}

quint32 ResourceManager::typeId() const
{
    return ResourceManager::m_staticTypeId;
}

QString ResourceManager::namePrefix() const
{
    return tr("Resource manager");
}

quint32 ResourceManager::getNextId()
{
    return ++m_idPool;
}


ResourceManager* ResourceManager::instance()
{
    if (WorldManager::instance() && WorldManager::instance()->context())
        return WorldManager::instance()->context()->resources();

    return 0;
}

quint32 ResourceManager::registerResource(const Resource& resource)
{
    quint32 id = resource.id();
    if (id)
    {
        auto it = m_resources.find(id);
        if (it == m_resources.end())
            m_resources[id] = resource;
        return id;
    }

    id = getNextId();
    m_resources[id] = resource;

    return id;
}
/*!
 * This function is called right before world saving.
 */
void ResourceManager::clearResources()
{
    m_resources.clear();
}

void ResourceManager::prepareSave()
{
    clearResources();
}

bool ResourceManager::save(QDataStream &stream) const
{
    if (!WorldObject::save(stream))
        return false;

    stream << m_resources.count();
    for (auto r : m_resources)
    {
        stream << r;
        if (stream.status() != QDataStream::Ok)
            return false;
    }

    return stream.status() == QDataStream::Ok;
}

bool ResourceManager::load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects)
{
    if (!WorldObject::load(stream, objects))
        return false;

    m_resources.clear();

    int count;
    stream >> count;

    for (int i = 0; i < count; i++)
    {
        Resource res;
        stream >> res;

        if (stream.status() != QDataStream::Ok)
            return false;

        registerResource(res);
    }

    return stream.status() == QDataStream::Ok;
}

Resource ResourceManager::getResource(quint32 id) const
{
    auto it = m_resources.find(id);
    if (it == m_resources.end())
        return Resource();

    return it.value();
}
}
}
