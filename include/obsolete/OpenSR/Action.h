/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_ACTION_H
#define RANGERS_ACTION_H

#include "OpenSR/Types.h"

#include <boost/variant.hpp>
#include <SDL_keyboard.h>

namespace Rangers
{
class Widget;
class Action;

//! UI action
class RANGERS_ENGINE_API Action
{
public:
    //! Type of action
    enum Type {NONE = 0, BUTTON_CLICKED = 1, KEY_PRESSED = 2, CHECKBOX_TOGGLED = 3, MOUSE_ENTER = 4,
               MOUSE_LEAVE = 5, MOUSE_DOWN = 6, MOUSE_UP = 7, MOUSE_CLICK = 8, TEXT_INPUT = 9,
               DIALOG_ACCEPT = 10, DIALOG_CANCEL = 11, RADIOBUTTON_SWITCHED = 12,
               USER = 127
              };
    //! Argument of action
    typedef boost::variant<std::string, Rect, SDL_Keysym, bool, uint8_t> Argument;

    //! Constructs empty action
    Action();
    //! Constructs new action
    Action(boost::shared_ptr<Widget> source, uint32_t type, const Argument& argument = Argument());

    //! Source of action
    boost::shared_ptr<Widget> source() const;
    //! Type of action
    uint32_t type() const;
    //! Argument of action
    Argument argument() const;

private:
    boost::shared_ptr<Widget> m_source;
    uint32_t m_type;
    Argument m_argument;
};
}

#endif
