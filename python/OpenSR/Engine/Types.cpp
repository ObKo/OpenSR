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

#include <OpenSR/Types.h>
#include <OpenSR/Texture.h>
#include <OpenSR/python/Wrappers.h>
#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace Rangers
{
namespace Python
{
namespace
{
boost::python::list getRegions(NinePatchDescriptor& self)
{
    return vectorToPythonList(self.regions);
}

void setRegions(NinePatchDescriptor& self, const boost::python::object& r)
{
    self.regions = pythonObjectToVector<boost::shared_ptr<TextureRegionDescriptor> >(r);
}

boost::python::list getSizeableRows(NinePatchDescriptor& self)
{
    return vectorToPythonList(self.sizeableRows);
}

void setSizeableRows(NinePatchDescriptor& self, const boost::python::object& r)
{
    self.sizeableRows = pythonObjectToVector<int>(r);
}

boost::python::list getSizeableColumns(NinePatchDescriptor& self)
{
    return vectorToPythonList(self.sizeableColumns);
}

void setSizeableColumns(NinePatchDescriptor& self, const boost::python::object& c)
{
    self.sizeableColumns = pythonObjectToVector<int>(c);
}
}
void exportTypes()
{
    using namespace boost::python;

    class_<ResourceObject, boost::shared_ptr<ResourceObject> >("ResourceObject");

    class_<Vector>("Vector", init<float, float>())
    .def(init<>())
    .def_readwrite("x", &Vector::x)
    .def_readwrite("y", &Vector::y)
    .add_property("length", &Vector::length)
    .def("norm", &Vector::norm)
    .def(self += self)
    .def(self + self)
    .def(self - self)
    .def(self * self)
    .def(self * float())
    .def(float() * self);

    class_<Color>("Color", init<float, float, float, float>())
    .def(init<float, float, float>())
    .def(init<>())
    .def_readwrite("r", &Color::r)
    .def_readwrite("g", &Color::g)
    .def_readwrite("b", &Color::b)
    .def_readwrite("a", &Color::a)
    .def("toRGB", &Color::toRGB)
    .def("toARGB", &Color::toARGB)
    .def("toString", (std::string(Color::*)(bool) const)&Color::toString)
    .def("toString", (std::string(Color::*)() const)&Color::toString)
    .def("fromUInt", &Color::fromUInt)
    .staticmethod("fromUInt")
    .def("fromString", (Color(*)(const std::wstring&))&Color::fromString)
    .staticmethod("fromString");

    class_<Rect>("Rect", init<float, float, float, float>())
    .def(init<>())
    .def("contains", &Rect::contains)
    .add_property("valid", &Rect::valid)
    .def(self += self)
    .def(self + self)
    .def_readwrite("x", &Rect::x)
    .def_readwrite("y", &Rect::y)
    .def_readwrite("width", &Rect::width)
    .def_readwrite("height", &Rect::height);

    class_<BeizerCurve>("BeizerCurve", init<const Vector&, const Vector&, const Vector&, const Vector&>())
    .def(init<>())
    .def_readwrite("p0", &BeizerCurve::p0)
    .def_readwrite("p0", &BeizerCurve::p1)
    .def_readwrite("p0", &BeizerCurve::p2)
    .def_readwrite("p0", &BeizerCurve::p3);

    class_<TextureRegion>("TextureRegion", init<boost::shared_ptr<Texture>, int, int, int, int>())
    .def(init<boost::shared_ptr<TextureRegionDescriptor> >())
    .def(init<boost::shared_ptr<Texture> >())
    .def(init<>())
    .def_readwrite("texture", &TextureRegion::texture)
    .def_readwrite("u1", &TextureRegion::u1)
    .def_readwrite("v1", &TextureRegion::v1)
    .def_readwrite("u2", &TextureRegion::u2)
    .def_readwrite("v2", &TextureRegion::v2);

    class_<NinePatchDescriptor, boost::shared_ptr<NinePatchDescriptor>, bases<ResourceObject> >("NinePatchDescriptor")
    .def_readwrite("rows", &NinePatchDescriptor::rows)
    .def_readwrite("columns", &NinePatchDescriptor::columns)
    .add_property("regions", &getRegions, &setRegions)
    .add_property("sizeableRows", &getSizeableRows, &setSizeableRows)
    .add_property("sizeableColumns", &getSizeableColumns, &setSizeableColumns);

    class_<FontDescriptor, boost::shared_ptr<FontDescriptor>, bases<ResourceObject> >("FontDescriptor")
    .def_readwrite("path", &FontDescriptor::path)
    .def_readwrite("size", &FontDescriptor::size)
    .def_readwrite("antialiasing", &FontDescriptor::antialiasing);

    class_<TextureRegionDescriptor, boost::shared_ptr<TextureRegionDescriptor>, bases<ResourceObject> >("TextureRegionDescriptor")
    .def_readwrite("texture", &TextureRegionDescriptor::texture)
    .def_readwrite("x", &TextureRegionDescriptor::x)
    .def_readwrite("y", &TextureRegionDescriptor::y)
    .def_readwrite("width", &TextureRegionDescriptor::width)
    .def_readwrite("height", &TextureRegionDescriptor::height);
}
}
}
