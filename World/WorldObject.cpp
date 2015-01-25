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

#include <QHash>
#include <QDataStream>

namespace OpenSR
{
namespace World
{
const quint32 WorldObject::staticTypeId = qHash(WorldObject::staticMetaObject.className());

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
    m_name = name;
    emit(nameChanged());
}

bool WorldObject::save(QDataStream &stream) const
{
    stream << m_name;
    return stream.status() == QDataStream::Ok;
}

bool WorldObject::load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects)
{
    QString name;
    stream >> name;
    setName(name);
    emit(nameChanged());
    return stream.status() == QDataStream::Ok;
}
}
}
