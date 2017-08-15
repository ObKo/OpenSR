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

#include "DefenceGenerator.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 DefenceGenerator::m_staticTypeId = typeIdFromClassName(DefenceGenerator::staticMetaObject.className());

template<>
void WorldObject::registerType<DefenceGenerator>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<DefenceGenerator>("OpenSR.World", 1, 0, "DefenceGenerator");
}

template<>
DefenceGenerator* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new DefenceGenerator(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<DefenceGenerator>()
{
    return DefenceGenerator::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<DefenceGenerator>()
{
    return &DefenceGenerator::staticMetaObject;
}

DefenceGenerator::DefenceGenerator(WorldObject *parent, quint32 id): Equipment(parent, id)
{
}

DefenceGenerator::~DefenceGenerator()
{
}

quint32 DefenceGenerator::typeId() const
{
    return DefenceGenerator::m_staticTypeId;
}

QString DefenceGenerator::namePrefix() const
{
    return tr("Defence generator");
}
}
}
