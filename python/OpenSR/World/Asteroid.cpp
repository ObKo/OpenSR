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

#include <OpenSR/World/Asteroid.h>

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

    static void setSemiAxis(Asteroid &self, boost::python::object o)
    {
        boost::python::tuple t = boost::python::extract<boost::python::tuple>(o);
        self.setSemiAxis(boost::python::extract<float>(t[0]), boost::python::extract<float>(t[1]));
    }

    static boost::python::object semiAxis(Asteroid &self)
    {
        return boost::python::make_tuple(self.a(), self.b());
    }

    static void setStyle(Asteroid &self, boost::python::object o)
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

void exportAsteroid()
{
    using namespace boost::python;

    class_<AsteroidWrap, bases<SystemObject>, boost::shared_ptr<AsteroidWrap>, boost::noncopyable> c("Asteroid", init<uint64_t>());
    c.def(init<>())
    .add_property("semiAxis", &AsteroidWrap::semiAxis, &AsteroidWrap::setSemiAxis)
    .add_property("period", &Asteroid::period, &Asteroid::setPeriod)
    .add_property("angle", &Asteroid::angle, &Asteroid::setAngle)
    .add_property("time", &Asteroid::time, &Asteroid::setTime)
    .add_property("mineral", &Asteroid::mineral, &Asteroid::setMineral)
    .add_property("speed", &Asteroid::speed)
    .add_property("style", &Asteroid::style, &AsteroidWrap::setStyle);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Asteroid, AsteroidWrap, c);
    register_ptr_to_python<boost::shared_ptr<Asteroid> >();
}
}
}
}