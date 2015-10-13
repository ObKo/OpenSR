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

#ifndef OPENSR_TYPES_H
#define OPENSR_TYPES_H

#include <OpenSR/OpenSR.h>
#include <QPointF>
#include <QMetaType>
#include <QList>
#include <QObject>

namespace OpenSR
{
class BezierCurve
{
    Q_GADGET

    Q_PROPERTY(QPointF p0 MEMBER p0)
    Q_PROPERTY(QPointF p1 MEMBER p1)
    Q_PROPERTY(QPointF p2 MEMBER p2)
    Q_PROPERTY(QPointF p3 MEMBER p3)

public:
    QPointF p0, p1, p2, p3;

    QList<QPointF> calcPolyline(int minStep = 1);
};
}

Q_DECLARE_METATYPE(OpenSR::BezierCurve)

#endif // OPENSR_TYPES_H
