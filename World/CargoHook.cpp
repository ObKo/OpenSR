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

#include "CargoHook.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 CargoHook::m_staticTypeId = typeIdFromClassName(CargoHook::staticMetaObject.className());

template<>
void WorldObject::registerType<CargoHook>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<CargoHook>("OpenSR.World", 1, 0, "CargoHook");
}

template<>
CargoHook* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new CargoHook(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<CargoHook>()
{
    return CargoHook::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<CargoHook>()
{
    return &CargoHook::staticMetaObject;
}

CargoHook::CargoHook(WorldObject *parent, quint32 id): Equipment(parent, id)
{
}

CargoHook::~CargoHook()
{
}

quint32 CargoHook::typeId() const
{
    return CargoHook::m_staticTypeId;
}

QString CargoHook::namePrefix() const
{
    return tr("Cargo hook");
}
}
}
