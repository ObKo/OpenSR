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

#include "MannedObject.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 MannedObject::m_staticTypeId = typeIdFromClassName(MannedObject::staticMetaObject.className());

template<>
void WorldObject::registerType<MannedObject>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<MannedObject>("OpenSR.World", 1, 0, "MannedObject");
}

template<>
MannedObject* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new MannedObject(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<MannedObject>()
{
    return MannedObject::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<MannedObject>()
{
    return &MannedObject::staticMetaObject;
}

MannedObject::MannedObject(WorldObject *parent, quint32 id): SpaceObject(parent, id)
{
}

MannedObject::~MannedObject()
{
}

quint32 MannedObject::typeId() const
{
    return MannedObject::m_staticTypeId;
}

QString MannedObject::namePrefix() const
{
    return tr("Manned object");
}
}
}
