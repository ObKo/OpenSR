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

#include "HabitablePlanet.h"
#include "WorldHelper.h"
#include "WorldManager.h"

namespace Rangers
{
namespace World
{
HabitablePlanet::HabitablePlanet(uint64_t id): Planet(id)
{
}

bool HabitablePlanet::deserialize(std::istream& stream)
{
    if (!Planet::deserialize(stream))
        return false;

    uint64_t landContext;
    stream.read((char *)&m_population, sizeof(uint32_t));
    stream.read((char *)&m_invader, sizeof(uint32_t));
    stream.read((char *)&landContext, sizeof(uint64_t));

    if (!stream.good())
        return false;

    if (landContext)
        m_landContext = boost::dynamic_pointer_cast<LandContext>(WorldManager::instance().getObject(landContext));

    return true;
}

uint32_t HabitablePlanet::invader() const
{
    return m_invader;
}

boost::shared_ptr<LandContext> HabitablePlanet::landContext() const
{
    return m_landContext;
}

uint32_t HabitablePlanet::population() const
{
    return m_population;
}

bool HabitablePlanet::serialize(std::ostream& stream) const
{
    if (!Planet::serialize(stream))
        return false;

    uint64_t landContext = 0;
    if (m_landContext)
        landContext = m_landContext->id();
    stream.write((const char *)&m_population, sizeof(uint32_t));
    stream.write((const char *)&m_invader, sizeof(uint32_t));
    stream.write((const char *)&landContext, sizeof(uint64_t));

    if (!stream.good())
        return false;

    return true;
}

uint32_t HabitablePlanet::type() const
{
    return WorldHelper::TYPE_HABITABLEPLANET;
}

void HabitablePlanet::setPopulation(uint32_t population)
{
    m_population = population;
}

void HabitablePlanet::setInvader(uint32_t invader)
{
    m_invader = invader;
}

void HabitablePlanet::setLandContext(boost::shared_ptr<LandContext> landContext)
{
    m_landContext = landContext;
}

std::list< uint64_t > HabitablePlanet::dependencies()
{
    std::list< uint64_t > dep = WorldObject::dependencies();
    if (m_landContext)
        dep.push_back(m_landContext->id());
    return dep;
}
}
}
