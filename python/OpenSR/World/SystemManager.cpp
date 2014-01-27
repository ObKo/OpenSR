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
#include <OpenSR/World/SystemManager.h>

#include <OpenSR/World/Types.h>
#include <OpenSR/World/PlanetarySystem.h>

namespace Rangers
{
namespace World
{
namespace Python
{
boost::python::object systems(SystemManager &self)
{
    return Rangers::Python::listToPythonList(self.systems());
}

void exportSystemManager()
{
    using namespace boost::python;

    class_<SystemManager, boost::noncopyable> c("SystemManager", boost::python::no_init);
    c.add_property("systems", &systems)
    .add_property("currentSystem", &SystemManager::currentSystem, &SystemManager::setCurrentSystem)
    .def("addSystem", &SystemManager::addSystem);
}
}
}
}