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

#include <OpenSR/Action.h>
#include <OpenSR/Widget.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportAction()
{
    using namespace boost::python;
    enum_<Action::Type>("ActionType")
    .value("NONE", Action::NONE)
    .value("BUTTON_CLICKED", Action::BUTTON_CLICKED)
    .value("KEY_PRESSED", Action::KEY_PRESSED)
    .value("CHECKBOX_TOGGLED", Action::CHECKBOX_TOGGLED)
    .value("MOUSE_ENTER", Action::MOUSE_ENTER)
    .value("MOUSE_LEAVE", Action::MOUSE_LEAVE)
    .value("MOUSE_DOWN", Action::MOUSE_DOWN)
    .value("MOUSE_UP", Action::MOUSE_UP)
    .value("MOUSE_CLICK", Action::MOUSE_CLICK);

    class_<Action::Argument>("ActionArgument")
    .def(init<>())
    .def(init<const std::wstring&>())
    .def(init<const Rect&>())
    .def(init<const SDL_keysym&>())
    .def(init<bool>())
    .def(init<uint8_t>());

    class_<Action>("Action", init<>())
    .def(init<Widget*, Action::Type, const Action::Argument&>()[with_custodian_and_ward<1, 2>()])
    .def(init<Widget*, Action::Type>()[with_custodian_and_ward<1, 2>()])
    .def("source", &Action::source, return_internal_reference<>())
    .def("type", &Action::type)
    .def("argument", &Action::argument)
    .def("stringArgument", &Action::stringArgument)
    .def("rectArgument", &Action::rectArgument)
    .def("keyArgument", &Action::keyArgument)
    .def("boolArgument", &Action::boolArgument)
    .def("byteArgument", &Action::byteArgument);
}
}
}
