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

#include <OpenSR/Object.h>
#include <boost/python.hpp>

#include "OpenSR/python/ObjectWrap.h"

namespace Rangers
{
namespace Python
{
struct ObjectWrap: Object, boost::python::wrapper<Object>
{
    ObjectWrap(): Object()
    {
    }
    RANGERS_PYTHON_WRAP_OBJECT(Object)

    static void setPosition(Object& self, boost::python::object pos)
    {
        boost::python::extract<Vector> v(pos);
        boost::python::extract<boost::python::tuple> t(pos);
        if (v.check())
        {
            self.setPosition(v);
        }
        else if (t.check())
        {
            boost::python::tuple p = t;
            self.setPosition(boost::python::extract<float>(p[0]), boost::python::extract<float>(p[1]));
        }
        else
        {
            //FIXME: Ugly way to throw TypeError
            self.setPosition(boost::python::extract<Vector>(pos));
        }
    }

    static void setColor(Object& self, boost::python::object pos)
    {
        boost::python::extract<uint32_t> u(pos);
        boost::python::extract<boost::python::tuple> t(pos);
        if (u.check())
        {
            self.setColor(u);
        }
        else if (t.check())
        {
            boost::python::tuple p = t;
            if (boost::python::len(p) == 3)
            {
                self.setColor(boost::python::extract<float>(p[0]), boost::python::extract<float>(p[1]),
                              boost::python::extract<float>(p[2]));
            }
            else
            {
                self.setColor(boost::python::extract<float>(p[0]), boost::python::extract<float>(p[1]),
                              boost::python::extract<float>(p[2]), boost::python::extract<float>(p[3]));
            }
        }
        else
        {
            //FIXME: Ugly way to throw TypeError
            self.setColor(boost::python::extract<uint32_t>(pos));
        }
    }

    static boost::python::list children(Object& self)
    {
        return listToPythonList(self.children());
    }

};

void exportObject()
{
    using namespace boost::python;
    class_<Python::ObjectWrap, boost::shared_ptr<ObjectWrap>, boost::noncopyable> c("Object", init<>());
    c.add_property("position", &Object::position, &ObjectWrap::setPosition)
    .add_property("rotation", &Object::rotation, &Object::setRotation)
    .add_property("color", &Object::color, &ObjectWrap::setColor)
    .add_property("layer", &Object::layer, &Object::setLayer)
    .add_property("children", &ObjectWrap::children)
    .def("addChild", &Object::addChild)
    .def("removeChild", &Object::removeChild)
    .def("parent", &Object::parent, boost::python::return_internal_reference<>())
    .def("needUpdate", &Object::needUpdate)
    .def("mapFromGlobal", &Object::mapFromGlobal)
    .def("mapFromParent", &Object::mapFromParent)
    .def("mapFromSceen", &Object::mapFromScreen)
    .def("mapToGlobal", (Vector(Object::*)(const Vector&) const)&Object::mapToGlobal)
    .def("mapToParent", (Vector(Object::*)(const Vector&) const)&Object::mapToParent)
    .def("mapToSceen", (Vector(Object::*)(const Vector&) const)&Object::mapToScreen)
    .def("mapToGlobal", (Rect(Object::*)(const Rect&) const)&Object::mapToGlobal)
    .def("mapToParent", (Rect(Object::*)(const Rect&) const)&Object::mapToParent)
    .def("mapToSceen", (Rect(Object::*)(const Rect&) const)&Object::mapToScreen);
    RANGERS_PYTHON_WRAP_OBJECT_DEF(Object, ObjectWrap, c);
    register_ptr_to_python<boost::shared_ptr<Object> >();
}
}
}
