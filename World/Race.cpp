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
#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 Race::m_staticTypeId = typeIdFromClassName(Race::staticMetaObject.className());

template<>
void WorldObject::registerType<Race>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<RaceStyle>();
    qRegisterMetaTypeStreamOperators<RaceStyle>();
    qRegisterMetaType<RaceStyle::Data>();
    qRegisterMetaTypeStreamOperators<RaceStyle::Data>();
    qmlRegisterType<Race>("OpenSR.World", 1, 0, "Race");
}

template<>
Race* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Race(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Race>()
{
    return Race::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Race>()
{
    return &Race::staticMetaObject;
}

QString RaceStyle::icon() const
{
    return getData<Data>().icon;
}

QColor RaceStyle::color() const
{
    return getData<Data>().color;
}

QString RaceStyle::sound() const
{
    return getData<Data>().sound;
}

void RaceStyle::setIcon(const QString& icon)
{
    auto d = getData<Data>();
    d.icon = icon;
    setData(d);
}

void RaceStyle::setColor(const QColor& color)
{
    auto d = getData<Data>();
    d.color = color;
    setData(d);
}

void RaceStyle::setSound(QString& sound)
{
    auto d = getData<Data>();
    d.sound = sound;
    setData(d);
}

Race::Race(WorldObject *parent, quint32 id): WorldObject(parent, id)
{
}

Race::~Race()
{
}

quint32 Race::typeId() const
{
    return Race::m_staticTypeId;
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

void Race::prepareSave()
{
    m_style.registerResource();
}

QDataStream& operator<<(QDataStream & stream, const RaceStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, RaceStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const RaceStyle::Data& data)
{
    return stream << data.color << data.icon << data.sound;
}

QDataStream& operator>>(QDataStream & stream, RaceStyle::Data& data)
{
    return stream >> data.color >> data.icon >> data.sound;
}
}
}
