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

#include "Planet.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Planet::staticTypeId = typeIdFromClassName(Planet::staticMetaObject.className());

template<>
void WorldObject::registerType<Planet>()
{
    qmlRegisterType<Planet>("OpenSR.World", 1, 0, "Planet");
}

template<>
Planet* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Planet(parent, id);
}

Planet::Planet(WorldObject *parent, quint32 id): SpaceObject(parent, id)
{
}

Planet::~Planet()
{
}

quint32 Planet::typeId() const
{
    return Planet::staticTypeId;
}

QString Planet::namePrefix() const
{
    return tr("Planet");
}
}
}
