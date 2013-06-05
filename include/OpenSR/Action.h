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
    enum Type {NONE, BUTTON_CLICKED, KEY_PRESSED, CHECKBOX_TOGGLED, MOUSE_ENTER, MOUSE_LEAVE, MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, TEXT_INPUT};
    //! Argument of action
    typedef boost::variant<std::wstring, Rect, SDL_Keysym, bool, uint8_t> Argument;

    //! Constructs empty action
    Action();
    //! Constructs new action
    Action(boost::shared_ptr<Widget> source, Type type, const Argument& argument = Argument());

    //! Source of action
    boost::shared_ptr<Widget> source() const;
    //! Type of action
    Type type() const;
    //! Argument of action
    Argument argument() const;

    //! Get argument as string
    std::wstring stringArgument() const;
    //! Get argument as Rect
    Rect rectArgument() const;
    //! Get argument as keysym
    SDL_Keysym keyArgument() const;
    //! Get argument as bool
    bool boolArgument() const;
    //! Get argument as 8-bit unsigned integer
    uint8_t byteArgument() const;

private:
    boost::shared_ptr<Widget> m_source;
    Type m_type;
    Argument m_argument;
};
}

#endif
