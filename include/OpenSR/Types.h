/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

//! Struct that handles region of texture.
struct RANGERS_ENGINE_API TextureRegion
{
    //! Constructs texture region.
    TextureRegion(boost::shared_ptr<Texture> texture, int x, int y, int width, int height);
    TextureRegion(const TextureRegionDescriptor& desc);
    TextureRegion(boost::shared_ptr<Texture> texture);
    TextureRegion();
    boost::shared_ptr<Texture> texture;
    float u1, v1, u2, v2;
};

struct RANGERS_ENGINE_API NinePatchDescriptor
{
    int rows;
    int columns;
    std::vector<TextureRegionDescriptor> regions;
    std::vector<int> sizeableRows;
    std::vector<int> sizeableColumns;
    std::vector<int> tiledRegions;
};

struct RANGERS_ENGINE_API FontDescriptor
{
    std::wstring path;
    int size;
};

struct RANGERS_ENGINE_API TextureRegionDescriptor
{
    std::wstring texture;
    int x, y, width, height;
};

RANGERS_ENGINE_API Rect operator+(const Rect& r1, const Rect& r2);
RANGERS_ENGINE_API Vector operator+(const Vector& v1, const Vector& v2);
RANGERS_ENGINE_API Vector operator-(const Vector& v1, const Vector& v2);
RANGERS_ENGINE_API float operator*(const Vector& v1, const Vector& v2);
RANGERS_ENGINE_API Vector operator*(const Vector& v, float s);
RANGERS_ENGINE_API Vector operator*(float s, const Vector& v);
}

#endif
