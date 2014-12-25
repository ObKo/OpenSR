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

#include "OpenSR/Types.h"

#include "OpenSR/Texture.h"
#include "OpenSR/ResourceManager.h"
#include <OpenSR/Log.h>

#include <cmath>

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

Vector Vector::norm() const
{
    float l = length();
    return Vector(x / l, y / l);
}

float Vector::length() const
{
    return float(sqrt((*this) * (*this)));
}

bool Vector::operator==(const Vector& other)
{
    return (other.x == x) && (other.y == y);
}

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3):
    p0(p0), p1(p1), p2(p2), p3(p3)
{
}

Color::Color()
{
    r = 1.0f;
    g = 1.0f;
    b = 1.0f;
    a = 1.0f;
}

Color::Color(float red, float green, float blue, float alpha)
{
    r = std::min(std::max(red, 0.0f), 1.0f);
    g = std::min(std::max(green, 0.0f), 1.0f);
    b = std::min(std::max(blue, 0.0f), 1.0f);
    a = std::min(std::max(alpha, 0.0f), 1.0f);
}

Color Color::fromUInt(uint32_t color)
{
    return Color(((color >> 16) & 0xFF) / 255.0f,
                 ((color >> 8) & 0xFF) / 255.0f,
                 ((color) & 0xFF) / 255.0f,
                 ((color >> 24) & 0xFF) / 255.0f);
}

Color Color::fromString(const std::string& color)
{
    if (color.at(0) != '#')
    {
        Log::warning() << "Invalid color string: " << color;
        return Color();
    }

    Color c;
    try
    {
        if (color.length() == 7)
            c = fromUInt(std::stoul(color.substr(1), 0, 16) | 0xFF000000);
        else if (color.length() == 9)
            c = fromUInt(std::stoul(color.substr(1), 0, 16));
        else
            Log::warning() << "Invalid color string: " << color;

    }
    catch (std::invalid_argument &e)
    {
        Log::warning() << "Invalid color string: " << color;
    }
    return c;
}

uint32_t Color::toRGB() const
{
    uint8_t red = uint8_t(std::min(std::max(r, 0.0f), 1.0f) * 255.0f);
    uint8_t green = uint8_t(std::min(std::max(g, 0.0f), 1.0f) * 255.0f);
    uint8_t blue = uint8_t(std::min(std::max(b, 0.0f), 1.0f) * 255.0f);
    return (red << 16) | (green << 8) | (blue);
}

uint32_t Color::toARGB() const
{
    uint8_t red = uint8_t(std::min(std::max(r, 0.0f), 1.0f) * 255.0f);
    uint8_t green = uint8_t(std::min(std::max(g, 0.0f), 1.0f) * 255.0f);
    uint8_t blue = uint8_t(std::min(std::max(b, 0.0f), 1.0f) * 255.0f);
    uint8_t alpha = uint8_t(std::min(std::max(a, 0.0f), 1.0f) * 255.0f);
    return (alpha << 24) | (red << 16) | (green << 8) | (blue);
}

std::string Color::toString(bool alpha) const
{
    //FIXME: C-way...
    uint32_t c = toARGB();
    char result[10] = "";
    if (alpha)
        snprintf(result, 10, "#%08X\0", c);
    else
        snprintf(result, 8, "#%06X\0", c & 0xFFFFFF);
    return std::string(result);
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
    if (other.width < 0 || other.height < 0 || width < 0 || height < 0)
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
    if (width < 0 || height < 0)
        return false;
    return (v.x > x) && (v.x < x + width) && (v.y > y) && (v.y < y + height);
}

bool Rect::valid() const
{
    return (width >= 0) && (height >= 0);
}

TextureRegion::TextureRegion(boost::shared_ptr<Texture> texture, int x, int y, int width, int height)
{
    this->texture = texture;
    if (texture)
    {
        if (width < 0)
            width = texture->width();
        if (height < 0)
            height = texture->height();
        this->u1 = float(x) / (texture->width() - 1);
        this->u2 = float(x + width - 1) / (texture->width() - 1);
        this->v1 = float(y) / (texture->height() - 1);
        this->v2 = float(y + height - 1) / (texture->height() - 1);
    }
    else
    {
        this->u1 = 0;
        this->u2 = 0;
        this->v1 = 0;
        this->v2 = 0;
    }
}

TextureRegion::TextureRegion(boost::shared_ptr<TextureRegionDescriptor> desc)
{
    if (!desc)
    {
        this->u1 = 0;
        this->u2 = 0;
        this->v1 = 0;
        this->v2 = 0;
        return;
    }

    texture = ResourceManager::instance().loadTexture(desc->texture);
    int width = desc->width;
    int height = desc->height;
    if (texture)
    {
        if (width < 0)
            width = texture->width();
        if (height < 0)
            height = texture->height();
        this->u1 = float(desc->x) / texture->width();
        this->u2 = u1 + float(width) / texture->width();
        this->v1 = float(desc->y) / texture->height();
        this->v2 = v1 + float(height) / texture->height();
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

Vector operator+(const Vector& v1, const Vector& v2)
{
    return Vector(v1.x + v2.x, v1.y + v2.y);
}

Vector operator-(const Vector& v1, const Vector& v2)
{
    return Vector(v1.x - v2.x, v1.y - v2.y);
}

float operator*(const Vector& v1, const Vector& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

Vector operator*(const Vector& v, float s)
{
    return Vector(s * v.x, s * v.y);
}

Vector operator*(float s, const Vector& v)
{
    return Vector(s * v.x, s * v.y);
}
}
