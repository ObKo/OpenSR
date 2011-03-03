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

using namespace Rangers;

LuaWidget::LuaWidget(std::wstring fileName, Rangers::Object* parent): Widget(parent)
{
    luaState = lua_open();
    luaopen_base(luaState);
    luaopen_table(luaState);
    tolua_Engine_open(luaState);
    tolua_libRanger_open(luaState);
    tolua_Object_open(luaState);
    tolua_Types_open(luaState);
    tolua_ResourceManager_open(luaState);
    tolua_Sprite_open(luaState);
    tolua_AnimatedSprite_open(luaState);
    tolua_LuaBindings_open(luaState);
    tolua_AnimatedTexture_open(luaState);
    if(luaL_dofile(luaState, toLocal(fileName).c_str()))
	std::cerr << lua_tostring(luaState, -1) << std::endl;
}

LuaWidget::LuaWidget(Rangers::Object* parent): Widget(parent), luaState()
{

}

void LuaWidget::draw() const
{
    lua_getglobal(luaState, "draw");
    lua_pcall(luaState, 0, 0, 0);
}

Rect LuaWidget::getBoundingRect() const
{
    lua_getglobal(luaState, "getBoundingRect");
    lua_pcall(luaState, 0, 1, 0);
    return *((Rect*)(lua_topointer(luaState, 0)));
}

void LuaWidget::keyPressed(SDL_keysym key)
{
    lua_getglobal(luaState, "keyPressed");
    lua_pushlightuserdata(luaState, &key);
    lua_pcall(luaState, 1, 0, 0);
}
 
void LuaWidget::mouseEnter()
{
    lua_getglobal(luaState, "mouseEnter");
    lua_pcall(luaState, 0, 0, 0);
}

void LuaWidget::mouseLeave()
{
    lua_getglobal(luaState, "mouseLeave");
    lua_pcall (luaState, 0, 0, 0);
}

void LuaWidget::mouseMove(int x, int y)
{
    lua_getglobal(luaState, "mouseMove");
    lua_pushinteger(luaState, x);
    lua_pushinteger(luaState, y);
    lua_pcall (luaState, 2, 0, 0);
}
