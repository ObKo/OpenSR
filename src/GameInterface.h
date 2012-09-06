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

#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#ifdef WIN32
#define RANGERS_GAME_API __declspec(dllexport)
#else
#define RANGERS_GAME_API
#endif

#include "Engine.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

    RANGERS_GAME_API int rangersGameInit();
    RANGERS_GAME_API int rangersAPIVersion();
    RANGERS_GAME_API void rangersGameSetupLua(lua_State *state);
    RANGERS_GAME_API void rangersGameDeinit();
}

#endif // GAMEINTERFACE_H
