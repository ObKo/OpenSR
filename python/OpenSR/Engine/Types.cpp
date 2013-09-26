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

#include <OpenSR/Types.h>
#include <OpenSR/Texture.h>

#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace Rangers
{
namespace Python
{
void exportTypes()
{
    using namespace boost::python;
    class_<Vector>("Vector", init<float, float>())
    .def(init<>())
    .def_readwrite("x", &Vector::x)
    .def_readwrite("y", &Vector::y);

    class_<std::vector<Vector> >("VectorOfVector")
    .def(vector_indexing_suite<std::vector<Vector> >());

    class_<Rect>("Rect", init<float, float, float, float>())
    .def(init<>())
    .def("contains", &Rect::contains)
    .def("valid", &Rect::valid)
    .def(self + self)
    .def_readwrite("x", &Rect::x)
    .def_readwrite("y", &Rect::y)
    .def_readwrite("width", &Rect::width)
    .def_readwrite("height", &Rect::height);

    class_<TextureRegion>("TextureRegion", init<boost::shared_ptr<Texture>, int, int, int, int>())
    .def(init<const TextureRegionDescriptor&>())
    .def(init<boost::shared_ptr<Texture> >())
    .def(init<>())
    .def_readwrite("texture", &TextureRegion::texture)
    .def_readwrite("u1", &TextureRegion::u1)
    .def_readwrite("v1", &TextureRegion::v1)
    .def_readwrite("u2", &TextureRegion::u2)
    .def_readwrite("v2", &TextureRegion::v2);

    class_<NinePatchDescriptor>("NinePatchDescriptor")
    .def_readwrite("rows", &NinePatchDescriptor::rows)
    .def_readwrite("columns", &NinePatchDescriptor::columns)
    .def_readwrite("regions", &NinePatchDescriptor::regions)
    .def_readwrite("sizeableRows", &NinePatchDescriptor::sizeableRows)
    .def_readwrite("sizeableColumns", &NinePatchDescriptor::sizeableColumns);

    class_<FontDescriptor>("FontDescriptor")
    .def_readwrite("path", &FontDescriptor::path)
    .def_readwrite("size", &FontDescriptor::size);

    class_<TextureRegionDescriptor>("TextureRegionDescriptor")
    .def_readwrite("texture", &TextureRegionDescriptor::texture)
    .def_readwrite("x", &TextureRegionDescriptor::x)
    .def_readwrite("y", &TextureRegionDescriptor::y)
    .def_readwrite("width", &TextureRegionDescriptor::width)
    .def_readwrite("height", &TextureRegionDescriptor::height);
}
}
}

/*
#ifndef RANGERS_TYPES_H
#define RANGERS_TYPES_H

#include "config.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace Rangers
{
class Texture;
class TextureRegionDescriptor;

struct Vector
{
    Vector();
    Vector(float x, float y);

    float x, y;
};

struct Vertex
{
    float x, y;
    float u, v;
};

struct Rect
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
struct TextureRegion
{
    //! Constructs texture region.
    TextureRegion(boost::shared_ptr<Texture> texture, int x, int y, int width, int height);
    TextureRegion(const TextureRegionDescriptor& desc);
    TextureRegion(boost::shared_ptr<Texture> texture);
    TextureRegion();
    boost::shared_ptr<Texture> texture;
    float u1, v1, u2, v2;
};

struct NinePatchDescriptor
{
    int rows;
    int columns;
    std::vector<TextureRegionDescriptor> regions;
    std::vector<int> sizeableRows;
    std::vector<int> sizeableColumns;
};

struct FontDescriptor
{
    std::wstring path;
    int size;
};

struct TextureRegionDescriptor
{
    std::wstring texture;
    int x, y, width, height;
};

Rect operator+(const Rect& r1, const Rect& r2);
}

#endif*/
