/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#include "Types.h"

namespace Rangers
{
Vector::Vector()
{
    x = 0;
    y = 0;
}

Vector::Vector(float x, float y)
{
    this->x = x;
    this->y = y;
}

Rect::Rect()
{
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
}

Rect::Rect(float x1, float y1, float x2, float y2): x1(x1), y1(y1), x2(x2), y2(y2)
{
}

Rect& Rect::operator+=(const Rect& other)
{
    if (other.x2 > x2)
        x2 = other.x2;
    if (other.y2 > y2)
        y2 = other.y2;
    if (other.x1 < x1)
        x1 = other.x1;
    if (other.y1 < y1)
        y1 = other.y1;
    return *this;
}

Rect operator+(const Rect& r1, const Rect& r2)
{
    Rect result(r1);
    result += r2;
    return result;
}

bool Rect::contains(const Vector& v)
{
    return (v.x > x1) && (v.x < x2) && (v.y > y1) && (v.y < y2);
}
}
