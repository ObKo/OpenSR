/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#include <boost/variant.hpp>
#include "Types.h"

namespace Rangers
{
class Widget;
class Action;

class Action
{
public:
    enum Type {BUTTON_CLICKED};
    typedef boost::variant<std::wstring, Rect> Argument;

    Action(Widget *source, Type type, const Argument& argument);

    Widget* source() const;
    Type type() const;
    Argument argument() const;

private:
    Widget *m_source;
    Type m_type;
    Argument m_argument;
};
}

#endif