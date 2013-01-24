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
#include "DesertPlanet.h"
#include "HabitablePlanet.h"

#include <fstream>
#include <libRanger.h>
#include <OpenSR/Log.h>

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

boost::shared_ptr<WorldObject> WorldManager::addObject(WorldObject *object)
{
    boost::shared_ptr<WorldObject> result = boost::shared_ptr<WorldObject>(object);
    m_objects[object->id()] = result;
    return result;
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

void WorldManager::generateWorld()
{
    boost::shared_ptr<SolarSystem> system = boost::static_pointer_cast<SolarSystem>(addObject(new SolarSystem()));
    boost::shared_ptr<DesertPlanet> planet = boost::static_pointer_cast<DesertPlanet>(addObject(new DesertPlanet()));
    boost::shared_ptr<DesertPlanet> sPlanet = boost::static_pointer_cast<DesertPlanet>(addObject(new DesertPlanet()));
    boost::shared_ptr<HabitablePlanet> hPlanet = boost::static_pointer_cast<HabitablePlanet>(addObject(new HabitablePlanet()));

    system->setName("Test system");
    system->setSize(2000.0f);
    system->setPosition(Point(0.0f, 0.0f));

    planet->setOrbit(600.0f);
    planet->setRadius(50.0f);
    planet->setName("Planet");
    planet->setPosition(Point(600.0f, 0.0f));
    planet->setStyle(textHash32(L"mercur"));

    hPlanet->setOrbit(450.0f);
    hPlanet->setRadius(75.0f);
    hPlanet->setName("Earth");
    hPlanet->setPosition(Point(-450.0f, 0.0f));
    hPlanet->setInvader(0);
    hPlanet->setPopulation(1000000);
    hPlanet->setStyle(textHash32(L"earth"));

    sPlanet->setOrbit(800.0f);
    sPlanet->setRadius(100.0f);
    sPlanet->setName("Saturn");
    sPlanet->setPosition(Point(0.0f, 800.0f));
    sPlanet->setStyle(textHash32(L"saturn"));

    m_currentSystem = system;
    m_solarSystems.push_back(system);

    system->addObject(planet);
    system->addObject(hPlanet);
    system->addObject(sPlanet);
}

std::list<boost::shared_ptr<SolarSystem> > WorldManager::solarSystems() const
{
    return m_solarSystems;
}

boost::shared_ptr<SolarSystem> WorldManager::currentSolarSystem() const
{
    return m_currentSystem;
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

}
}
