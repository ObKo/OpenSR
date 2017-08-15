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
const quint32 WorldObject::m_staticTypeId = typeIdFromClassName(WorldObject::staticMetaObject.className());

template<>
void WorldObject::registerType<WorldObject>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<WorldObject>("OpenSR.World", 1, 0, "WorldObject");
}

template<>
WorldObject* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new WorldObject(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<WorldObject>()
{
    return WorldObject::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<WorldObject>()
{
    return &WorldObject::staticMetaObject;
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
    return WorldObject::m_staticTypeId;
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

static QObject* childrenAtF(QQmlListProperty<QObject> *property, int index)
{
    return property->object->children().at(index);
}

static int childrenCountF(QQmlListProperty<QObject> *property)
{
    return property->object->children().count();
}

QQmlListProperty<QObject> WorldObject::getChildren()
{
    return QQmlListProperty<QObject>(this, 0, childrenCountF, childrenAtF);
}

void WorldObject::prepareSave()
{
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
 * \brief start next turn.
 * This function is called when next turn is requested.
 * Object must calculate all future action in turn.
 * \note All subclasses must call startTurn() from super-class.
 */
void WorldObject::startTurn()
{
    for (auto o : children())
    {
        WorldObject *wo = qobject_cast<WorldObject*>(o);
        if (wo)
            wo->startTurn();
    }
}

/*!
 * \brief process turn.
 * \param time - turn time, from 0.0f to 1.0f
 * This function is called during turn animation.
 * Normally, this function is called only for objects visible by player.
 * Object must "play" planned actions.
 * \note All subclasses must call processTurn() from super-class.
 */
void WorldObject::processTurn(float time)
{
    for (auto o : children())
    {
        WorldObject *wo = qobject_cast<WorldObject*>(o);
        if (wo)
            wo->processTurn(time);
    }
}
/*!
 * \brief finsh turn.
 * This function is called when turn is finished.
 * Object must complete all actions planned for turn.
 * \note All subclasses must call finishTurn() from super-class.
 */
void WorldObject::finishTurn()
{
    for (auto o : children())
    {
        WorldObject *wo = qobject_cast<WorldObject*>(o);
        if (wo)
            wo->finishTurn();
    }
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
