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

#include "Sector.h"

#include <QHash>
#include <QDataStream>

namespace OpenSR
{
namespace World
{
const quint32 Sector::staticTypeId = qHash(Sector::staticMetaObject.className());

Sector::Sector(WorldObject *parent, quint32 id): WorldObject(parent, id)
{
}

Sector::~Sector()
{
}

QPointF Sector::position() const
{
    return m_position;
}

void Sector::setPosition(const QPointF& position)
{
    m_position = position;
    emit(positionChanged());
}

quint32 Sector::typeId() const
{
    return Sector::staticTypeId;
}

QString Sector::namePrefix() const
{
    return tr("Sector");
}

bool Sector::load(QDataStream& stream, const QMap< quint32, WorldObject* >& objects)
{
    if (!WorldObject::load(stream, objects))
        return false;

    stream >> m_position;
    emit(positionChanged());

    return stream.status() == QDataStream::Ok;
}

bool Sector::save(QDataStream& stream) const
{
    if (!WorldObject::save(stream))
        return false;

    stream << m_position;

    return stream.status() == QDataStream::Ok;
}
}
}
