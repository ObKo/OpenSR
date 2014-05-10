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
#include <OpenSR/python/Wrappers.h>

#include <OpenSR/World/Types.h>

namespace Rangers
{
namespace World
{
namespace Python
{
namespace
{
boost::python::list getNextTurns(Trajectory& self)
{
    return Rangers::Python::listToPythonList(self.nextTurns);
}

void setNextTurns(Trajectory& self, const boost::python::object& r)
{
    self.nextTurns = Rangers::Python::pythonObjectToList<BeizerCurve>(r);
}
}

void exportTypes()
{
    using namespace boost::python;

    class_<Point> c("Point", init<float, float>());
    c.def(init<>())
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y);

    class_<PlanetStyle, boost::shared_ptr<PlanetStyle>, bases<ResourceObject> > c2("PlanetStyle", init<>());
    c2.def_readwrite("id", &PlanetStyle::id)
    .def_readwrite("texture", &PlanetStyle::texture)
    .def_readwrite("cloud", &PlanetStyle::cloud)
    .def_readwrite("hasCloud", &PlanetStyle::hasCloud)
    .def_readwrite("speed", &PlanetStyle::speed)
    .def_readwrite("cloudSpeed", &PlanetStyle::cloudSpeed)
    .def_readwrite("ring", &PlanetStyle::ring)
    .def_readwrite("ringBackground", &PlanetStyle::ringBackground)
    .def_readwrite("hasRing", &PlanetStyle::hasRing)
    .def_readwrite("hasRingBackground", &PlanetStyle::hasRingBackground)
    .def_readwrite("ambientColor", &PlanetStyle::ambientColor)
    .def_readwrite("ringOffsetX", &PlanetStyle::ringOffsetX)
    .def_readwrite("ringOffsetY", &PlanetStyle::ringOffsetY)
    .def_readwrite("ringBgOffsetX", &PlanetStyle::ringBgOffsetX)
    .def_readwrite("ringBgOffsetY", &PlanetStyle::ringBgOffsetY);

    class_<AsteroidStyle, boost::shared_ptr<AsteroidStyle>, bases<ResourceObject> > c3("AsteroidStyle", init<>());
    c3.def_readwrite("id", &AsteroidStyle::id)
    .def_readwrite("sprite", &AsteroidStyle::sprite)
    .def_readwrite("animated", &AsteroidStyle::animated);

    class_<SystemStyle, boost::shared_ptr<SystemStyle>, bases<ResourceObject> > c4("SystemStyle", init<>());
    c4.def_readwrite("id", &SystemStyle::id)
    .def_readwrite("starImage", &SystemStyle::starImage)
    .def_readwrite("starAnimation", &SystemStyle::starAnimation)
    .def_readwrite("color", &SystemStyle::color);

    class_<Trajectory> c5("Trajectory", init<>());
    c5.add_property("nextTurns", &getNextTurns, &setNextTurns);
}
}
}
}