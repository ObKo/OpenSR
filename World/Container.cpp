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

#include "Container.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Container::m_staticTypeId = typeIdFromClassName(Container::staticMetaObject.className());

template<>
void WorldObject::registerType<Container>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<Container>("OpenSR.World", 1, 0, "Container");
}

template<>
Container* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Container(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Container>()
{
    return Container::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Container>()
{
    return &Container::staticMetaObject;
}

Container::Container(WorldObject *parent, quint32 id): SpaceObject(parent, id)
{
}

Container::~Container()
{
}

quint32 Container::typeId() const
{
    return Container::m_staticTypeId;
}

QString Container::namePrefix() const
{
    return tr("Container");
}
}
}
