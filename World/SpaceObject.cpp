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

#include "SpaceObject.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 SpaceObject::m_staticTypeId = typeIdFromClassName(SpaceObject::staticMetaObject.className());

template<>
void WorldObject::registerType<SpaceObject>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<SpaceObject>("OpenSR.World", 1, 0, "SpaceObject");
}

template<>
SpaceObject* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new SpaceObject(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<SpaceObject>()
{
    return SpaceObject::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<SpaceObject>()
{
    return &SpaceObject::staticMetaObject;
}

SpaceObject::SpaceObject(WorldObject *parent, quint32 id): WorldObject(parent, id)
{
}

SpaceObject::~SpaceObject()
{
}

QPointF SpaceObject::position() const
{
    return m_position;
}

void SpaceObject::setPosition(const QPointF& pos)
{
    if (pos != m_position)
    {
        m_position = pos;
        emit(positionChanged());
    }
}

quint32 SpaceObject::typeId() const
{
    return SpaceObject::m_staticTypeId;
}

QString SpaceObject::namePrefix() const
{
    return tr("Space object");
}

QVariantList SpaceObject::trajectory() const
{
    QVariantList result;
    for (auto curve : m_trajectory)
        result.append(QVariant::fromValue(curve));
    return result;
}

void SpaceObject::setTrajectory(const QList<BezierCurve>& trajectory)
{
    m_trajectory = trajectory;
    emit(trajectoryChanged());
}

void SpaceObject::updateTrajectory()
{
}
}
}
