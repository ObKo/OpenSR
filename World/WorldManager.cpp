/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2014 Kosyak <ObKo@mail.ru>

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
#include "PlanetarySystem.h"
#include "WorldGenHook.h"
#include "WorldHelper.h"

#include <OpenSR/Engine.h>

#include <fstream>
#include <OpenSR/libRanger.h>
#include <OpenSR/Log.h>
#include <OpenSR/PythonBindings.h>

namespace
{
const uint32_t SAVE_FILE_SIGNATURE = *((uint32_t*)"SRSF");
}

namespace Rangers
{
namespace World
{
uint64_t WorldManager::m_idCounter = 0;

WorldManager::WorldManager()
{
    m_skinManager.loadStyles();
}

WorldManager& WorldManager::instance()
{
    static WorldManager manager;
    return manager;
}

void WorldManager::addObject(boost::shared_ptr<WorldObject> object)
{
    if (!object)
        return;
    m_objects[object->id()] = object;
}

void WorldManager::removeObject(boost::shared_ptr<WorldObject> object)
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = m_objects.find(object->id());
    if (i != m_objects.end())
        m_objects.erase(i);
}
void WorldManager::removeObject(uint64_t id)
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = m_objects.find(id);
    if (i != m_objects.end())
        m_objects.erase(i);
}


SkinManager& WorldManager::skinManager()
{
    return m_skinManager;
}

RaceManager& WorldManager::raceManager()
{
    return m_raceManager;
}

PlanetManager& WorldManager::planetManager()
{
    return m_planetManager;
}

void WorldManager::generateWorld()
{
    std::string worldRaces = Engine::instance().properties()->get<std::string>("world.races", "/world/races");
    std::string worldSystemStyles = Engine::instance().properties()->get<std::string>("world.systemStyles", "/world/system-styles");
    std::string worldPlanetStyles = Engine::instance().properties()->get<std::string>("world.planetStyles", "/world/planet-styles");
    std::string worldAsteroidStyles = Engine::instance().properties()->get<std::string>("world.asteroidStyles", "/world/asteroid-styles");
    std::string worldSystemBackgrounds = Engine::instance().properties()->get<std::string>("world.worldSystemBackgrounds", "/world/system-backgrounds");

    if (!worldRaces.empty())
        m_raceManager.loadRaces(worldRaces);

    if (!worldSystemStyles.empty())
        m_styleManager.loadSystemStyles(worldSystemStyles);

    if (!worldPlanetStyles.empty())
        m_styleManager.loadPlanetStyles(worldPlanetStyles);

    if (!worldAsteroidStyles.empty())
        m_styleManager.loadAsteroidStyles(worldAsteroidStyles);

    if (!worldSystemBackgrounds.empty())
        m_styleManager.loadSystemBackgrounds(worldSystemBackgrounds);

    std::list<boost::shared_ptr<WorldGenHook> >::const_iterator end = m_genHooks.end();
    for (std::list<boost::shared_ptr<WorldGenHook> >::const_iterator i = m_genHooks.begin(); i != end; ++i)
    {
        (*i)->generate();
    }
}

bool WorldManager::saveWorld(const std::string& file) const
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> > objects = m_objects;
    std::list<boost::shared_ptr<WorldObject> > savingList;

    while (!objects.empty())
    {
        boost::shared_ptr<WorldObject> object = (*objects.begin()).second;
        getSavingList(object, savingList, objects);
    }

    std::ofstream worldFile;

#if defined(WIN32) && defined(_MSC_VER)
    worldFile.open(fromUTF8(file));
#else
    worldFile.open(file);
#endif

    worldFile.write((const char*)&SAVE_FILE_SIGNATURE, 4);

    if (!worldFile.good())
    {
        Log::error() << "Cannot save world";
        worldFile.close();
        return false;
    }

    if (!m_planetManager.serialize(worldFile))
    {
        Log::error() << "Cannot save PlanetManager";
        worldFile.close();
        return false;
    }

    if (!m_systemManager.serialize(worldFile))
    {
        Log::error() << "Cannot save SystemManager";
        worldFile.close();
        return false;
    }

    if (!m_raceManager.serialize(worldFile))
    {
        Log::error() << "Cannot save RaceManager";
        worldFile.close();
        return false;
    }

    std::list<boost::shared_ptr<WorldObject> >::const_iterator end = savingList.end();
    for (std::list<boost::shared_ptr<WorldObject> >::const_iterator i = savingList.begin(); i != end; ++i)
    {
        if (!(*i)->serialize(worldFile))
        {
            Log::error() << "Cannot save world objects";
            worldFile.close();
            return false;
        }
    }

    worldFile.close();
    return true;
}


