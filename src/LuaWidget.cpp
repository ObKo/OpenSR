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

#include "LuaWidget.h"
#include "Log.h"
#include "Types.h"

using namespace Rangers;

//TODO: Check stack size after pcall()

LuaWidget::LuaWidget(std::wstring fileName, Rangers::Object* parent): Widget(parent)
{
    luaState = lua_open();
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
    tolua_pushusertype(luaState, this, "Rangers::LuaWidget");
    lua_setglobal(luaState, "this");
    if(luaL_dofile(luaState, toLocal(fileName).c_str()))
    	std::cerr << lua_tostring(luaState, -1) << std::endl;
}

LuaWidget::LuaWidget(Rangers::Object* parent): Widget(parent), luaState()
{

}

void LuaWidget::draw()
{
    if(!prepareDraw())
        return;
    lua_getglobal(luaState, "draw");
    if(!lua_isnil(luaState, -1))
    	lua_pcall(luaState, 0, 0, 0);
    endDraw();
}

Rect LuaWidget::getBoundingRect()
{
    lock();
    lua_getglobal(luaState, "getBoundingRect");

    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return Rect();
    }

    lua_pcall(luaState, 0, 1, 0);
    Rect bb = *((Rect*)(tolua_tousertype(luaState, -1, NULL)));
    lua_pop(luaState, 1);
    unlock();
    return bb;
}

void LuaWidget::keyPressed(SDL_keysym key)
{
    lock();
    lua_getglobal(luaState, "keyPressed");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }

    tolua_pushusertype(luaState, &key, "SDL_keysym");
    if(lua_pcall(luaState, 1, 0, 0))
        logger() << LWARNING << "Lua: " << lua_tostring(luaState, -1) << LEND;
    unlock();
}
 
void LuaWidget::mouseEnter()
{
    lock();
    lua_getglobal(luaState, "mouseEnter");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    if(lua_pcall(luaState, 0, 0, 0))
    	logger() << LWARNING << "Lua: " << lua_tostring(luaState, -1) << LEND;
    unlock();
}

void LuaWidget::mouseLeave()
{
    lock();
    lua_getglobal(luaState, "mouseLeave");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    if(lua_pcall (luaState, 0, 0, 0))
    	logger() << LWARNING << "Lua: " << lua_tostring(luaState, -1) << LEND;
    unlock();
}

void LuaWidget::mouseMove(int x, int y)
{
	Widget::mouseMove(x, y);
    lock();
    lua_getglobal(luaState, "mouseMove");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    lua_pushinteger(luaState, x);
    lua_pushinteger(luaState, y);
    lua_pcall(luaState, 2, 0, 0);
    unlock();
}

void LuaWidget::mouseDown(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(luaState, "mouseDown");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    lua_pushinteger(luaState, key);
    lua_pushinteger(luaState, x);
    lua_pushinteger(luaState, y);
    if(lua_pcall(luaState, 3, 0, 0))
    	logger() << LWARNING << "Lua: " << lua_tostring(luaState, -1) << LEND;
    unlock();
    Widget::mouseDown(key, x, y);
}

void LuaWidget::mouseUp(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(luaState, "mouseUp");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    lua_pushinteger(luaState, key);
    lua_pushinteger(luaState, x);
    lua_pushinteger(luaState, y);
   	if(lua_pcall(luaState, 3, 0, 0))
   		logger() << LWARNING << "Lua: " << lua_tostring(luaState, -1) << LEND;
    unlock();
    Widget::mouseUp(key, x, y);
}

void LuaWidget::mouseClick(int x, int y)
{
    lock();
    lua_getglobal(luaState, "mouseClick");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    lua_pushinteger(luaState, x);
    lua_pushinteger(luaState, y);
    if(lua_pcall(luaState, 2, 0, 0))
    	logger() << LWARNING << "Lua: " << lua_tostring(luaState, -1) << LEND;
    unlock();
    Widget::mouseClick(x, y);
}

void LuaWidget::processLogic(int dt)
{
    lock();
    lua_getglobal(luaState, "processLogic");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
    lua_pushinteger(luaState, dt);
   	lua_pcall(luaState, 1, 0, 0);
    unlock();
}

void LuaWidget::processMain()
{
    lock();
    lua_getglobal(luaState, "processMain");
    if(lua_isnil(luaState, -1))
    {
    	unlock();
    	return;
    }
   	lua_pcall(luaState, 0, 0, 0);
    unlock();
}

