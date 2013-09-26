/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_TILED_BEIZER_CURVE_H
#define RANGERS_TILED_BEIZER_CURVE_H

#include "OpenSR/TiledPolyline.h"

namespace Rangers
{
class Vector;
class Texture;
class TiledBeizerCurvePrivate;
class RANGERS_ENGINE_API TiledBeizerCurve: public TiledPolyline
{
    RANGERS_DECLARE_PRIVATE(TiledBeizerCurve)
public:
    TiledBeizerCurve();
    TiledBeizerCurve(boost::shared_ptr<Texture> texture);
    TiledBeizerCurve(const std::wstring& texture);

    virtual ~TiledBeizerCurve();

    void setControlPoints(Vector p0, Vector p1, Vector p2, Vector p3);

protected:
    TiledBeizerCurve(TiledBeizerCurvePrivate &p);

    void calcCurve();

    RANGERS_DISABLE_COPY(TiledBeizerCurve)

};
}

#endif
