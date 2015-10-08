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

#include "Asteroid.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Asteroid::staticTypeId = typeIdFromClassName(Asteroid::staticMetaObject.className());

template<>
void WorldObject::registerType<Asteroid>()
{
    qmlRegisterType<Asteroid>("OpenSR.World", 1, 0, "Asteroid");
}

template<>
Asteroid* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Asteroid(parent, id);
}

Asteroid::Asteroid(WorldObject *parent, quint32 id): SpaceObject(parent, id)
{
}

Asteroid::~Asteroid()
{
}

quint32 Asteroid::typeId() const
{
    return Asteroid::staticTypeId;
}

QString Asteroid::namePrefix() const
{
    return tr("Asteroid");
}
}
}
