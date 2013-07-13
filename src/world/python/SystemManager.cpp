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
#include "SystemManager.h"
#include "PlanetarySystem.h"

#include "Types.h"


namespace Rangers
{
namespace World
{
namespace Python
{
void exportSystemManager()
{
    using namespace boost::python;

    class_<SystemManager, boost::noncopyable> c("SystemManager", boost::python::no_init);
    c.def("style", (boost::shared_ptr<SystemStyle> (SystemManager::*)(const std::string&))&SystemManager::style)
    .def("style", (boost::shared_ptr<SystemStyle> (SystemManager::*)(uint32_t))&SystemManager::style)
    .def("loadStyles", &SystemManager::loadStyles)
    .def("systems", &SystemManager::systems)
    .def("currentSystem", &SystemManager::currentSystem)
    .def("setCurrentSystem", &SystemManager::setCurrentSystem)
    .def("addSystem", &SystemManager::addSystem);
}
}
}
}