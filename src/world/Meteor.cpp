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

#include "Meteor.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
Meteor::Meteor(uint64_t id): SystemObject(id), m_speed(0), m_mineral(0)
{
}

bool Meteor::deserialize(std::istream& stream)
{
    if (!SystemObject::deserialize(stream))
        return false;

    stream.read((char *)&m_focus, sizeof(Point));
    stream.read((char *)&m_speed, sizeof(float));
    stream.read((char *)&m_mineral, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}

Point Meteor::focus() const
{
    return m_focus;
}

float Meteor::mineral() const
{
    return m_mineral;
}

bool Meteor::serialize(std::ostream& stream) const
{
    if (!SystemObject::serialize(stream))
        return false;

    stream.write((const char *)&m_focus, sizeof(Point));
    stream.write((const char *)&m_speed, sizeof(float));
    stream.write((const char *)&m_mineral, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}

float Meteor::speed() const
{
    return m_speed;
}

uint32_t Meteor::type() const
{
    return WorldHelper::TYPE_METEOR;
}

void Meteor::setFocus(const Point& focus)
{
    m_focus = focus;
}

void Meteor::setSpeed(float speed)
{
    m_speed = speed;
}

void Meteor::setMineral(float mineral)
{
    m_mineral = mineral;
}
}
}
