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

#ifndef OPENSR_WORLD_RESOURCEMANAGER_H
#define OPENSR_WORLD_RESOURCEMANAGER_H

#include "World.h"
#include "WorldObject.h"
#include "Resource.h"

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API ResourceManager: public WorldObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

public:
    Q_INVOKABLE ResourceManager(WorldObject *parent = 0, quint32 id = 0);
    virtual ~ResourceManager();

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

    static ResourceManager *instance();

    Resource getResource(quint32 id) const;

    //! Forget about all resources.
    void clearResources();

    virtual void prepareSave();
    virtual bool save(QDataStream &stream) const;
    virtual bool load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects);

private:
    quint32 registerResource(const Resource& res);
    quint32 getNextId();

    quint32 m_idPool;
    QMap<quint32, Resource> m_resources;

    friend class Resource;
};
}
}

#endif // OPENSR_WORLD_RESOURCEMANAGER_H
