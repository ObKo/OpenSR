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

#include <boost/python.hpp>
#include <OpenSR/World/python/WorldObjectWrap.h>

#include <OpenSR/World/SpaceObject.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct SpaceObjectWrap: SpaceObject, boost::python::wrapper<SpaceObject>
{
    SpaceObjectWrap(uint64_t id = 0): SpaceObject(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(SpaceObject)
};

void exportSpaceObject()
{
    using namespace boost::python;

    class_<SpaceObjectWrap, bases<WorldObject>, boost::shared_ptr<SpaceObjectWrap>, boost::noncopyable> c("SpaceObject", init<uint64_t>());
    c.def(init<>())
    .def("position", &SpaceObject::position)
    .def("setPosition", &SpaceObject::setPosition);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(SpaceObject, SpaceObjectWrap, c);
    register_ptr_to_python<boost::shared_ptr<SpaceObject> >();
}
}
}
}