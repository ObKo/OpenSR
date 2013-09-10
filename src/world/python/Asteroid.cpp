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
#include "Asteroid.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct AsteroidWrap: Asteroid, boost::python::wrapper<Asteroid>
{
    AsteroidWrap(uint64_t id = 0): Asteroid(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(Asteroid)
};

void exportAsteroid()
{
    using namespace boost::python;

    class_<AsteroidWrap, bases<SystemObject>, boost::shared_ptr<AsteroidWrap>, boost::noncopyable> c("Asteroid", init<uint64_t>());
    c.def(init<>())
    .def("a", &Asteroid::a)
    .def("b", &Asteroid::b)
    .def("period", &Asteroid::period)
    .def("angle", &Asteroid::angle)
    .def("time", &Asteroid::time)
    .def("mineral", &Asteroid::mineral)
    .def("style", &Asteroid::style)
    .def("setStyle", (void (Asteroid::*)(uint32_t))&Asteroid::setStyle)
    .def("setStyle", (void (Asteroid::*)(const std::string&))&Asteroid::setStyle)
    .def("setSemiAxis", &Asteroid::setSemiAxis)
    .def("setPeriod", &Asteroid::setPeriod)
    .def("setAngle", &Asteroid::setAngle)
    .def("setTime", &Asteroid::setTime)
    .def("setMineral", &Asteroid::setMineral);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Asteroid, AsteroidWrap, c);
    register_ptr_to_python<boost::shared_ptr<Asteroid> >();
}
}
}
}