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

#include "Item.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Item::m_staticTypeId = typeIdFromClassName(Item::staticMetaObject.className());

template<>
void WorldObject::registerType<Item>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<Item>("OpenSR.World", 1, 0, "WorldItem");
}

template<>
Item* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Item(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Item>()
{
    return Item::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Item>()
{
    return &Item::staticMetaObject;
}

Item::Item(WorldObject *parent, quint32 id): WorldObject(parent, id)
{
}

Item::~Item()
{
}

quint32 Item::typeId() const
{
    return Item::m_staticTypeId;
}

QString Item::namePrefix() const
{
    return tr("Item");
}
}
}
