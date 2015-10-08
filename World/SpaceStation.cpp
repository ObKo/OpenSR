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

#include "SpaceStation.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 SpaceStation::m_staticTypeId = typeIdFromClassName(SpaceStation::staticMetaObject.className());

template<>
void WorldObject::registerType<SpaceStation>()
{
    qmlRegisterType<SpaceStation>("OpenSR.World", 1, 0, "SpaceStation");
}

template<>
SpaceStation* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new SpaceStation(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<SpaceStation>()
{
    return SpaceStation::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<SpaceStation>()
{
    return &SpaceStation::staticMetaObject;
}

SpaceStation::SpaceStation(WorldObject *parent, quint32 id): MannedObject(parent, id)
{
}

SpaceStation::~SpaceStation()
{
}

quint32 SpaceStation::typeId() const
{
    return SpaceStation::m_staticTypeId;
}

QString SpaceStation::namePrefix() const
{
    return tr("Space station");
}
}
}
