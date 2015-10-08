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

#include "WorldObject.h"
#include "WorldManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 WorldObject::staticTypeId = typeIdFromClassName(WorldObject::staticMetaObject.className());

template<>
void WorldObject::registerType<WorldObject>()
{
    qmlRegisterType<WorldObject>("OpenSR.World", 1, 0, "WorldObject");
}

template<>
WorldObject* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new WorldObject(parent, id);
}

WorldObject::WorldObject(WorldObject *parent, quint32 id): QObject(parent),
    m_id(id)
{
    if (!m_id)
        m_id = WorldManager::instance()->getNextId();
}

WorldObject::~WorldObject()
{
}

quint32 WorldObject::id() const
{
    return m_id;
}

quint32 WorldObject::typeId() const
{
    return WorldObject::staticTypeId;
}

QString WorldObject::name() const
{
    return m_name;
}

QString WorldObject::namePrefix() const
{
    return tr("Object");
}

void WorldObject::setName(const QString& name)
{
    if (m_name != name)
    {
        m_name = name;
        emit(nameChanged());
    }
}

bool WorldObject::save(QDataStream &stream) const
{
    return true;
}

bool WorldObject::load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects)
{
    return true;
}

/*!
 * \brief Generate type ID from class name
 * \param className class name (usually, metaObject()->className)
 * \return type ID for class
 */
quint32 WorldObject::typeIdFromClassName(const QString& className)
{
    quint32 h = 0;
    int n = className.length();

    while (n--)
    {
        h = (h << 4) + className[n].unicode();
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}
}
}
