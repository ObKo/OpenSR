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
#include "Object.h"

namespace Rangers
{
//TODO: Check stack size after pcall()

LuaWidget::LuaWidget(std::wstring fileName, Rangers::Object* parent): Widget(parent)
{
    m_luaState = lua_open();
    luaopen_base(m_luaState);
    luaopen_table(m_luaState);
    luaopen_math(m_luaState);
    tolua_Engine_open(m_luaState);
    tolua_libRanger_open(m_luaState);
    tolua_Object_open(m_luaState);
    tolua_Types_open(m_luaState);
    tolua_ResourceManager_open(m_luaState);
    tolua_Sprite_open(m_luaState);
    tolua_AnimatedSprite_open(m_luaState);
    tolua_LuaBindings_open(m_luaState);
    tolua_AnimatedTexture_open(m_luaState);
    tolua_LuaWidget_open(m_luaState);
    tolua_GAISprite_open(m_luaState);
    tolua_pushusertype(m_luaState, this, "Rangers::LuaWidget");
    lua_setglobal(m_luaState, "this");
    if (luaL_dofile(m_luaState, toLocal(fileName).c_str()))
        std::cerr << lua_tostring(m_luaState, -1) << std::endl;
}

LuaWidget::LuaWidget(Rangers::Object* parent): Widget(parent), m_luaState()
{

}

void LuaWidget::draw()
{
    if (!prepareDraw())
        return;

    for (std::list<Object*>::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->draw();

    endDraw();
}

Rect LuaWidget::getBoundingRect()
{
    lock();
    lua_getglobal(m_luaState, "getBoundingRect");

    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return Rect();
    }

    lua_pcall(m_luaState, 0, 1, 0);
    Rect bb = *((Rect*)(tolua_tousertype(m_luaState, -1, NULL)));
    lua_pop(m_luaState, 1);
    unlock();
    return bb;
}

void LuaWidget::keyPressed(SDL_keysym key)
{
    lock();
    lua_getglobal(m_luaState, "keyPressed");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }

    tolua_pushusertype(m_luaState, &key, "SDL_keysym");
    if (lua_pcall(m_luaState, 1, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState, -1);
    unlock();
}

void LuaWidget::mouseEnter()
{
    lock();
    lua_getglobal(m_luaState, "mouseEnter");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    if (lua_pcall(m_luaState, 0, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState, -1);
    unlock();
}

void LuaWidget::mouseLeave()
{
    lock();
    lua_getglobal(m_luaState, "mouseLeave");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    if (lua_pcall(m_luaState, 0, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState, -1);
    unlock();
}

void LuaWidget::mouseMove(int x, int y)
{
    Widget::mouseMove(x, y);
    lock();
    lua_getglobal(m_luaState, "mouseMove");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState, x);
    lua_pushinteger(m_luaState, y);
    lua_pcall(m_luaState, 2, 0, 0);
    unlock();
}

void LuaWidget::mouseDown(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(m_luaState, "mouseDown");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState, key);
    lua_pushinteger(m_luaState, x);
    lua_pushinteger(m_luaState, y);
    if (lua_pcall(m_luaState, 3, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState, -1);
    unlock();
    Widget::mouseDown(key, x, y);
}

void LuaWidget::mouseUp(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(m_luaState, "mouseUp");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState, key);
    lua_pushinteger(m_luaState, x);
    lua_pushinteger(m_luaState, y);
    if (lua_pcall(m_luaState, 3, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState, -1);
    unlock();
    Widget::mouseUp(key, x, y);
}

void LuaWidget::mouseClick(int x, int y)
{
    lock();
    lua_getglobal(m_luaState, "mouseClick");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState, x);
    lua_pushinteger(m_luaState, y);
    if (lua_pcall(m_luaState, 2, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState, -1);
    unlock();
    Widget::mouseClick(x, y);
}

void LuaWidget::processLogic(int dt)
{
    lock();
    for (std::list<Object*>::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->processLogic(dt);
    lua_getglobal(m_luaState, "processLogic");
    if (lua_isnil(m_luaState, -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState, dt);
    lua_pcall(m_luaState, 1, 0, 0);
    unlock();
}

void LuaWidget::processMain()
{
    lock();
    for (std::list<Object*>::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->processMain();
    unlock();
}
}
