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

    Q_PROPERTY(StationKind StationKind READ Kind WRITE setStationKind NOTIFY StationKindChanged)
    Q_PROPERTY(OpenSR::World::StationStyle style READ style WRITE setStyle NOTIFY styleChanged)

public:
    enum class StationKind {
        Unspecified = 0,
        RangerCenter, ScienceBase, MilitaryBase, BusinessCenter, MedicalCenter
    };
    Q_ENUM(StationKind)
    static const quint32 m_StationKindStaticTypeId;

public:
    Q_INVOKABLE SpaceStation(WorldObject *parent = 0, quint32 id = 0);
    virtual ~SpaceStation();

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

    StationKind Kind() const;
    OpenSR::World::StationStyle style() const;

    virtual void prepareSave();

public slots:
    void setStationKind(StationKind kind);
    void setStyle(OpenSR::World::StationStyle style);

signals:
    void StationKindChanged(StationKind kind);
    void styleChanged(OpenSR::World::StationStyle style);

private:
    // TODO: the m_stationKind should be of type StationKind but to fix saving these
    // values we need to resolve issues with forward declarations and registering
    // metatypes in the WorldManager class.
    StationKind m_StationKind;
    OpenSR::World::StationStyle m_style;
};
}
}

Q_DECLARE_METATYPE(OpenSR::World::StationStyle)
Q_DECLARE_METATYPE(OpenSR::World::StationStyle::Data)
Q_DECLARE_METATYPE(OpenSR::World::SpaceStation::StationKind)

#endif // OPENSR_WORLD_SPACESTATION_H
