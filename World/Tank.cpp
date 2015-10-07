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

#include "Tank.h"

namespace OpenSR
{
namespace World
{
const quint32 Tank::staticTypeId = typeIdFromClassName(Tank::staticMetaObject.className());

Tank::Tank(WorldObject *parent, quint32 id): Equipment(parent, id)
{
}

Tank::~Tank()
{
}

quint32 Tank::typeId() const
{
    return Tank::staticTypeId;
}

QString Tank::namePrefix() const
{
    return tr("Tank");
}
}
}
