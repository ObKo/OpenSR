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

#include "InhabitedPlanet.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 InhabitedPlanet::m_staticTypeId = typeIdFromClassName(InhabitedPlanet::staticMetaObject.className());

template<>
void WorldObject::registerType<InhabitedPlanet>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<InhabitedPlanet>("OpenSR.World", 1, 0, "InhabitedPlanet");
}

template<>
InhabitedPlanet* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new InhabitedPlanet(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<InhabitedPlanet>()
{
    return InhabitedPlanet::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<InhabitedPlanet>()
{
    return &InhabitedPlanet::staticMetaObject;
}

InhabitedPlanet::InhabitedPlanet(WorldObject *parent, quint32 id): Planet(parent, id)
{
}

InhabitedPlanet::~InhabitedPlanet()
{
}

quint32 InhabitedPlanet::typeId() const
{
    return InhabitedPlanet::m_staticTypeId;
}

QString InhabitedPlanet::namePrefix() const
{
    return tr("Inhabited planet");
}
}
}
