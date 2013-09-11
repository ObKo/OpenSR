/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#include "SkinManager.h"
#include "RaceManager.h"
#include "PlanetManager.h"
#include "SystemManager.h"
#include "StyleManager.h"

namespace Rangers
{
namespace World
{
class WorldObject;
class PlanetarySystem;
class WorldGenHook;

class RANGERS_WORLD_API WorldManager
{
public:
    WorldManager();

    static WorldManager& instance();

    void addObject(boost::shared_ptr<WorldObject> object);
    void removeObject(boost::shared_ptr<WorldObject> object);
    void removeObject(uint64_t id);

    void generateWorld();
    bool saveWorld(const std::wstring& file) const;
    bool loadWorld(const std::wstring& file);

    void calcTurn();
    void finishTurn();

    boost::shared_ptr<WorldObject> getObject(uint64_t id);

    void addGenHook(boost::shared_ptr<WorldGenHook> hook);
    void removeGenHook(boost::shared_ptr<WorldGenHook> hook);

    SkinManager& skinManager();
    RaceManager& raceManager();
    PlanetManager& planetManager();
    SystemManager& systemManager();
    StyleManager& styleManager();

    static uint64_t getNextId();

private:
    void getSavingList(boost::shared_ptr<WorldObject> object, std::list<boost::shared_ptr<WorldObject> >& list, std::map<uint64_t, boost::shared_ptr<WorldObject> >& remainingObjects) const;

    std::map<uint64_t, boost::shared_ptr<WorldObject> > m_objects;

    SkinManager m_skinManager;
    RaceManager m_raceManager;
    PlanetManager m_planetManager;
    SystemManager m_systemManager;
    StyleManager m_styleManager;

    std::list<boost::shared_ptr<WorldGenHook> > m_genHooks;

    static uint64_t m_idCounter;
};
}
}

#endif