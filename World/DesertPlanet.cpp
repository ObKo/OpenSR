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

#include "DesertPlanet.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 DesertPlanet::m_staticTypeId = typeIdFromClassName(DesertPlanet::staticMetaObject.className());

template<>
void WorldObject::registerType<DesertPlanet>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<DesertPlanet>("OpenSR.World", 1, 0, "DesertPlanet");
}

template<>
DesertPlanet* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new DesertPlanet(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<DesertPlanet>()
{
    return DesertPlanet::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<DesertPlanet>()
{
    return &DesertPlanet::staticMetaObject;
}

DesertPlanet::DesertPlanet(WorldObject *parent, quint32 id): Planet(parent, id)
{
}

DesertPlanet::~DesertPlanet()
{
}

quint32 DesertPlanet::typeId() const
{
    return DesertPlanet::m_staticTypeId;
}

QString DesertPlanet::namePrefix() const
{
    return tr("Desert planet");
}
}
}
