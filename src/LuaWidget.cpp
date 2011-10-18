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

class LuaDeleter
{
public:
    void operator()(lua_State*& ptr)
    {
        if (ptr)
        {
            lua_close(ptr);
        }
    }
};

LuaWidget::LuaWidget(const std::wstring& fileName, Rangers::Widget* parent): Widget(parent)
{
    m_luaState = boost::shared_ptr<lua_State>(lua_open(), LuaDeleter());
    luaopen_base(m_luaState.get());
    luaopen_table(m_luaState.get());
    luaopen_math(m_luaState.get());
    tolua_Engine_open(m_luaState.get());
    tolua_libRanger_open(m_luaState.get());
    tolua_Object_open(m_luaState.get());
    tolua_Types_open(m_luaState.get());
    tolua_ResourceManager_open(m_luaState.get());
    tolua_Sprite_open(m_luaState.get());
    tolua_AnimatedSprite_open(m_luaState.get());
    tolua_LuaBindings_open(m_luaState.get());
    tolua_AnimatedTexture_open(m_luaState.get());
    tolua_LuaWidget_open(m_luaState.get());
    tolua_GAISprite_open(m_luaState.get());
    tolua_Button_open(m_luaState.get());
    tolua_pushusertype(m_luaState.get(), this, "Rangers::LuaWidget");
    lua_setglobal(m_luaState.get(), "this");
    if (luaL_dofile(m_luaState.get(), toLocal(fileName).c_str()))
        Log::error() << "Cannot load lua script: " << lua_tostring(m_luaState.get(), -1);
}

LuaWidget::LuaWidget(const char *data, size_t size, const std::string& name, Widget *parent): Widget(parent)
{
    m_luaState = boost::shared_ptr<lua_State>(lua_open(), LuaDeleter());
    luaopen_base(m_luaState.get());
    luaopen_table(m_luaState.get());
    luaopen_math(m_luaState.get());
    tolua_Engine_open(m_luaState.get());
    tolua_libRanger_open(m_luaState.get());
    tolua_Object_open(m_luaState.get());
    tolua_Types_open(m_luaState.get());
    tolua_ResourceManager_open(m_luaState.get());
    tolua_Sprite_open(m_luaState.get());
    tolua_AnimatedSprite_open(m_luaState.get());
    tolua_LuaBindings_open(m_luaState.get());
    tolua_AnimatedTexture_open(m_luaState.get());
    tolua_LuaWidget_open(m_luaState.get());
    tolua_GAISprite_open(m_luaState.get());
    tolua_Button_open(m_luaState.get());
    tolua_pushusertype(m_luaState.get(), this, "Rangers::LuaWidget");
    lua_setglobal(m_luaState.get(), "this");
    if (luaL_loadbuffer(m_luaState.get(), data, size, name.c_str()) || lua_pcall(m_luaState.get(), 0, LUA_MULTRET, 0))
        Log::error() << "Cannot load lua script: " << lua_tostring(m_luaState.get(), -1);
}

LuaWidget::LuaWidget(Rangers::Widget* parent): Widget(parent), m_luaState(boost::shared_ptr<lua_State>((lua_State*)0, LuaDeleter()))
{
}

LuaWidget::LuaWidget(const LuaWidget& other): Widget(other)
{
    m_luaState = other.m_luaState;
}

LuaWidget& LuaWidget::operator=(const LuaWidget& other)
{
    Widget::operator=(other);
    m_luaState = other.m_luaState;

    return *this;
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
    lua_getglobal(m_luaState.get(), "getBoundingRect");

    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return Widget::getBoundingRect();
    }

    Rect bb;
    lua_pcall(m_luaState.get(), 0, 1, 0);
    if (Rect *bp = ((Rect*)(tolua_tousertype(m_luaState.get(), -1, 0))))
        bb = *bp;
    lua_pop(m_luaState.get(), 1);
    unlock();
    return bb + Widget::getBoundingRect();
}

void LuaWidget::keyPressed(SDL_keysym key)
{
    lock();
    lua_getglobal(m_luaState.get(), "keyPressed");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return;
    }

    tolua_pushusertype(m_luaState.get(), &key, "SDL_keysym");
    if (lua_pcall(m_luaState.get(), 1, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState.get(), -1);
    unlock();
}

void LuaWidget::mouseEnter()
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseEnter");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return;
    }
    if (lua_pcall(m_luaState.get(), 0, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState.get(), -1);
    unlock();
    Widget::mouseEnter();
}

void LuaWidget::mouseLeave()
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseLeave");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return;
    }
    if (lua_pcall(m_luaState.get(), 0, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState.get(), -1);
    unlock();
    Widget::mouseLeave();
}

void LuaWidget::mouseMove(int x, int y)
{
    Widget::mouseMove(x, y);
    lock();
    lua_getglobal(m_luaState.get(), "mouseMove");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    lua_pcall(m_luaState.get(), 2, 0, 0);
    unlock();
    Widget::mouseMove(x, y);
}

void LuaWidget::mouseDown(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseDown");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        Widget::mouseDown(key, x, y);
        return;
    }
    lua_pushinteger(m_luaState.get(), key);
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    if (lua_pcall(m_luaState.get(), 3, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState.get(), -1);
    unlock();
    Widget::mouseDown(key, x, y);
}

void LuaWidget::mouseUp(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseUp");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        Widget::mouseUp(key, x, y);
        return;
    }
    lua_pushinteger(m_luaState.get(), key);
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    if (lua_pcall(m_luaState.get(), 3, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState.get(), -1);
    unlock();
    Widget::mouseUp(key, x, y);
}

void LuaWidget::mouseClick(int x, int y)
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseClick");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    if (lua_pcall(m_luaState.get(), 2, 0, 0))
        Log::warning() << "Lua: " << lua_tostring(m_luaState.get(), -1);
    unlock();
    Widget::mouseClick(x, y);
}

void LuaWidget::processLogic(int dt)
{
    lock();
    for (std::list<Object*>::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->processLogic(dt);
    lua_getglobal(m_luaState.get(), "processLogic");
    if (lua_isnil(m_luaState.get(), -1))
    {
        unlock();
        return;
    }
    lua_pushinteger(m_luaState.get(), dt);
    lua_pcall(m_luaState.get(), 1, 0, 0);
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
