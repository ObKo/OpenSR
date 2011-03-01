/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#ifdef _WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#define BUFFER_OFFSET(i) ((char*)0 + (i))

namespace Rangers
{
class Vector
{
public:
    Vector();
    Vector(float x, float y);

    float x, y;
};

struct Vertex
{
    float x, y;
    float u, w;
};

struct Rect
{
    float x1, y1;
    float x2, y2;
};
};

#endif
