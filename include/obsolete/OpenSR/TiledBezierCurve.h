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

#ifndef RANGERS_TILED_BEZIER_CURVE_H
#define RANGERS_TILED_BEZIER_CURVE_H

#include "OpenSR/TiledPolyline.h"

namespace Rangers
{
class Vector;
class Texture;
class TiledBezierCurvePrivate;
class RANGERS_ENGINE_API TiledBezierCurve: public TiledPolyline
{
    RANGERS_DECLARE_PRIVATE(TiledBezierCurve)
public:
    TiledBezierCurve();
    TiledBezierCurve(boost::shared_ptr<Texture> texture);
    TiledBezierCurve(const std::string& texture);

    virtual ~TiledBezierCurve();

    void setCurve(const BezierCurve& curve);

    BezierCurve curve() const;

protected:
    TiledBezierCurve(TiledBezierCurvePrivate &p);

    void calcCurve();

    RANGERS_DISABLE_COPY(TiledBezierCurve)

};
}

#endif
