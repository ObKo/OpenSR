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
#include "ActionListener.h"
#include "Action.h"
#include "ResourceManager.h"
#include "LuaBindings.h"
#include "Engine.h"
#include <libRanger.h>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/luabind.hpp>

namespace Rangers
{
void luaDeleter(lua_State*& ptr)
{
    if (ptr)
    {
        lua_close(ptr);
        ptr = 0;
    }
}

//TODO: Lua errors handling
LuaWidget::LuaActionListener::LuaActionListener(LuaWidget *widget): m_widget(widget)
{

}

void LuaWidget::LuaActionListener::actionPerformed(const Action &action)
{
    m_widget->lock();
    luabind::object actionPerformedFunc = luabind::globals(m_widget->m_luaState.get())["actionPerformed"];
    if (!actionPerformedFunc.is_valid())
    {
        m_widget->unlock();
        return;
    }
    try
    {
        luabind::call_function<void>(actionPerformedFunc, action);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_widget->m_luaState.get());
    }

    m_widget->unlock();
}

LuaWidget* LuaWidget::LuaActionListener::associatedWidget() const
{
    return m_widget;
}

void LuaWidget::LuaActionListener::setAssociatedWidget(LuaWidget *widget)
{
    m_widget = widget;
}

LuaWidget::LuaWidget(const char *data, size_t size, const std::string& name, Widget *parent): Widget(parent)
{
    initLuaState();
    if (luaL_loadbuffer(m_luaState.get(), data, size, name.c_str()) || lua_pcall(m_luaState.get(), 0, LUA_MULTRET, 0))
    {
        Log::error() << "[LuaWidget] " << lua_tostring(m_luaState.get(), -1);
        Engine::instance()->markWidgetDeleting(this);
    }
}

LuaWidget::LuaWidget(const std::wstring& name, Widget *parent)
{
    initLuaState();
    size_t size;
    char *luaData = ResourceManager::instance()->loadData(name, size);
    if (luaData)
    {
        if (luaL_loadbuffer(m_luaState.get(), luaData, size, toLocal(name.c_str()).c_str()) || lua_pcall(m_luaState.get(), 0, LUA_MULTRET, 0))
        {
            Log::error() << "[LuaWidget] " << lua_tostring(m_luaState.get(), -1);
            Engine::instance()->markWidgetDeleting(this);
        }
        delete[] luaData;
    }
}

LuaWidget::LuaWidget(Rangers::Widget* parent): Widget(parent), m_luaState(boost::shared_ptr<lua_State>((lua_State*)0, luaDeleter))
{
    m_actionListener = new LuaActionListener(this);
}

LuaWidget::~LuaWidget()
{
    delete m_actionListener;
}

LuaWidget::LuaWidget(const LuaWidget& other): Widget(other)
{
    m_luaState = other.m_luaState;
    m_actionListener = other.m_actionListener;
    if (m_luaState)
    {
        luabind::globals(m_luaState.get())["this"] = this;
    }
    //FIXME: Do something with action listener and LUA.
    m_actionListener->setAssociatedWidget(this);
}

LuaWidget& LuaWidget::operator=(const LuaWidget& other)
{
    Widget::operator=(other);
    m_luaState = other.m_luaState;
    m_actionListener = other.m_actionListener;
    if (m_luaState)
    {
        luabind::globals(m_luaState.get())["this"] = this;
    }
    //FIXME: Do something with action listener and LUA.
    m_actionListener->setAssociatedWidget(this);

    return *this;
}

void LuaWidget::draw() const
{
    if (!prepareDraw())
        return;

    std::list<Object*> children = m_children;

    for (std::list<Object*>::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->draw();

    endDraw();
}

Rect LuaWidget::getBoundingRect() const
{
    lock();

    luabind::object getBoundingRectFunc = luabind::globals(m_luaState.get())["getBoundingRect"];
    if (!getBoundingRectFunc.is_valid() || luabind::type(getBoundingRectFunc) != LUA_TFUNCTION)
    {
        unlock();
        return Widget::getBoundingRect();
    }
    Rect bb;
    try
    {
        bb = luabind::call_function<Rect>(getBoundingRectFunc);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
        unlock();
        return Widget::getBoundingRect();
    }
    unlock();
    return bb + Widget::getBoundingRect();
}

void LuaWidget::mouseEnter()
{
    lock();
    luabind::object func = luabind::globals(m_luaState.get())["mouseEnter"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseEnter();
        return;
    }
    try
    {
        luabind::call_function<void>(func);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
    Widget::mouseEnter();
}

void LuaWidget::mouseLeave()
{
    lock();
    luabind::object func = luabind::globals(m_luaState.get())["mouseLeave"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseLeave();
        return;
    }
    try
    {
        luabind::call_function<void>(func);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
    Widget::mouseLeave();
}

void LuaWidget::mouseMove(const Vector &p)
{
    lock();
    luabind::object func = luabind::globals(m_luaState.get())["mouseMove"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseMove(p);
        return;
    }
    try
    {
        luabind::call_function<void>(func, p.x, p.y);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
    Widget::mouseMove(p);
}

void LuaWidget::mouseDown(uint8_t key, const Vector &p)
{
    lock();
    luabind::object func = luabind::globals(m_luaState.get())["mouseDown"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseDown(key, p);
        return;
    }
    try
    {
        luabind::call_function<void>(func, key, p.x, p.y);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
    Widget::mouseDown(key, p);
}

void LuaWidget::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    luabind::object func = luabind::globals(m_luaState.get())["mouseUp"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseUp(key, p);
        return;
    }
    try
    {
        luabind::call_function<void>(func, key, p.x, p.y);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
    Widget::mouseUp(key, p);
}

void LuaWidget::mouseClick(const Vector &p)
{
    lock();
    luabind::object func = luabind::globals(m_luaState.get())["mouseClick"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseClick(p);
        return;
    }
    try
    {
        luabind::call_function<void>(func, p.x, p.y);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
    Widget::mouseClick(p);
}

void LuaWidget::processLogic(int dt)
{
    lock();
    std::list<Object*> children = m_children;
    for (std::list<Object*>::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->processLogic(dt);

    luabind::object func = luabind::globals(m_luaState.get())["processLogic"];
    if (!func.is_valid() || luabind::type(func) != LUA_TFUNCTION)
    {
        unlock();
        return;
    }
    try
    {
        luabind::call_function<void>(func, dt);
    }
    catch (luabind::error e)
    {
        luaErrorHandler(m_luaState.get());
    }
    unlock();
}

void LuaWidget::initLuaState()
{
    m_actionListener = new LuaActionListener(this);
    m_luaState = boost::shared_ptr<lua_State>(Rangers::initLuaState(), luaDeleter);
    luabind::globals(m_luaState.get())["this"] = this;
    luabind::globals(m_luaState.get())["actionListener"] = m_actionListener;

    lua_atpanic(m_luaState.get(), LuaWidget::luaErrorHandler);
}

void LuaWidget::dispose()
{
    Engine::instance()->markWidgetDeleting(this);
}

int LuaWidget::luaErrorHandler(lua_State* state)
{
    std::wstring error = fromUTF8(lua_tostring(state, -1));
    LuaWidget *widget = luabind::object_cast<LuaWidget*>(luabind::globals(state)["this"]);
    assert(widget != 0);
    Log::error() << "[LuaWidget] " << error;
    Engine::instance()->markWidgetDeleting(widget);
    return 0;
}
}
