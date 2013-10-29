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

#include <OpenSR/World/PlanetarySystem.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct PlanetarySystemWrap: PlanetarySystem, boost::python::wrapper<PlanetarySystem>
{
    PlanetarySystemWrap(uint64_t id = 0): PlanetarySystem(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(PlanetarySystem)

    static void setStyle(PlanetarySystem &self, boost::python::object o)
    {
        boost::python::extract<uint32_t> i(o);
        if (i.check())
        {
            self.setStyle(boost::python::extract<uint32_t>(o));
        }
        else
        {
            self.setStyle(boost::python::extract<std::string>(o));
        }
    }

    static void setPosition(PlanetarySystem &self, boost::python::object o)
    {
        boost::python::extract<Point> p(o);
        if (p.check())
        {
            self.setPosition(boost::python::extract<Point>(o));
        }
        else
        {
            boost::python::tuple t = boost::python::extract<boost::python::tuple>(o);
            self.setPosition(Point(boost::python::extract<float>(t[0]), boost::python::extract<float>(t[1])));
        }
    }

    static boost::python::object systemObjects(PlanetarySystem &self)
    {
        return Rangers::Python::listToPythonList(self.systemObjects());
    }
};

void exportPlanetarySystem()
{
    using namespace boost::python;

    class_<PlanetarySystemWrap, bases<WorldObject>, boost::shared_ptr<PlanetarySystemWrap>, boost::noncopyable> c("PlanetarySystem", init<uint64_t>());
    c.def(init<>())
    .add_property("name", &PlanetarySystem::name, &PlanetarySystem::setName)
    .add_property("position", &PlanetarySystem::position, &PlanetarySystemWrap::setPosition)
    .add_property("systemObjects", &PlanetarySystemWrap::systemObjects)
    .add_property("size", &PlanetarySystem::size, &PlanetarySystem::setSize)
    .add_property("style", &PlanetarySystem::style, &PlanetarySystemWrap::setStyle)
    .def("addObject", &PlanetarySystem::addObject)
    .def("removeObject", &PlanetarySystem::removeObject);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(PlanetarySystem, PlanetarySystemWrap, c);
    register_ptr_to_python<boost::shared_ptr<PlanetarySystem> >();
}
}
}
}