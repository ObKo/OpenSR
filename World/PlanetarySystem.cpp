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

#include "PlanetarySystem.h"
#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 PlanetarySystem::m_staticTypeId = typeIdFromClassName(PlanetarySystem::staticMetaObject.className());

template<>
void WorldObject::registerType<PlanetarySystem>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<PlanetarySystemStyle::Data>();
    qRegisterMetaTypeStreamOperators<PlanetarySystemStyle::Data>();
    qRegisterMetaType<PlanetarySystemStyle>();
    qRegisterMetaTypeStreamOperators<PlanetarySystemStyle>();
    qmlRegisterType<PlanetarySystem>("OpenSR.World", 1, 0, "PlanetarySystem");
}

template<>
PlanetarySystem* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new PlanetarySystem(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<PlanetarySystem>()
{
    return PlanetarySystem::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<PlanetarySystem>()
{
    return &PlanetarySystem::staticMetaObject;
}

QString PlanetarySystemStyle::background() const
{
    return getData<Data>().background;
}

QString PlanetarySystemStyle::star() const
{
    return getData<Data>().star;
}

QColor PlanetarySystemStyle::starColor() const
{
    return getData<Data>().starColor;
}

void PlanetarySystemStyle::setBackground(const QString& bg)
{
    Data d = getData<Data>();
    d.background = bg;
    setData(d);
}

void PlanetarySystemStyle::setStar(QString& star)
{
    Data d = getData<Data>();
    d.star = star;
    setData(d);
}

void PlanetarySystemStyle::setStarColor(const QColor& color)
{
    Data d = getData<Data>();
    d.starColor = color;
    setData(d);
}

QDataStream& operator<<(QDataStream & stream, const PlanetarySystemStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, PlanetarySystemStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const PlanetarySystemStyle::Data& data)
{
    return stream << data.background << data.star << data.starColor;
}

QDataStream& operator>>(QDataStream & stream, PlanetarySystemStyle::Data& data)
{
    return stream >> data.background >> data.star >> data.starColor;
}

PlanetarySystem::PlanetarySystem(WorldObject *parent, quint32 id): WorldObject(parent, id)
{
}

PlanetarySystem::~PlanetarySystem()
{
}

quint32 PlanetarySystem::typeId() const
{
    return PlanetarySystem::m_staticTypeId;
}

QString PlanetarySystem::namePrefix() const
{
    return tr("System");
}

PlanetarySystemStyle PlanetarySystem::style() const
{
    return m_style;
}

int PlanetarySystem::size() const
{
    return m_size;
}

void PlanetarySystem::setStyle(const PlanetarySystemStyle& style)
{
    m_style = style;
    emit(styleChanged());
}

void PlanetarySystem::setSize(int size)
{
    if (m_size != size)
    {
        m_size = size;
        emit(sizeChanged());
    }
}

void PlanetarySystem::prepareSave()
{
    WorldObject::prepareSave();
    m_style.registerResource();
}
}
}
