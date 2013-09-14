/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_SPACE_OBJECT_H
#define WORLD_SPACE_OBJECT_H

#include "OpenSR/World/WorldObject.h"
#include "OpenSR/World/Types.h"

namespace Rangers
{
namespace World
{
class RANGERS_WORLD_API SpaceObject: public WorldObject
{
public:
    SpaceObject(uint64_t id = 0);

    Point position() const;

    void setPosition(const Point& point);

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
protected:
    Point m_position;
};
}
}

#endif