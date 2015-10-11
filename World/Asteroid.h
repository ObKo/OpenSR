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

#ifndef OPENSR_WORLD_ASTEROID_H
#define OPENSR_WORLD_ASTEROID_H

#include "World.h"
#include "SpaceObject.h"
#include "Resource.h"

#include <QColor>

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API AsteroidStyle: public Resource
{
    Q_GADGET

    Q_PROPERTY(QString texture READ texture WRITE setTexture)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    struct Data
    {
        QString texture;
        QColor color;
    };

    QString texture() const;
    QColor color() const;

    void setTexture(const QString& texture);
    void setColor(const QColor& color);
};

QDataStream& operator<<(QDataStream & stream, const AsteroidStyle& style);
QDataStream& operator>>(QDataStream & stream, AsteroidStyle& style);
QDataStream& operator<<(QDataStream & stream, const AsteroidStyle::Data& data);
QDataStream& operator>>(QDataStream & stream, AsteroidStyle::Data& data);

class OPENSR_WORLD_API Asteroid: public SpaceObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(OpenSR::World::AsteroidStyle style READ style WRITE setStyle NOTIFY styleChanged)

public:
    Q_INVOKABLE Asteroid(WorldObject *parent = 0, quint32 id = 0);
    virtual ~Asteroid();

    AsteroidStyle style() const;

    void setStyle(const AsteroidStyle& style);

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

    virtual void prepareSave();

Q_SIGNALS:
    void styleChanged();

private:
    AsteroidStyle m_style;
};
}
}

Q_DECLARE_METATYPE(OpenSR::World::AsteroidStyle)
Q_DECLARE_METATYPE(OpenSR::World::AsteroidStyle::Data)

#endif // OPENSR_WORLD_ASTEROID_H
