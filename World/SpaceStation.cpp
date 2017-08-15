/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 - 2017 Kosyak <ObKo@mail.ru>

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

#include "ResourceManager.h"
#include "SpaceStation.h"
#include "WorldBindings.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 SpaceStation::m_staticTypeId = typeIdFromClassName(SpaceStation::staticMetaObject.className());

template<>
void WorldObject::registerType<SpaceStation>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<StationStyle>();
    qRegisterMetaTypeStreamOperators<StationStyle>();
    qRegisterMetaType<StationStyle::Data>();
    qRegisterMetaTypeStreamOperators<StationStyle::Data>();
    bindEnumsToJS<SpaceStation>(script);
    qmlRegisterType<SpaceStation>("OpenSR.World", 1, 0, "SpaceStation");
}

template<>
SpaceStation* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new SpaceStation(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<SpaceStation>()
{
    return SpaceStation::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<SpaceStation>()
{
    return &SpaceStation::staticMetaObject;
}

SpaceStation::SpaceStation(WorldObject *parent, quint32 id): MannedObject(parent, id)
  , m_stationKind(StationKind::Unspecified)
{
}

SpaceStation::~SpaceStation()
{
}

quint32 SpaceStation::typeId() const
{
    return SpaceStation::m_staticTypeId;
}

QString SpaceStation::namePrefix() const
{
    return tr("Space station");
}

SpaceStation::StationKind SpaceStation::stationKind() const
{
    return m_stationKind;
}

StationStyle SpaceStation::style() const
{
    return m_style;
}

void SpaceStation::prepareSave()
{
    MannedObject::prepareSave();
    m_style.registerResource();
}

void SpaceStation::setStationKind(StationKind kind)
{
    if (m_stationKind == kind)
        return;

    m_stationKind = kind;
    emit stationKindChanged(kind);
}

void SpaceStation::setStyle(const StationStyle& style)
{
    if (m_style == style)
        return;

    m_style = style;
    emit styleChanged(style);
}

QString StationStyle::texture() const
{
    return getData<Data>().texture;
}

void StationStyle::setTexture(const QString &texture)
{
    auto d = getData<Data>();
    d.texture = texture;
    setData(d);
}

bool operator==(const StationStyle& one, const StationStyle& another)
{
    return one.texture() == another.texture();
}

// StationStyle Streaming
QDataStream& operator<<(QDataStream & stream, const StationStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, StationStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const StationStyle::Data& data)
{
    return stream << data.texture;
}

QDataStream& operator>>(QDataStream & stream, StationStyle::Data& data)
{
    return stream >> data.texture;
}

}
}
