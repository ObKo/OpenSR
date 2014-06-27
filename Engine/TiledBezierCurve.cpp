/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/TiledBezierCurve.h"
#include "OpenSR/private/TiledBezierCurve_p.h"
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/Log.h>

namespace
{
const float ANGLE_TRESHOLD = 0.9999f;
}

namespace Rangers
{
TiledBezierCurve::TiledBezierCurve(): TiledPolyline(*(new TiledBezierCurvePrivate()))
{
}

TiledBezierCurve::TiledBezierCurve(boost::shared_ptr<Texture> texture): TiledPolyline(*(new TiledBezierCurvePrivate()))
{
    RANGERS_D(TiledBezierCurve);
    d->texture = texture;
}

TiledBezierCurve::TiledBezierCurve(const std::string& texture): TiledPolyline(*(new TiledBezierCurvePrivate()))
{
    RANGERS_D(TiledBezierCurve);
    d->texture = ResourceManager::instance().loadTexture(texture);
}

TiledBezierCurve::~TiledBezierCurve()
{
}

TiledBezierCurve::TiledBezierCurve(TiledBezierCurvePrivate &p): TiledPolyline(p)
{
}

TiledBezierCurvePrivate::TiledBezierCurvePrivate()
{
}

void TiledBezierCurve::setCurve(const BezierCurve& curve)
{
    RANGERS_D(TiledBezierCurve);
    d->curve = curve;
    calcCurve();
}

BezierCurve TiledBezierCurve::curve() const
{
    RANGERS_D(const TiledBezierCurve);
    return d->curve;
}

Vector TiledBezierCurvePrivate::calcBezierPoint(float t, Vector& d)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    
    d = (- 3 * tt + 6 * t - 3) * curve.p0 + (9 * tt - 12 * t + 3) * curve.p1 + (-9 * tt + 6 * t) * curve.p2 + 3 * tt * curve.p3;
    Vector p = uuu * curve.p0 + 3 * uu * t * curve.p1 + 3 * u * tt * curve.p2 + ttt * curve.p3;

    return p;
}

void TiledBezierCurvePrivate::findPoints(float t0, float t1, std::list<Vector>& points, std::list<Vector>::iterator& i)
{
    float tMid = (t0 + t1) / 2;
    Vector dl, dr, dc;
    Vector p0 = calcBezierPoint(t0, dl);
    Vector p1 = calcBezierPoint(t1, dr);

    if (texture && ((p0 - p1).length() < texture->width()))
    {
        return;
    }
    //Vector leftDirection = (p0 - pMid).norm();
    //Vector rightDirection = (p1 - pMid).norm();
    Vector pMid = calcBezierPoint(tMid, dc);
    dl = dl.norm();
    dr = dr.norm();
    dc = dc.norm();

    if (fabs(dl * dc) < ANGLE_TRESHOLD)
        findPoints(t0, tMid, points, i);
    
    points.insert(i, pMid);
    
    if (fabs(dr * dc) < ANGLE_TRESHOLD)
        findPoints(tMid, t1, points, i);
}

void TiledBezierCurve::calcCurve()
{
    RANGERS_D(TiledBezierCurve);

    std::list<Vector> points;
    
    Vector dir;
    Vector p0 = d->calcBezierPoint(0.0f, dir);
    Vector p1 = d->calcBezierPoint(1.0f, dir);
    points.push_back(p0);
    points.push_back(p1);

    std::list<Vector>::iterator i = points.begin();
    ++i;

    d->findPoints(0.0f, 1.0f, points, i);
    d->points = std::vector<Vector>(points.size());
    std::copy(points.begin(), points.end(), d->points.begin());

    markToUpdate();
}
}
