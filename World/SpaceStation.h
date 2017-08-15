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

#ifndef OPENSR_WORLD_SPACESTATION_H
#define OPENSR_WORLD_SPACESTATION_H

#include "World.h"
#include "MannedObject.h"
#include "Resource.h"

namespace OpenSR
{
namespace World
{

class OPENSR_WORLD_API StationStyle: public Resource
{
    Q_GADGET
    Q_PROPERTY(QString texture READ texture WRITE setTexture)

public:
    struct Data {
        QString texture;
    };

    QString texture() const;
    void setTexture(const QString& texture);
};

bool operator==(const StationStyle& one, const StationStyle& another);

QDataStream& operator<<(QDataStream & stream, const StationStyle& style);
QDataStream& operator>>(QDataStream & stream, StationStyle& style);
QDataStream& operator<<(QDataStream & stream, const StationStyle::Data& data);
QDataStream& operator>>(QDataStream & stream, StationStyle::Data& data);

class OPENSR_WORLD_API SpaceStation: public MannedObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(StationKind stationKind READ stationKind WRITE setStationKind NOTIFY stationKindChanged)
    Q_PROPERTY(OpenSR::World::StationStyle style READ style WRITE setStyle NOTIFY styleChanged)

public:
    enum class StationKind {
        Unspecified = 0,
        RangerCenter, ScienceBase, MilitaryBase, BusinessCenter, MedicalCenter
    };
    Q_ENUM(StationKind)

public:
    Q_INVOKABLE SpaceStation(WorldObject *parent = 0, quint32 id = 0);
    virtual ~SpaceStation();

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

    StationKind stationKind() const;
    StationStyle style() const;

    virtual void prepareSave();

public slots:
    void setStationKind(StationKind kind);
    void setStyle(const StationStyle& style);

signals:
    void stationKindChanged(StationKind kind);
    void styleChanged(const StationStyle& style);

private:
    StationKind m_stationKind;
    OpenSR::World::StationStyle m_style;
};
}
}

Q_DECLARE_METATYPE(OpenSR::World::StationStyle)
Q_DECLARE_METATYPE(OpenSR::World::StationStyle::Data)

#endif // OPENSR_WORLD_SPACESTATION_H
