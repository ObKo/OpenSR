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

#ifndef OPENSR_WORLD_PLANETARYSYSTEM_H
#define OPENSR_WORLD_PLANETARYSYSTEM_H

#include "World.h"
#include "WorldObject.h"
#include "Resource.h"

#include <QColor>

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API PlanetarySystemStyle: public Resource
{
    Q_GADGET

    Q_PROPERTY(QString background READ background WRITE setBackground)
    Q_PROPERTY(QString star READ star WRITE setStar)
    Q_PROPERTY(QColor starColor READ starColor WRITE setStarColor)

public:
    struct Data
    {
        QString background;
        QString star;
        QColor starColor;
    };

    QString background() const;
    QString star() const;
    QColor starColor() const;

    void setBackground(const QString& bg);
    void setStar(QString& star);
    void setStarColor(const QColor& color);
};

QDataStream& operator<<(QDataStream & stream, const PlanetarySystemStyle& style);
QDataStream& operator>>(QDataStream & stream, PlanetarySystemStyle& style);
QDataStream& operator<<(QDataStream & stream, const PlanetarySystemStyle::Data& data);
QDataStream& operator>>(QDataStream & stream, PlanetarySystemStyle::Data& data);

class OPENSR_WORLD_API PlanetarySystem: public WorldObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(OpenSR::World::PlanetarySystemStyle style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

public:
    Q_INVOKABLE PlanetarySystem(WorldObject *parent = 0, quint32 id = 0);
    virtual ~PlanetarySystem();

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

    PlanetarySystemStyle style() const;
    int size() const;

    void setStyle(const PlanetarySystemStyle& style);
    void setSize(int size);

    virtual void prepareSave();

Q_SIGNALS:
    void styleChanged();
    void sizeChanged();

private:
    PlanetarySystemStyle m_style;
    int m_size;
};
}
}

Q_DECLARE_METATYPE(OpenSR::World::PlanetarySystemStyle::Data)
Q_DECLARE_METATYPE(OpenSR::World::PlanetarySystemStyle)

#endif // OPENSR_WORLD_PLANETARYSYSTEM_H
