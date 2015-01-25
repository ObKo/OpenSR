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

#include "Race.h"

#include <QHash>
#include <QDataStream>

namespace OpenSR
{
namespace World
{
const quint32 Race::staticTypeId = qHash(Race::staticMetaObject.className());

Race::Race(WorldObject *parent, quint32 id): WorldObject(parent, id)
{
}

Race::~Race()
{
}

quint32 Race::typeId() const
{
    return Race::staticTypeId;
}

QString Race::namePrefix() const
{
    return tr("Race");
}

void Race::setStyle(const RaceStyle& style)
{
    m_style = style;
    emit(styleChanged());
}

RaceStyle Race::style() const
{
    return m_style;
}

bool Race::load(QDataStream& stream, const QMap< quint32, WorldObject* >& objects)
{
    if (!WorldObject::load(stream, objects))
        return false;

    stream >> m_style;
    emit(styleChanged());

    return stream.status() == QDataStream::Ok;
}

bool Race::save(QDataStream& stream) const
{
    if (!WorldObject::save(stream))
        return false;

    stream << m_style;

    return stream.status() == QDataStream::Ok;
}

QDataStream& operator<<(QDataStream& stream, const RaceStyle& style)
{
    return stream << style.color << style.icon << style.sound;
}

QDataStream& operator>>(QDataStream& stream, RaceStyle& style)
{
    return stream >> style.color >> style.icon >> style.sound;
}
}
}
