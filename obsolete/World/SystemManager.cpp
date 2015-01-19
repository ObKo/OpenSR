/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include "SystemManager.h"

namespace
{
const uint32_t SYSTEM_MANAGER_SIGNATURE = *((uint32_t*)"SRSM");
}

namespace Rangers
{
namespace World
{
SystemManager::SystemManager()
{

}

SystemManager::SystemManager(const SystemManager& other)
{

}

bool SystemManager::deserialize(std::istream& stream)
{
    return true;
}

bool SystemManager::serialize(std::ostream& stream) const
{
    return true;
}

std::list<boost::shared_ptr<PlanetarySystem> > SystemManager::systems() const
{
    return m_systems;
}

boost::shared_ptr<PlanetarySystem> SystemManager::currentSystem() const
{
    return m_currentSystem;
}

void SystemManager::setCurrentSystem(boost::shared_ptr<PlanetarySystem> system)
{
    std::list<boost::shared_ptr<PlanetarySystem> >::iterator i = std::find(m_systems.begin(), m_systems.end(), system);
    if (i != m_systems.end())
        m_currentSystem = system;
}

void SystemManager::addSystem(boost::shared_ptr<PlanetarySystem> system)
{
    m_systems.push_back(system);
}
}
}
