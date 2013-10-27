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

#include <OpenSR/World/Planet.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct PlanetWrap: Planet, boost::python::wrapper<Planet>
{
    PlanetWrap(uint64_t id = 0): Planet(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(Planet)

    static void setStyle(Planet &self, boost::python::object o)
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
};

void exportPlanet()
{
    using namespace boost::python;

    class_<PlanetWrap, bases<SystemObject>, boost::shared_ptr<PlanetWrap>, boost::noncopyable> c("Planet", init<uint64_t>());
    c.def(init<>())
    .add_property("radius", &Planet::radius, &Planet::setRadius)
    .add_property("orbit", &Planet::orbit, &Planet::setOrbit)
    .add_property("style", &Planet::style, &PlanetWrap::setStyle)
    .add_property("angle", &Planet::angle, &Planet::setAngle)
    .add_property("angleSpeed", &Planet::angleSpeed, &Planet::setAngleSpeed);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Planet, PlanetWrap, c);
    register_ptr_to_python<boost::shared_ptr<Planet> >();
}
}
}
}