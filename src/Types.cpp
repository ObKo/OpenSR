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
#include "Texture.h"

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
    x = 0;
    y = 0;
    width = -1;
    height = -1;
}

Rect::Rect(float x1, float y1, float x2, float y2)
{
    x = x1;
    y = y1;
    width = x2 - x1;
    height = y2 - y1;
}

Rect& Rect::operator+=(const Rect& other)
{
    if(other.width < 0 || other.height < 0 || width < 0 || height < 0)
        return *this;
    float ox1 = other.x, oy1 = other.y, ox2 = other.x + other.width, oy2 = other.y + other.height;
    float x1 = x, y1 = y, x2 = x + width, y2 = y + height;
    if (ox2 > x2)
        x2 = ox2;
    if (oy2 > y2)
        y2 = oy2;
    if (ox1 < x1)
        x1 = ox1;
    if (oy1 < y1)
        y1 = oy1;
    x = x1;
    y = y1;
    width = x2 - x1;
    height = y2 - y1;
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
    if(width < 0 || height < 0)
        return false;      
    return (v.x > x) && (v.x < x + width) && (v.y > y) && (v.y < y + height);
}

TextureRegion::TextureRegion(boost::shared_ptr<Texture> texture, float x, float y, float width, float height)
{
    this->texture = texture;
    if (texture)
    {
        this->u1 = float(x) / texture->width();
        this->u2 = float(x + width) / texture->width();
        this->v1 = float(y) / texture->height();
        this->v2 = float(y + height) / texture->height();
    }
    else
    {
        this->u1 = 0;
        this->u2 = 0;
        this->v1 = 0;
        this->v2 = 0;
    }
}

TextureRegion::TextureRegion(boost::shared_ptr<Texture> texture)
{
    this->texture = texture;
    if (texture)
    {
        this->u1 = 0.0f;
        this->u2 = 1.0f;
        this->v1 = 0.0f;
        this->v2 = 1.0f;
    }
    else
    {
        this->u1 = 0.0f;
        this->u2 = 0.0f;
        this->v1 = 0.0f;
        this->v2 = 0.0f;
    }
}

TextureRegion::TextureRegion()
{
    this->texture = boost::shared_ptr<Texture>();
    this->u1 = 0;
    this->u2 = 0;
    this->v1 = 0;
    this->v2 = 0;
}
}
