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

#include "Planet.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
Planet::Planet(uint64_t id): SystemObject(id)
{
}

bool Planet::deserialize(std::istream& stream)
{
    if (!SystemObject::deserialize(stream))
        return false;

    stream.read((char *)&m_radius, sizeof(float));
    stream.read((char *)&m_orbit, sizeof(float));
    stream.read((char *)&m_style, sizeof(uint32_t));

    return true;
}

float Planet::orbit() const
{
    return m_orbit;
}

float Planet::radius() const
{
    return m_radius;
}

bool Planet::serialize(std::ostream& stream) const
{
    if (!SystemObject::serialize(stream))
        return false;

    stream.write((const char *)&m_radius, sizeof(float));
    stream.write((const char *)&m_orbit, sizeof(float));
    stream.write((const char *)&m_style, sizeof(uint32_t));

    return true;
}

uint32_t Planet::type() const
{
    return WorldHelper::TYPE_PLANET;
}

void Planet::setRadius(float radius)
{
    m_radius = radius;
}

void Planet::setOrbit(float orbit)
{
    m_orbit = orbit;
}

void Planet::setStyle(uint32_t style)
{
    m_style = style;
}

uint32_t Planet::style() const
{
    return m_style;
}
}
}
