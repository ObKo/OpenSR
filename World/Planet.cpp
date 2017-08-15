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

#include "Planet.h"
#include "ResourceManager.h"

#include <QtQml>


namespace OpenSR
{
namespace World
{
const quint32 Planet::m_staticTypeId = typeIdFromClassName(Planet::staticMetaObject.className());

template<>
void WorldObject::registerType<Planet>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<PlanetStyle>();
    qRegisterMetaTypeStreamOperators<PlanetStyle>();
    qRegisterMetaType<PlanetStyle::Data>();
    qRegisterMetaTypeStreamOperators<PlanetStyle::Data>();
    qmlRegisterType<Planet>("OpenSR.World", 1, 0, "Planet");
}

template<>
Planet* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Planet(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Planet>()
{
    return Planet::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Planet>()
{
    return &Planet::staticMetaObject;
}

Planet::Planet(WorldObject *parent, quint32 id): SpaceObject(parent, id)
{
}

Planet::~Planet()
{
}

quint32 Planet::typeId() const
{
    return Planet::m_staticTypeId;
}

QString Planet::namePrefix() const
{
    return tr("Planet");
}

PlanetStyle Planet::style() const
{
    return m_style;
}

void Planet::setStyle(const PlanetStyle& style)
{
    if (m_style == style)
        return;

    m_style = style;
    emit styleChanged(style);
}

QString PlanetStyle::surface() const
{
    return getData<Data>().surface;
}

void PlanetStyle::setSurface(const QString &texture)
{
    auto d = getData<Data>();
    d.surface = texture;
    setData(d);
}

QString PlanetStyle::cloud0() const
{
    return getData<Data>().cloud0;
}

void PlanetStyle::setCloud0(const QString &texture)
{
    auto d = getData<Data>();
    d.cloud0 = texture;
    setData(d);
}

QString PlanetStyle::cloud1() const
{
    return getData<Data>().cloud1;
}

void PlanetStyle::setCloud1(const QString &texture)
{
    auto d = getData<Data>();
    d.cloud1 = texture;
    setData(d);
}

int PlanetStyle::radius() const
{
    return getData<Data>().radius;
}

void PlanetStyle::setRadius(const int r)
{
    auto d = getData<Data>();
    d.radius = r;
    setData(d);
}

QColor PlanetStyle::atmosphere() const
{
    return getData<Data>().atmosphere;
}

void PlanetStyle::setAtmosphere(const QColor& c)
{
    auto d = getData<Data>();
    d.atmosphere = c;
    setData(d);
}

bool operator==(const PlanetStyle& one, const PlanetStyle& another)
{
    return  (one.surface()  == another.surface() ) &&
            (one.cloud0() == another.cloud0()) &&
            (one.cloud1() == another.cloud1()) &&
            (one.radius() == another.radius()) &&
            (one.atmosphere() == another.atmosphere())
            ;
}

// PlanetStyle Streaming
QDataStream& operator<<(QDataStream & stream, const PlanetStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, PlanetStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const PlanetStyle::Data& data)
{
    return stream << data.surface << data.cloud0 << data.cloud1 << data.radius << data.atmosphere;
}

QDataStream& operator>>(QDataStream & stream, PlanetStyle::Data& data)
{
    return stream >> data.surface >> data.cloud0 >> data.cloud1 >> data.radius << data.atmosphere;
}

}
}
