/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2014 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_TYPES_H
#define WORLD_TYPES_H

#include "OpenSR/World/world-global.h"
#include <OpenSR/Types.h>

#include <string>
#include <list>
#include <stdint.h>

namespace Rangers
{
namespace World
{
struct RANGERS_WORLD_API Point
{
    Point(float _x, float _y);
    Point();

    float x;
    float y;
};

struct RANGERS_WORLD_API Trajectory
{
    Trajectory();

    std::list<BezierCurve> nextTurns;
};

struct RANGERS_WORLD_API PlanetStyle: public ResourceObject
{
    PlanetStyle();

    std::string id;
    std::string texture;
    std::string cloud;
    bool hasCloud;
    float speed;
    float cloudSpeed;
    std::string ring;
    std::string ringBackground;
    bool hasRing;
    bool hasRingBackground;
    Color ambientColor;
    float ringOffsetX, ringOffsetY;
    float ringBgOffsetX, ringBgOffsetY;
};

struct RANGERS_WORLD_API SystemStyle: public ResourceObject
{
    SystemStyle();

    std::string id;
    std::string starImage;
    std::string starAnimation;

    Color color;
};

struct RANGERS_WORLD_API AsteroidStyle: public ResourceObject
{
    AsteroidStyle();

    std::string id;
    std::string sprite;
    bool animated;
};
}
}

#endif
