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

#include "Scanner.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Scanner::m_staticTypeId = typeIdFromClassName(Scanner::staticMetaObject.className());

template<>
void WorldObject::registerType<Scanner>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<Scanner>("OpenSR.World", 1, 0, "Scanner");
}

template<>
Scanner* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Scanner(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Scanner>()
{
    return Scanner::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Scanner>()
{
    return &Scanner::staticMetaObject;
}

Scanner::Scanner(WorldObject *parent, quint32 id): Equipment(parent, id)
{
}

Scanner::~Scanner()
{
}

quint32 Scanner::typeId() const
{
    return Scanner::m_staticTypeId;
}

QString Scanner::namePrefix() const
{
    return tr("Scanner");
}
}
}
