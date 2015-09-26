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

#include "OpenSR/Types.h"
#include <QVariantList>
#include <QLinkedList>
#include <QVector2D>

namespace OpenSR
{
namespace
{
const float ANGLE_TRESHOLD = 0.9999f;

QVector2D calcBezierPoint(const BezierCurve& curve, float t, QVector2D& d)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    d = (- 3 * tt + 6 * t - 3) * QVector2D(curve.p0) +
        (9 * tt - 12 * t + 3) * QVector2D(curve.p1) +
        (-9 * tt + 6 * t) * QVector2D(curve.p2) +
        3 * tt * QVector2D(curve.p3);

    QVector2D p = uuu * QVector2D(curve.p0) +
                  3 * uu * t * QVector2D(curve.p1) +
                  3 * u * tt * QVector2D(curve.p2) +
                  ttt * QVector2D(curve.p3);

    return p;
}

void findPoints(const BezierCurve& curve, int minSize, float t0, float t1,
                QLinkedList<QPointF>& points, QLinkedList<QPointF>::Iterator& i)
{
    float tMid = (t0 + t1) / 2;
    QVector2D dl, dr, dc;
    QVector2D p0 = calcBezierPoint(curve, t0, dl);
    QVector2D p1 = calcBezierPoint(curve, t1, dr);

    if ((p0 - p1).length() < minSize)
    {
        return;
    }

    QVector2D pMid = calcBezierPoint(curve, tMid, dc);
    dl.normalize();
    dr.normalize();
    dc.normalize();

    if (qAbs(QVector2D::dotProduct(dl, dc)) < ANGLE_TRESHOLD)
        findPoints(curve, minSize, t0, tMid, points, i);

    i = points.insert(i, pMid.toPointF());
    ++i;

    if (qAbs(QVector2D::dotProduct(dr, dc)) < ANGLE_TRESHOLD)
        findPoints(curve, minSize, tMid, t1, points, i);
}
}

QList<QPointF> BezierCurve::calcPolyline(int minStep)
{
    QLinkedList<QPointF> points;

    QVector2D dir;
    QVector2D p0 = calcBezierPoint(*this, 0.0f, dir);
    QVector2D p1 = calcBezierPoint(*this, 1.0f, dir);

    points.append(p0.toPointF());
    points.append(p1.toPointF());

    QLinkedList<QPointF>::Iterator i = points.begin();
    ++i;

    findPoints(*this, minStep, 0.0f, 1.0f, points, i);

    QList<QPointF> result;

    for (const QPointF& p : points)
        result.append(p);

    return result;
}
}
