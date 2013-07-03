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
#include <boost/shared_ptr.hpp>

#include "WorldStyleManager.h"

namespace Rangers
{
namespace World
{
class WorldObject;
class SolarSystem;
class WorldManager
{
public:
    WorldManager();

    static WorldManager& instance();

    boost::shared_ptr<WorldObject> addObject(WorldObject *object);
    void removeObject(boost::shared_ptr<WorldObject> object);
    void removeObject(uint64_t id);

    void generateWorld();
    bool saveWorld(const std::wstring& file) const;

    void calcTurn();
    void finishTurn();

    boost::shared_ptr<WorldObject> getObject(uint64_t id);

    std::list<boost::shared_ptr<SolarSystem> > solarSystems() const;
    boost::shared_ptr<SolarSystem> currentSolarSystem() const;

    WorldStyleManager& styleManager();

    static uint64_t getNextId();

private:
    void getSavingList(boost::shared_ptr<WorldObject> object, std::list<boost::shared_ptr<WorldObject> >& list, std::map<uint64_t, boost::shared_ptr<WorldObject> >& remainingObjects) const;

    std::map<uint64_t, boost::shared_ptr<WorldObject> > m_objects;
    std::list<boost::shared_ptr<SolarSystem> > m_solarSystems;
    boost::shared_ptr<SolarSystem> m_currentSystem;
    WorldStyleManager m_styleManager;

    static uint64_t m_idCounter;
};
}
}

#endif