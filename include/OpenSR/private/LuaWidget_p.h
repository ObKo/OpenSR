/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LUA_WIDGET_P_H
#define RANGERS_LUA_WIDGET_P_H

#include "Widget_p.h"

struct lua_State;
namespace Rangers
{
class LuaWidget::LuaActionListener;
class LuaWidgetPrivate: public WidgetPrivate
{
    RANGERS_DECLARE_PUBLIC(LuaWidget)
public:
    LuaWidgetPrivate();

    boost::shared_ptr<lua_State> luaState;
    LuaWidget::LuaActionListener *actionListener;

    void initLuaState();
    static int luaErrorHandler(lua_State* state);
};
}

#endif