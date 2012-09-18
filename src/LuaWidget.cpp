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

#include "LuaWidget.h"
#include "Log.h"
#include "Types.h"
#include "ActionListener.h"
#include "Action.h"
#include "ResourceManager.h"
#include "LuaBindings.h"
#include "Engine.h"
#include <libRanger.h>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/luabind.hpp>
#include <SDL.h>

#include "private/LuaWidget_p.h"

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

LuaWidget::LuaActionListener::LuaActionListener(LuaWidget *widget): m_widget(widget)
{

}
void LuaWidget::LuaActionListener::actionPerformed(const Action &action)
{
    m_widget->lock();
    if (luabind::type(luabind::globals(m_widget->d_func()->luaState.get())["actionPerformed"]) != LUA_TFUNCTION)
    {
        m_widget->unlock();
        return;
    }
    try
    {
        luabind::call_function<void>(m_widget->d_func()->luaState.get(), "actionPerformed", action);
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(m_widget->d_func()->luaState.get());
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

LuaWidgetPrivate::LuaWidgetPrivate()
{
    luaState = boost::shared_ptr<lua_State>((lua_State*)0, luaDeleter);
    actionListener = 0;
}

LuaWidget::LuaWidget(const char *data, size_t size, const std::string& name, Widget *parent): Widget(*(new LuaWidgetPrivate()), parent)
{
    RANGERS_D(LuaWidget);
    d->initLuaState();
    if (luaL_loadbuffer(d->luaState.get(), data, size, name.c_str()) || lua_pcall(d->luaState.get(), 0, LUA_MULTRET, 0))
    {
        Log::error() << "[LuaWidget] " << lua_tostring(d->luaState.get(), -1);
        Engine::instance()->markWidgetDeleting(this);
    }
}

LuaWidget::LuaWidget(const std::wstring& name, Widget *parent): Widget(*(new LuaWidgetPrivate()), parent)
{
    RANGERS_D(LuaWidget);
    d->initLuaState();
    size_t size;
    char *luaData = ResourceManager::instance()->loadData(name, size);
    if (luaData)
    {
        if (luaL_loadbuffer(d->luaState.get(), luaData, size, toLocal(name.c_str()).c_str()) || lua_pcall(d->luaState.get(), 0, LUA_MULTRET, 0))
        {
            Log::error() << "[LuaWidget] " << lua_tostring(d->luaState.get(), -1);
            Engine::instance()->markWidgetDeleting(this);
        }
        delete[] luaData;
    }
}

LuaWidget::LuaWidget(Rangers::Widget* parent): Widget(*(new LuaWidgetPrivate()), parent)
{
}

LuaWidget::~LuaWidget()
{
    RANGERS_D(LuaWidget);
    if (d->actionListener)
        delete d->actionListener;
}

LuaWidget::LuaWidget(const LuaWidget& other): Widget(*(new LuaWidgetPrivate()), other)
{
    RANGERS_D(LuaWidget);
    d->luaState = other.d_func()->luaState;
    d->actionListener = other.d_func()->actionListener;
    if (d->luaState)
    {
        luabind::globals(d->luaState.get())["this"] = this;
    }
    //FIXME: Do something with action listener and LUA.
    if (d->actionListener)
        d->actionListener->setAssociatedWidget(this);
}

LuaWidget& LuaWidget::operator=(const LuaWidget& other)
{
    RANGERS_D(LuaWidget);
    Widget::operator=(other);
    d->luaState = other.d_func()->luaState;
    d->actionListener = other.d_func()->actionListener;
    if (d->luaState)
    {
        luabind::globals(d->luaState.get())["this"] = this;
    }
    //FIXME: Do something with action listener and LUA.
    if (d->actionListener)
        d->actionListener->setAssociatedWidget(this);

    return *this;
}

void LuaWidget::draw() const
{
    RANGERS_D(const LuaWidget);
    if (!prepareDraw())
        return;

    std::list<Object*> children = d->children;

    for (std::list<Object*>::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->draw();

    endDraw();
}

Rect LuaWidget::getBoundingRect() const
{
    lock();
    RANGERS_D(const LuaWidget);

    if (luabind::type(luabind::globals(d->luaState.get())["getBoundingRect"]) != LUA_TFUNCTION)
    {
        unlock();
        return Widget::getBoundingRect();
    }
    Rect bb;
    try
    {
        bb = luabind::call_function<Rect>(d->luaState.get(), "getBoundingRect");
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
        unlock();
        return Widget::getBoundingRect();
    }
    unlock();
    return bb + Widget::getBoundingRect();
}

void LuaWidget::mouseEnter()
{
    lock();
    RANGERS_D(LuaWidget);
    if (luabind::type(luabind::globals(d->luaState.get())["mouseEnter"]) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseEnter();
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "mouseEnter");
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
    Widget::mouseEnter();
}

void LuaWidget::mouseLeave()
{
    lock();
    RANGERS_D(LuaWidget);
    if (luabind::type(luabind::globals(d->luaState.get())["mouseLeave"]) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseLeave();
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "mouseLeave");
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
    Widget::mouseLeave();
}

void LuaWidget::mouseMove(const Vector &p)
{
    lock();
    RANGERS_D(LuaWidget);
    if (luabind::type(luabind::globals(d->luaState.get())["mouseMove"]) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseMove(p);
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "mouseMove", p.x, p.y);
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
    Widget::mouseMove(p);
}

void LuaWidget::mouseDown(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(LuaWidget);
    if (luabind::type(luabind::globals(d->luaState.get())["mouseDown"]) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseDown(key, p);
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "mouseDown", key, p.x, p.y);
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
    Widget::mouseDown(key, p);
}

void LuaWidget::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(LuaWidget);
    if (luabind::type(luabind::globals(d->luaState.get())["mouseUp"]) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseUp(key, p);
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "mouseUp", key, p.x, p.y);
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
    Widget::mouseUp(key, p);
}

void LuaWidget::mouseClick(const Vector &p)
{
    lock();
    RANGERS_D(LuaWidget);
    if (luabind::type(luabind::globals(d->luaState.get())["mouseClick"]) != LUA_TFUNCTION)
    {
        unlock();
        Widget::mouseClick(p);
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "mouseClick", p.x, p.y);
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
    Widget::mouseClick(p);
}

void LuaWidget::processLogic(int dt)
{
    lock();
    RANGERS_D(LuaWidget);
    std::list<Object*> children = d->children;
    unlock();
    for (std::list<Object*>::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->processLogic(dt);

    lock();
    if (luabind::type(luabind::globals(d->luaState.get())["processLogic"]) != LUA_TFUNCTION)
    {
        unlock();
        return;
    }
    try
    {
        luabind::call_function<void>(d->luaState.get(), "processLogic", dt);
    }
    catch (luabind::error e)
    {
        LuaWidgetPrivate::luaErrorHandler(d->luaState.get());
    }
    unlock();
}

void LuaWidgetPrivate::initLuaState()
{
    RANGERS_Q(LuaWidget);
    actionListener = new LuaWidget::LuaActionListener(q);
    luaState = boost::shared_ptr<lua_State>(Rangers::initLuaState(), luaDeleter);
    luabind::globals(luaState.get())["this"] = q;
    luabind::globals(luaState.get())["actionListener"] = actionListener;

    lua_atpanic(luaState.get(), LuaWidgetPrivate::luaErrorHandler);
}

void LuaWidget::dispose()
{
    Engine::instance()->markWidgetDeleting(this);
}

int LuaWidgetPrivate::luaErrorHandler(lua_State* state)
{
    std::wstring error = fromUTF8(lua_tostring(state, -1));
    LuaWidget *widget = luabind::object_cast<LuaWidget*>(luabind::globals(state)["this"]);
    assert(widget != 0);
    Log::error() << "[LuaWidget] " << error;
    Engine::instance()->markWidgetDeleting(widget);
    return 0;
}
}
