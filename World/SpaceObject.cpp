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

#include "SpaceObject.h"
#include "WorldHelper.h"
#include <map>

namespace Rangers
{
namespace World
{
SpaceObject::SpaceObject(uint64_t id): WorldObject(id)
{
    m_position.x = 0;
    m_position.y = 0;
}

bool SpaceObject::deserialize(std::istream& stream)
{
    if (!WorldObject::deserialize(stream))
        return false;

    stream.read((char *)&m_position, sizeof(Point));

    if (!stream.good())
        return false;

    return true;
}

Point SpaceObject::position() const
{
    return m_position;
}

bool SpaceObject::serialize(std::ostream& stream) const
{
    if (!WorldObject::serialize(stream))
        return false;

    stream.write((const char *)&m_position, sizeof(Point));

    if (!stream.good())
        return false;

    return true;
}

uint32_t SpaceObject::type() const
{
    return WorldHelper::TYPE_SPACEOBJECT;
}

void SpaceObject::setPosition(const Point& point)
{
    m_position = point;
}

}
}
