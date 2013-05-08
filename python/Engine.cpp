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

namespace Rangers
{
namespace Python
{
struct NullDeleter
{
    void operator()(const void*) {}
};
boost::shared_ptr<Engine> getEngineInstance()
{
    return boost::shared_ptr<Engine>(&Engine::instance(), NullDeleter());
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
    .def("addWidget", &Engine::addWidget, with_custodian_and_ward<1, 2>())
    .def("removeWidget", &Engine::removeWidget)
    .def("markWidgetDeleting", &Engine::markWidgetDeleting)
    .def("coreFont", &Engine::coreFont)
    .def("serviceFont", &Engine::serviceFont)
    .def("getTicks", &Engine::getTicks)
    .def("properties", &Engine::properties)
    .def("rootNode", &Engine::rootNode, boost::python::return_internal_reference<>())
    .def("defaultSkin", &Engine::defaultSkin)
    .def("setDefaultSkin", (void (Engine::*)(const std::wstring&))&Engine::setDefaultSkin)
    .def("setDefaultSkin", (void (Engine::*)(const Skin&))&Engine::setDefaultSkin)
    .def("loadPlugin", &Engine::loadPlugin);
}
}
}
