/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_WORLD_PLANET_H
#define OPENSR_WORLD_PLANET_H

#include "World.h"
#include "SpaceObject.h"

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API Planet: public SpaceObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

public:
    Q_INVOKABLE Planet(uint32_t id = 0, WorldObject *parent = 0);
    virtual ~Planet();

    virtual uint32_t typeId() const;
    virtual QString namePrefix() const;

    bool save(QDataStream &stream) const;
    bool load(QDataStream &stream);
};
}
}

#endif // OPENSR_WORLD_PLANET_H
