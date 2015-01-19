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

#include <QHash>

namespace OpenSR
{
namespace World
{
const quint32 Item::staticTypeId = qHash(Item::staticMetaObject.className());

Item::Item(quint32 id, WorldObject *parent): WorldObject(id, parent)
{
}

Item::~Item()
{
}

quint32 Item::typeId() const
{
    return Item::staticTypeId;
}

QString Item::namePrefix() const
{
    return tr("Item");
}
}
}
