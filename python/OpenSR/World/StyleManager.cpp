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

#include <OpenSR/World/StyleManager.h>
#include <OpenSR/World/Types.h>

namespace Rangers
{
namespace World
{
namespace Python
{
void exportStyleManager()
{
    using namespace boost::python;
    class_<StyleManager, boost::noncopyable> c("StyleManager", boost::python::no_init);
    c.def("planetStyle", (boost::shared_ptr<PlanetStyle> (StyleManager::*)(const std::string&))&StyleManager::planetStyle)
    .def("planetStyle", (boost::shared_ptr<PlanetStyle> (StyleManager::*)(uint32_t))&StyleManager::planetStyle)
    .def("systemStyle", (boost::shared_ptr<SystemStyle> (StyleManager::*)(const std::string&))&StyleManager::systemStyle)
    .def("systemStyle", (boost::shared_ptr<SystemStyle> (StyleManager::*)(uint32_t))&StyleManager::systemStyle)
    .def("asteroidStyle", (boost::shared_ptr<AsteroidStyle> (StyleManager::*)(const std::string&))&StyleManager::asteroidStyle)
    .def("asteroidStyle", (boost::shared_ptr<AsteroidStyle> (StyleManager::*)(uint32_t))&StyleManager::asteroidStyle)
    .def("loadPlanetStyles", &StyleManager::loadPlanetStyles)
    .def("loadSystemStyles", &StyleManager::loadSystemStyles)
    .def("loadAsteroidStyles", &StyleManager::loadAsteroidStyles);
}
}
}
}