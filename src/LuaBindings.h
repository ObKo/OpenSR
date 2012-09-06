/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LUABINDINGS_H
#define RANGERS_LUABINDINGS_H

#include "config.h"
#include <boost/shared_ptr.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace Rangers
{
class AnimatedTexture;
class GAISprite;
class Texture;
class Font;
RANGERS_ENGINE_API int execLuaScript(const std::wstring& fileName);
RANGERS_ENGINE_API int execLuaScript(const char *data, size_t size, const std::string& name);
RANGERS_ENGINE_API lua_State *initLuaState();
RANGERS_ENGINE_API std::wstring fromLua(const char *s);

RANGERS_ENGINE_API void luaDebug(std::wstring s);
RANGERS_ENGINE_API void luaWarning(std::wstring s);
RANGERS_ENGINE_API void luaError(std::wstring s);
}

#endif
