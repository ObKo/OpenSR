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

    stream.read((char *)&m_population, sizeof(uint32_t));
    stream.read((char *)&m_invader, sizeof(uint32_t));
    stream.read((char *)&m_race, sizeof(uint32_t));

    if (!stream.good())
        return false;

    if (!m_landContext.deserialize(stream))
        return false;

    return true;
}

uint32_t HabitablePlanet::invader() const
{
    return m_invader;
}

LandContext HabitablePlanet::landContext() const
{
    return m_landContext;
}

uint32_t HabitablePlanet::population() const
{
    return m_population;
}

uint32_t HabitablePlanet::race() const
{
    return m_race;
}

bool HabitablePlanet::serialize(std::ostream& stream) const
{
    if (!Planet::serialize(stream))
        return false;

    stream.write((const char *)&m_population, sizeof(uint32_t));
    stream.write((const char *)&m_invader, sizeof(uint32_t));
    stream.write((const char *)&m_race, sizeof(uint32_t));

    if (!stream.good())
        return false;

    if (!m_landContext.serialize(stream))
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

void HabitablePlanet::setLandContext(const LandContext& landContext)
{
    m_landContext = landContext;
}

void HabitablePlanet::setRace(uint32_t race)
{
    m_race = race;
}
}
}
