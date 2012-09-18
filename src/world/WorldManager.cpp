/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include "WorldManager.h"
#include "WorldObject.h"

namespace Rangers
{
namespace World
{
uint64_t WorldManager::m_idCounter = 0;

WorldManager& WorldManager::instance()
{
    static WorldManager manager;
    return manager;
}

void WorldManager::addObject(WorldObject *object)
{
    m_objects[object->id()] = object;
}
void WorldManager::removeObject(WorldObject *object)
{
    std::map<uint64_t, WorldObject*>::iterator i = m_objects.find(object->id());
    if (i != m_objects.end())
        m_objects.erase(i);
}
void WorldManager::removeObject(uint64_t id)
{
    std::map<uint64_t, WorldObject*>::iterator i = m_objects.find(id);
    if (i != m_objects.end())
        m_objects.erase(i);
}

bool WorldManager::saveWorld(const std::wstring& file) const
{
}

uint64_t WorldManager::getNextId()
{
    return ++m_idCounter;
}

void WorldManager::getSavingList(WorldObject *object, std::list<WorldObject*>& list, std::map<uint64_t, WorldObject*>& remainingObjects)
{
    if (!object)
        return;
    std::list<uint64_t> dependencies = object->dependencies();
    std::list<uint64_t>::const_iterator end = dependencies.end();
    for (std::list<uint64_t>::const_iterator i = dependencies.begin(); i != end; ++i)
    {
        std::map<uint64_t, WorldObject*>::iterator j = remainingObjects.find(*i);
        if (j != remainingObjects.end())
        {
            getSavingList((*j).second, list, remainingObjects);
            list.push_back((*j).second);
            remainingObjects.erase(j);
        }
    }
    std::map<uint64_t, WorldObject*>::iterator i = remainingObjects.find(object->id());
    if (i != remainingObjects.end())
    {
        list.push_back(object);
        remainingObjects.erase(i);
    }
}
}
}
