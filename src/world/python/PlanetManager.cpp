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
#include "PlanetManager.h"

#include "Types.h"

#include <OpenSR/Texture.h>


namespace Rangers
{
namespace World
{
namespace Python
{
void exportPlanetManager()
{
    using namespace boost::python;

    class_<PlanetManager, boost::noncopyable> c("PlanetManager", boost::python::no_init);
    c.def("style", (boost::shared_ptr<PlanetStyle> (PlanetManager::*)(const std::string&))&PlanetManager::style)
    .def("style", (boost::shared_ptr<PlanetStyle> (PlanetManager::*)(uint32_t))&PlanetManager::style)
    .def("loadStyles", &PlanetManager::loadStyles)
    .def("getPlanetImage", (boost::shared_ptr<Texture> (PlanetManager::*)(boost::shared_ptr<PlanetStyle>, int))&PlanetManager::getPlanetImage)
    .def("getPlanetImage", (boost::shared_ptr<Texture> (PlanetManager::*)(uint32_t, int))&PlanetManager::getPlanetImage)
    .def("dropImageCache", &PlanetManager::dropImageCache);
}
}
}
}