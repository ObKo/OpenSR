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

#include <OpenSR/Engine.h>
#include <OpenSR/Object.h>
#include <OpenSR/Widget.h>
#include <OpenSR/Node.h>
#include <OpenSR/Font.h>

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include "Wrappers.h"

namespace Rangers
{
namespace Python
{
int propertiesGetInt(Engine &self, const std::string& key, int defaultValue)
{
    return self.properties()->get<int>(key, defaultValue);
}
std::string propertiesGetString(Engine &self, const std::string& key, const std::string& defaultValue)
{
    return self.properties()->get<std::string>(key, defaultValue);
}
bool propertiesGetBool(Engine &self, const std::string& key, bool defaultValue)
{
    return self.properties()->get<bool>(key, defaultValue);
}
void propertiesSetInt(Engine &self, const std::string& key, int value)
{
    self.properties()->put(key, value);
}
void propertiesSetString(Engine &self, const std::string& key, const std::string& value)
{
    self.properties()->put(key, value);
}
void propertiesSetBool(Engine &self, const std::string& key, bool value)
{
    self.properties()->put(key, value);
}

void exportEngine()
{
    using namespace boost::python;
    class_<Engine, boost::noncopyable>("Engine", boost::python::no_init)
//        .def("init", &Engine::init)
    .def("quit", &Engine::quit)
//        .def("run", &Engine::run)
    .def("instance", &Engine::instance, return_value_policy<reference_existing_object>())
    .staticmethod("instance")
    .def("markToUpdate", &Engine::markToUpdate)
    .def("unmarkToUpdate", &Engine::unmarkToUpdate)
    .def("addWidget", &Engine::addWidget)
    .def("removeWidget", &Engine::removeWidget)
    .def("coreFont", &Engine::coreFont)
    .def("serviceFont", &Engine::serviceFont)
    .def("getTicks", &Engine::getTicks)
    .def("properties", &Engine::properties)
    .def("rootNode", &Engine::rootNode, boost::python::return_internal_reference<>())
    .def("defaultSkin", &Engine::defaultSkin)
    .def("setDefaultSkin", (void (Engine::*)(const std::wstring&))&Engine::setDefaultSkin)
    .def("setDefaultSkin", (void (Engine::*)(const Skin&))&Engine::setDefaultSkin)
    .def("loadPlugin", &Engine::loadPlugin)
    .def("screenWidth", &Engine::screenWidth)
    .def("screenHeight", &Engine::screenHeight)
    .def("propertiesGetInt", &propertiesGetInt)
    .def("propertiesGetBool", &propertiesGetBool)
    .def("propertiesGetString", &propertiesGetString)
    .def("propertiesSetInt", &propertiesSetInt)
    .def("propertiesSetBool", &propertiesSetBool)
    .def("propertiesSetString", &propertiesSetString);
}
}
}
