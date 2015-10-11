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

#include "Asteroid.h"

#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Asteroid::m_staticTypeId = typeIdFromClassName(Asteroid::staticMetaObject.className());

template<>
void WorldObject::registerType<Asteroid>()
{
    qRegisterMetaType<AsteroidStyle>();
    qRegisterMetaTypeStreamOperators<AsteroidStyle>();
    qRegisterMetaType<AsteroidStyle::Data>();
    qRegisterMetaTypeStreamOperators<AsteroidStyle::Data>();
    qmlRegisterType<Asteroid>("OpenSR.World", 1, 0, "Asteroid");
}

template<>
Asteroid* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Asteroid(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Asteroid>()
{
    return Asteroid::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Asteroid>()
{
    return &Asteroid::staticMetaObject;
}

QString AsteroidStyle::texture() const
{
    return getData<Data>().texture;
}

QColor AsteroidStyle::color() const
{
    return getData<Data>().color;
}

void AsteroidStyle::setTexture(const QString& texture)
{
    auto d = getData<Data>();
    d.texture = texture;
    setData(d);
}

void AsteroidStyle::setColor(const QColor& color)
{
    auto d = getData<Data>();
    d.color = color;
    setData(d);
}

QDataStream& operator<<(QDataStream & stream, const AsteroidStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, AsteroidStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const AsteroidStyle::Data& data)
{
    return stream << data.color << data.texture;
}

QDataStream& operator>>(QDataStream & stream, AsteroidStyle::Data& data)
{
    return stream >> data.color >> data.texture;
}

Asteroid::Asteroid(WorldObject *parent, quint32 id): SpaceObject(parent, id)
{
}

Asteroid::~Asteroid()
{
}

AsteroidStyle Asteroid::style() const
{
    return m_style;
}

void Asteroid::setStyle(const AsteroidStyle& style)
{
    m_style = style;
    emit(styleChanged());
}

quint32 Asteroid::typeId() const
{
    return Asteroid::m_staticTypeId;
}

QString Asteroid::namePrefix() const
{
    return tr("Asteroid");
}

void Asteroid::prepareSave()
{
    m_style.registerResource();
}
}
}
