/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_TYPES_H
#define RANGERS_TYPES_H

#include "OpenSR/config.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

namespace Rangers
{
class Texture;
struct TextureRegionDescriptor;

struct RANGERS_ENGINE_API Color
{
    Color();
    Color(float red, float green, float blue, float alpha = 1.0f);

    float r;
    float g;
    float b;
    float a;

    //! To usigned int in 0xRRGGBB format
    uint32_t toRGB() const;
    //! To usigned int in 0xAARRGGBB format
    uint32_t toARGB() const;
    //! To string in "#AARRGGBB" or "#RRGGBB" format
    std::string toString(bool alpha = false) const;

    //! From unsigned int in 0xAARRGGBB format
    static Color fromUInt(uint32_t color);
    //! From string in "#AARRGGBB" or "#RRGGBB" format
    static Color fromString(const std::string& color);
};

struct RANGERS_ENGINE_API Vector
{
    Vector();
    Vector(float x, float y);

    Vector norm() const;
    float length() const;

    float x, y;

    bool operator==(const Vector& other);
    Vector& operator+=(const Vector& other);
};

struct RANGERS_ENGINE_API BeizerCurve
{
    BeizerCurve();
    BeizerCurve(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3);

    Vector p0, p1, p2, p3;
};

struct RANGERS_ENGINE_API Vertex
{
    float x, y;
    float u, v;
};

struct RANGERS_ENGINE_API Rect
{
    Rect();
    Rect(float x1, float y1, float x2, float y2);
    float x, y;
    float width, height;
    Rect& operator+=(const Rect& other);
    bool contains(const Vector& v);
    bool valid() const;
};

struct RANGERS_ENGINE_API ResourceObject
{
    virtual ~ResourceObject() {}
};

struct RANGERS_ENGINE_API TextureRegionDescriptor: public ResourceObject
{
    std::string texture;
    int x, y, width, height;
};

struct RANGERS_ENGINE_API NinePatchDescriptor: public ResourceObject
{
    int rows;
    int columns;
    std::vector<boost::shared_ptr<TextureRegionDescriptor> > regions;
    std::vector<int> sizeableRows;
    std::vector<int> sizeableColumns;
    std::vector<int> tiledRegions;
};

//TODO: Load font from descriptor
struct RANGERS_ENGINE_API FontDescriptor: public ResourceObject
{
    std::string path;
    int size;
    bool antialiasing;
};

struct RANGERS_ENGINE_API SoundDescriptor: public ResourceObject
{
    std::string path;
};

//! Struct that handles region of texture.
struct RANGERS_ENGINE_API TextureRegion
{
    //! Constructs texture region.
    TextureRegion(boost::shared_ptr<Texture> texture, int x, int y, int width, int height);
    TextureRegion(boost::shared_ptr<TextureRegionDescriptor> desc);
    TextureRegion(boost::shared_ptr<Texture> texture);
    TextureRegion();
    boost::shared_ptr<Texture> texture;
    float u1, v1, u2, v2;
};

RANGERS_ENGINE_API Rect operator+(const Rect& r1, const Rect& r2);
RANGERS_ENGINE_API Vector operator+(const Vector& v1, const Vector& v2);
RANGERS_ENGINE_API Vector operator-(const Vector& v1, const Vector& v2);
RANGERS_ENGINE_API float operator*(const Vector& v1, const Vector& v2);
RANGERS_ENGINE_API Vector operator*(const Vector& v, float s);
RANGERS_ENGINE_API Vector operator*(float s, const Vector& v);
}

#endif
