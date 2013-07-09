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
#include "SolarSystem.h"
#include "WorldGenHook.h"

#include <fstream>
#include <libRanger.h>
#include <OpenSR/Log.h>
#include <OpenSR/PythonBindings.h>

namespace Rangers
{
namespace World
{
uint64_t WorldManager::m_idCounter = 0;

WorldManager::WorldManager()
{
    m_styleManager.loadStyles();
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


WorldStyleManager& WorldManager::styleManager()
{
    return m_styleManager;
}

RaceManager& WorldManager::raceManager()
{
    return m_raceManager;
}

void WorldManager::generateWorld()
{
    std::list<boost::shared_ptr<WorldGenHook> >::const_iterator end = m_genHooks.end();
    for (std::list<boost::shared_ptr<WorldGenHook> >::const_iterator i = m_genHooks.begin(); i != end; ++i)
    {
        (*i)->generate();
    }
}

std::list<boost::shared_ptr<SolarSystem> > WorldManager::solarSystems() const
{
    return m_solarSystems;
}

boost::shared_ptr<SolarSystem> WorldManager::currentSolarSystem() const
{
    return m_currentSystem;
}

void WorldManager::setCurrentSolarSystem(boost::shared_ptr<SolarSystem> system)
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> >::iterator i = m_objects.find(system->id());
    if (i != m_objects.end())
        m_currentSystem = system;
}

bool WorldManager::saveWorld(const std::wstring& file) const
{
    std::map<uint64_t, boost::shared_ptr<WorldObject> > objects = m_objects;
    std::list<boost::shared_ptr<WorldObject> > savingList;

    while (objects.size())
    {
        boost::shared_ptr<WorldObject> object = (*objects.begin()).second;
        getSavingList(object, savingList, objects);
    }

    std::ofstream worldFile;

#if defined(WIN32) && defined(_MSC_VER)
    worldFile.open(file);
#else
    worldFile.open(toLocal(file).c_str());
#endif

    std::list<boost::shared_ptr<WorldObject> >::const_iterator end = savingList.end();
    for (std::list<boost::shared_ptr<WorldObject> >::const_iterator i = savingList.begin(); i != end; ++i)
    {
        if (!(*i)->serialize(worldFile))
        {
            Log::error() << "Cannot serialize world!";
            break;
        }
    }

    worldFile.close();
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
}
}
