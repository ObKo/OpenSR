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

#include "WorldContext.h"

#include <QMap>
#include <QDataStream>

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 WorldContext::staticTypeId = typeIdFromClassName(WorldContext::staticMetaObject.className());

template<>
void WorldObject::registerType<WorldContext>()
{
    qmlRegisterType<WorldContext>("OpenSR.World", 1, 0, "WorldContext");
}

template<>
WorldContext* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new WorldContext(parent, id);
}

WorldContext::WorldContext(WorldObject *parent, quint32 id): WorldObject(parent, id), m_currentSystem(0)
{
}

WorldContext::~WorldContext()
{
}

quint32 WorldContext::typeId() const
{
    return WorldContext::staticTypeId;
}

QString WorldContext::namePrefix() const
{
    return tr("World");
}


PlanetarySystem* WorldContext::currentSystem() const
{
    return m_currentSystem;
}

void WorldContext::setCurrentSystem(PlanetarySystem *system)
{
    if (m_currentSystem != system)
    {
        m_currentSystem = system;
        emit(currentSystemChanged());
    }
}

bool WorldContext::save(QDataStream &stream) const
{
    quint32 id = 0;

    if (m_currentSystem)
        id = m_currentSystem->id();

    stream << id;
    return stream.status() == QDataStream::Ok;
}

bool WorldContext::load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects)
{
    quint32 id;
    stream >> id;
    auto it = objects.find(id);
    if (it != objects.end())
        setCurrentSystem(qobject_cast<PlanetarySystem*>(it.value()));

    return stream.status() == QDataStream::Ok;
}
}
}
