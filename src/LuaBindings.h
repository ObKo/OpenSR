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

#ifndef RANGERS_LUABINDINGS_H
#define RANGERS_LUABINDINGS_H

#include <boost/shared_ptr.hpp>
#include <Font.h>
#include <Texture.h>
#include <AnimatedTexture.h>
#include <tolua++.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

int tolua_libRanger_open(lua_State* tolua_S);
int tolua_Engine_open(lua_State* tolua_S);
int tolua_Object_open(lua_State* tolua_S);
int tolua_Types_open(lua_State* tolua_S);
int tolua_ResourceManager_open(lua_State* tolua_S);
int tolua_Sprite_open(lua_State* tolua_S);
int tolua_AnimatedSprite_open(lua_State* tolua_S);
int tolua_LuaBindings_open(lua_State* tolua_S);
int tolua_Texture_open(lua_State* tolua_S);
int tolua_AnimatedTexture_open(lua_State* tolua_S);
int tolua_LuaWidget_open(lua_State* tolua_S);

std::wstring fromLua(const char *s);

namespace Rangers
{
Font *getPointer(boost::shared_ptr<Font> sp);
Texture *getPointer(boost::shared_ptr<Texture> sp);
AnimatedTexture *getPointer(boost::shared_ptr<AnimatedTexture> sp);

void luaDebug(std::wstring s);
}

#endif