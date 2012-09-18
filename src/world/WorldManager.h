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

#ifndef WORLD_WORLD_MANAGER_H
#define WORLD_WORLD_MANAGER_H

#include <map>
#include <list>
#include <string>
#include <stdint.h>

namespace Rangers
{
namespace World
{
class WorldObject;
class WorldManager
{
public:
    static WorldManager& instance();

    void addObject(WorldObject *object);
    void removeObject(WorldObject *object);
    void removeObject(uint64_t id);

    bool saveWorld(const std::wstring& file) const;

    static uint64_t getNextId();

private:
    void getSavingList(WorldObject *object, std::list<WorldObject*>& list, std::map<uint64_t, WorldObject*>& remainingObjects);
    std::map<uint64_t, WorldObject*> m_objects;

    static uint64_t m_idCounter;
};
}
}

#endif