bool WorldManager::loadWorld(const std::string& file)
{
    std::ifstream worldFile;

#if defined(WIN32) && defined(_MSC_VER)
    worldFile.open(fromUTF8(file));
#else
    worldFile.open(file);
#endif

    uint32_t sig;
    worldFile.read((char*)&sig, 4);

    if ((!worldFile.good()) || (sig != SAVE_FILE_SIGNATURE))
    {
        Log::error() << "Cannot load world";
        worldFile.close();
        return false;
    }

    if (!m_planetManager.deserialize(worldFile))
    {
        Log::error() << "Cannot load PlanetManager";
        worldFile.close();
        return false;
    }

    if (!m_systemManager.deserialize(worldFile))
    {
        Log::error() << "Cannot load SystemManager";
        worldFile.close();
        return false;
    }

    if (!m_raceManager.deserialize(worldFile))
    {
        Log::error() << "Cannot load RaceManager";
        worldFile.close();
        return false;
    }

    while (worldFile.good())
    {
        uint32_t classType;
        //FIXME: Ugly
        worldFile.seekg(4, std::ios_base::cur);
        worldFile.read((char *)&classType, 4);
        if (worldFile.eof())
            break;
        worldFile.seekg(-8, std::ios_base::cur);

        if (!worldFile.good())
        {
            Log::error() << "Cannot load world objects.";
            worldFile.close();
            return false;
        }

        boost::shared_ptr<WorldObject> object = WorldHelper::createObjectByType(classType);
        if ((!object) || (!object->deserialize(worldFile)))
        {
            Log::error() << "Cannot load world objects.";
            worldFile.close();
            return false;
        }
        addObject(object);
        //FIXME: Quite ugly too.
        if (object->type() == WorldHelper::TYPE_PLANETARYSYSTEM)
        {
            boost::shared_ptr<PlanetarySystem> system = boost::static_pointer_cast<PlanetarySystem>(object);
            m_systemManager.addSystem(system);
            //FIXME: Should save current system in some "game" context.
            m_systemManager.setCurrentSystem(system);
        }
    }

    worldFile.close();
    return true;
}

void WorldManager::calcTurn()
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> > objects = m_objects;
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator end = objects.end();
    for (std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = objects.begin(); i != end; ++i)
    {
        (*i).second->calcTurn();
    }
}

void WorldManager::finishTurn()
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> > objects = m_objects;
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator end = objects.end();
    for (std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = objects.begin(); i != end; ++i)
    {
        (*i).second->finishTurn();
    }
}

uint64_t WorldManager::getNextId()
{
    return ++m_idCounter;
}

void WorldManager::getSavingList(boost::shared_ptr<WorldObject> object, std::list<boost::shared_ptr<WorldObject> >& list, std::map<uint64_t, boost::shared_ptr<WorldObject> >& remainingObjects) const
{
    if (!object)
        return;

    std::list<uint64_t> dependencies = object->dependencies();

    std::list<uint64_t>::const_iterator end = dependencies.end();
    for (std::list<uint64_t>::const_iterator i = dependencies.begin(); i != end; ++i)
    {
        std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator j = remainingObjects.find(*i);
        if (j != remainingObjects.end())
        {
            getSavingList((*j).second, list, remainingObjects);
        }
    }
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = remainingObjects.find(object->id());
    if (i != remainingObjects.end())
    {
        list.push_back(object);
        remainingObjects.erase(i);
    }
}

boost::shared_ptr<WorldObject> WorldManager::getObject(uint64_t id)
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = m_objects.find(id);
    if (i != m_objects.end())
        return (*i).second;
    else
        return boost::shared_ptr<WorldObject>();
}

void WorldManager::addGenHook(boost::shared_ptr< WorldGenHook > hook)
{
    if (!hook)
        return;
    m_genHooks.push_back(hook);
}

void WorldManager::removeGenHook(boost::shared_ptr< WorldGenHook > hook)
{
    if (!hook)
        return;
    m_genHooks.remove(hook);
}

SystemManager& WorldManager::systemManager()
{
    return m_systemManager;
}

StyleManager& WorldManager::styleManager()
{
    return m_styleManager;
}
}
}
