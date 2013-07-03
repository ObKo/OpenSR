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

#include <cmath>

namespace Rangers
{
namespace World
{
Planet::Planet(uint64_t id): SystemObject(id), m_radius(0.0f),
    m_orbit(0.0f), m_angle(0.0f), m_angleSpeed(0.0f), m_style(0), m_startAngle(0.0f)
{
}

bool Planet::deserialize(std::istream& stream)
{
    if (!SystemObject::deserialize(stream))
        return false;

    stream.read((char *)&m_radius, sizeof(float));
    stream.read((char *)&m_orbit, sizeof(float));
    stream.read((char *)&m_style, sizeof(uint32_t));
    stream.read((char *)&m_angle, sizeof(float));
    stream.read((char *)&m_angleSpeed, sizeof(float));

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

float Planet::angle() const
{
    return m_angle;
}

float Planet::angleSpeed() const
{
    return m_angleSpeed;
}

bool Planet::serialize(std::ostream& stream) const
{
    if (!SystemObject::serialize(stream))
        return false;

    stream.write((const char *)&m_radius, sizeof(float));
    stream.write((const char *)&m_orbit, sizeof(float));
    stream.write((const char *)&m_style, sizeof(uint32_t));
    stream.write((const char *)&m_angle, sizeof(float));
    stream.write((const char *)&m_angleSpeed, sizeof(float));

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
    updatePosition();
}

void Planet::setStyle(uint32_t style)
{
    m_style = style;
}

void Planet::setAngle(float angle)
{
    m_angle = angle;
    updatePosition();
}

void Planet::updatePosition()
{
    m_position.x = m_orbit * cos(m_angle);
    m_position.y = m_orbit * sin(m_angle);
}

void Planet::setAngleSpeed(float speed)
{
    m_angleSpeed = speed;
}

uint32_t Planet::style() const
{
    return m_style;
}

void Planet::calcTurn()
{
    m_startAngle = m_angle;
}

void Planet::finishTurn()
{
    m_angle = m_startAngle + m_angleSpeed;
    updatePosition();
}

void Planet::turn(float progress)
{
    m_angle = m_startAngle + m_angleSpeed * progress;
    updatePosition();
}
}
}
