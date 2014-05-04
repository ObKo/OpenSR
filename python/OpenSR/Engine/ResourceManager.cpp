/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/AnimatedTexture.h>
#include <OpenSR/Font.h>
#include <OpenSR/libRanger.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addDATFile, ResourceManager::addDATFile, 1, 2)
void exportResourceManager()
{
    using namespace boost::python;
    class_<ResourceManager, boost::noncopyable>("ResourceManager", no_init)
    .def("addRPKG", &ResourceManager::addRPKG)
    .def("addDir", &ResourceManager::addDir)
    .def("loadTexture", &ResourceManager::loadTexture)
    .def("loadAnimation", &ResourceManager::loadAnimation)
    .def("loadFont", (boost::shared_ptr<Font> (ResourceManager::*)(const FontDescriptor&))&ResourceManager::loadFont)
    .def("loadFont", (boost::shared_ptr<Font> (ResourceManager::*)(const std::string&, int, bool))&ResourceManager::loadFont)
    .def("resourceExists", &ResourceManager::resourceExists)
    .def("objectManager", &ResourceManager::objectManager, return_internal_reference<>())
    .def("instance", &ResourceManager::instance, return_value_policy<reference_existing_object>())
    .def("addDATFile", &ResourceManager::addDATFile, addDATFile())
    .def("datRoot", &ResourceManager::datRoot)
    .def("datValue", &ResourceManager::datValue)
    .staticmethod("instance");
}
}
}
