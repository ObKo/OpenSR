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

#include "Equipment.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Equipment::m_staticTypeId = typeIdFromClassName(Equipment::staticMetaObject.className());

template<>
void WorldObject::registerType<Equipment>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<Equipment>("OpenSR.World", 1, 0, "Equipment");
}

template<>
Equipment* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Equipment(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Equipment>()
{
    return Equipment::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Equipment>()
{
    return &Equipment::staticMetaObject;
}

Equipment::Equipment(WorldObject *parent, quint32 id): Item(parent, id)
{
}

Equipment::~Equipment()
{
}

quint32 Equipment::typeId() const
{
    return Equipment::m_staticTypeId;
}

QString Equipment::namePrefix() const
{
    return tr("Equipment");
}
}
}
