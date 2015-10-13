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

#ifndef OPENSR_WORLD_SPACEOBJECT_H
#define OPENSR_WORLD_SPACEOBJECT_H

#include "World.h"
#include "WorldObject.h"

#include <OpenSR/Types.h>

#include <QPointF>

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API SpaceObject: public WorldObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVariantList trajectory READ trajectory NOTIFY trajectoryChanged STORED false)

public:
    Q_INVOKABLE SpaceObject(WorldObject *parent = 0, quint32 id = 0);
    virtual ~SpaceObject();

    QPointF position() const;
    QVariantList trajectory() const;

    void setPosition(const QPointF& pos);

    virtual void updateTrajectory();

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

protected:
    void setTrajectory(const QList<BezierCurve>& trajectory);

Q_SIGNALS:
    void positionChanged();
    void trajectoryChanged();

private:
    QPointF m_position;
    QList<BezierCurve> m_trajectory;
};
}
}

#endif // OPENSR_WORLD_SPACEOBJECT_H
