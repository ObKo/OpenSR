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
#include "SolarSystem.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct SolarSystemWrap: SolarSystem, boost::python::wrapper<SolarSystem>
{
    SolarSystemWrap(uint64_t id = 0): SolarSystem(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(SolarSystem)
};

void exportSolarSystem()
{
    using namespace boost::python;

    class_<SolarSystemWrap, bases<WorldObject>, boost::shared_ptr<SolarSystem>, boost::noncopyable> c("SolarSystem", init<uint64_t>());
    c.def(init<>())
    .def("name", &SolarSystem::name)
    .def("position", &SolarSystem::position)
    .def("systemObjects", &SolarSystem::systemObjects)
    .def("size", &SolarSystem::size)
    .def("addObject", &SolarSystem::addObject)
    .def("removeObject", &SolarSystem::removeObject)
    .def("setName", &SolarSystem::setName)
    .def("setPosition", &SolarSystem::setPosition)
    .def("setSize", &SolarSystem::setSize);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(SolarSystem, SolarSystemWrap, c);
}
}
}
}