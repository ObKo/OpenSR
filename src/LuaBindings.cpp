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

#include "LuaBindings.h"
#include "libRanger.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Log.h"
#include <Font.h>
#include <Texture.h>
#include <AnimatedTexture.h>
#include "GAISprite.h"
#include <tolua++.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cerrno>

namespace Rangers
{
int execLuaScript(const std::wstring& name)
{
    size_t size;
    char *luaData = ResourceManager::instance()->loadData(name, size);
    if (!luaData)
        return -1;
    int state = execLuaScript(luaData, size, toLocal(name));
    delete[] luaData;
    return state;
}

int execLuaScript(const char *data, size_t size, const std::string& name)
{
    lua_State *luaState = lua_open();
    luaopen_base(luaState);
    luaopen_table(luaState);
    luaopen_math(luaState);
    tolua_Engine_open(luaState);
    tolua_libRanger_open(luaState);
    tolua_Object_open(luaState);
    tolua_Types_open(luaState);
    tolua_ResourceManager_open(luaState);
    tolua_Sprite_open(luaState);
    tolua_AnimatedSprite_open(luaState);
    tolua_LuaBindings_open(luaState);
    tolua_AnimatedTexture_open(luaState);
    tolua_LuaWidget_open(luaState);
    tolua_GAISprite_open(luaState);
    tolua_Button_open(luaState);
    tolua_Node_open(luaState);
    tolua_WidgetNode_open(luaState);
    tolua_CheckBox_open(luaState);
    tolua_NinePatch_open(luaState);
    tolua_ScrollArea_open(luaState);
    tolua_pushusertype(luaState, Engine::instance(), "Rangers::Engine");
    lua_setglobal(luaState, "engine");
    tolua_pushusertype(luaState, ResourceManager::instance(), "Rangers::ResourceManager");
    lua_setglobal(luaState, "resources");
    int state;
    if (state = (luaL_loadbuffer(luaState, data, size, name.c_str()) || lua_pcall(luaState, 0, LUA_MULTRET, 0)))
        Log::error() << "Cannot exec lua script: " << lua_tostring(luaState, -1);
    lua_close(luaState);
    return state;
}

std::wstring fromLua(const char *s)
{
    return Rangers::fromUTF8(s);
}

Font* getPointer(boost::shared_ptr<Font> sp)
{
    return sp.get();
}

Texture* getPointer(boost::shared_ptr<Texture> sp)
{
    return sp.get();
}

AnimatedTexture* getPointer(boost::shared_ptr<AnimatedTexture> sp)
{
    return sp.get();
}

GAISprite *getPointer(boost::shared_ptr<GAISprite> sp)
{
    return sp.get();
}

void luaDebug(std::wstring s)
{
    Log::debug() << L"Lua: " << s;
}

void luaWarning(std::wstring s)
{
    Log::warning() << L"Lua: " << s;
}

void luaError(std::wstring s)
{
    Log::error() << L"Lua: " << s;
}

void freePointer(boost::shared_ptr<Font> *sp)
{
    delete sp;
}

void freePointer(boost::shared_ptr<Texture> *sp)
{
    delete sp;
}

void freePointer(boost::shared_ptr<AnimatedTexture> *sp)
{
    delete sp;
}
}
