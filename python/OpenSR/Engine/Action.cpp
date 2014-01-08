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

namespace
{
std::wstring argumentGetString(Action::Argument &self)
{
    return boost::get<std::wstring>(self);
}

Rect argumentGetRect(Action::Argument &self)
{
    return boost::get<Rect>(self);
}

SDL_Keysym argumentGetKey(Action::Argument &self)
{
    return boost::get<SDL_Keysym>(self);
}

bool argumentGetBool(Action::Argument &self)
{
    return boost::get<bool>(self);
}

uint8_t argumentGetUint(Action::Argument &self)
{
    return boost::get<uint8_t>(self);
}
}

void exportAction()
{
    using namespace boost::python;

    class_<Action::Argument>("ActionArgument")
    .def(init<>())
    .def(init<const std::wstring&>())
    .def(init<const Rect&>())
    .def(init<const SDL_Keysym&>())
    .def(init<bool>())
    .def(init<uint8_t>())
    .add_property("string", &argumentGetString)
    .add_property("rect", &argumentGetRect)
    .add_property("key", &argumentGetKey)
    .add_property("bool", &argumentGetBool)
    .add_property("uint", &argumentGetUint);

    {
        scope s = class_<Action>("Action", init<>())
                  .def(init<boost::shared_ptr<Widget>, uint32_t, const Action::Argument&>()[with_custodian_and_ward<1, 2>()])
                  .def(init<boost::shared_ptr<Widget>, uint32_t>()[with_custodian_and_ward<1, 2>()])
                  .add_property("source", &Action::source)
                  .add_property("type", &Action::type)
                  .add_property("argument", &Action::argument);

        enum_<Action::Type>("Type")
        .value("NONE", Action::NONE)
        .value("BUTTON_CLICKED", Action::BUTTON_CLICKED)
        .value("KEY_PRESSED", Action::KEY_PRESSED)
        .value("CHECKBOX_TOGGLED", Action::CHECKBOX_TOGGLED)
        .value("MOUSE_ENTER", Action::MOUSE_ENTER)
        .value("MOUSE_LEAVE", Action::MOUSE_LEAVE)
        .value("MOUSE_DOWN", Action::MOUSE_DOWN)
        .value("MOUSE_UP", Action::MOUSE_UP)
        .value("MOUSE_CLICK", Action::MOUSE_CLICK)
        .value("USER", Action::USER);
    }
}
}
}
