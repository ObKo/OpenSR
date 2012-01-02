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
#include <tolua++.h>
#include <libRanger.h>

namespace Rangers
{
class LuaDeleter
{
public:
    void operator()(lua_State*& ptr)
    {
        if (ptr)
        {
            lua_close(ptr);
            ptr = 0;
        }
    }
};

//TODO: Lua errors handling
LuaWidget::LuaActionListener::LuaActionListener(LuaWidget *widget): m_widget(widget)
{

}

void LuaWidget::LuaActionListener::actionPerformed(const Action &action)
{
    m_widget->lock();
    lua_getglobal(m_widget->m_luaState.get(), "actionPerformed");
    if (lua_isnil(m_widget->m_luaState.get(), -1))
    {
        m_widget->unlock();
        return;
    }
    //FIXME: const * -> *
    tolua_pushusertype(m_widget->m_luaState.get(), (void *)&action, "Rangers::Action");
    if (lua_pcall(m_widget->m_luaState.get(), 1, 0, 0))
        Log::warning() << "[LuaWidget] " << lua_tostring(m_widget->m_luaState.get(), -1);

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

LuaWidget::LuaWidget(Rangers::Widget* parent): Widget(parent), m_luaState(boost::shared_ptr<lua_State>((lua_State*)0, LuaDeleter()))
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
        tolua_pushusertype(m_luaState.get(), this, "Rangers::LuaWidget");
        lua_setglobal(m_luaState.get(), "this");
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
        tolua_pushusertype(m_luaState.get(), this, "Rangers::LuaWidget");
        lua_setglobal(m_luaState.get(), "this");
    }
    //FIXME: Do something with action listener and LUA.
    m_actionListener->setAssociatedWidget(this);

    return *this;
}

void LuaWidget::draw() const
{
    if (!prepareDraw())
        return;

    for (std::list<Object*>::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->draw();

    endDraw();
}

Rect LuaWidget::getBoundingRect() const
{
    lock();
    lua_getglobal(m_luaState.get(), "getBoundingRect");

    lua_pcall(m_luaState.get(), 0, 1, 0);

    Rect *bp = ((Rect*)(tolua_tousertype(m_luaState.get(), -1, 0)));
    lua_pop(m_luaState.get(), 1);

    if (!bp)
    {
        unlock();
        return Widget::getBoundingRect();
    }

    unlock();
    return *bp + Widget::getBoundingRect();
}

void LuaWidget::mouseEnter()
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseEnter");
    lua_pcall(m_luaState.get(), 0, 0, 0);
    unlock();
    Widget::mouseEnter();
}

void LuaWidget::mouseLeave()
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseLeave");
    lua_pcall(m_luaState.get(), 0, 0, 0);
    unlock();
    Widget::mouseLeave();
}

void LuaWidget::mouseMove(int x, int y)
{
    Widget::mouseMove(x, y);
    lock();
    lua_getglobal(m_luaState.get(), "mouseMove");
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
    lua_pushinteger(m_luaState.get(), key);
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    lua_pcall(m_luaState.get(), 3, 0, 0);
    unlock();
    Widget::mouseDown(key, x, y);
}

void LuaWidget::mouseUp(uint8_t key, int x, int y)
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseUp");
    lua_pushinteger(m_luaState.get(), key);
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    lua_pcall(m_luaState.get(), 3, 0, 0);
    unlock();
    Widget::mouseUp(key, x, y);
}

void LuaWidget::mouseClick(int x, int y)
{
    lock();
    lua_getglobal(m_luaState.get(), "mouseClick");
    lua_pushinteger(m_luaState.get(), x);
    lua_pushinteger(m_luaState.get(), y);
    lua_pcall(m_luaState.get(), 2, 0, 0);
    unlock();
    Widget::mouseClick(x, y);
}

void LuaWidget::processLogic(int dt)
{
    lock();
    for (std::list<Object*>::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->processLogic(dt);
    lua_getglobal(m_luaState.get(), "processLogic");
    lua_pushinteger(m_luaState.get(), dt);
    lua_pcall(m_luaState.get(), 1, 0, 0);
    unlock();
}

void LuaWidget::initLuaState()
{
    m_actionListener = new LuaActionListener(this);
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
    tolua_Node_open(m_luaState.get());
    tolua_WidgetNode_open(m_luaState.get());
    tolua_CheckBox_open(m_luaState.get());
    tolua_NinePatch_open(m_luaState.get());
    tolua_ScrollArea_open(m_luaState.get());
    tolua_pushusertype(m_luaState.get(), this, "Rangers::LuaWidget");
    lua_setglobal(m_luaState.get(), "this");
    tolua_pushusertype(m_luaState.get(), m_actionListener, "Rangers::LuaWidget::LuaActionListener");
    lua_setglobal(m_luaState.get(), "actionListener");
    tolua_pushusertype(m_luaState.get(), Engine::instance(), "Rangers::Engine");
    lua_setglobal(m_luaState.get(), "engine");
    tolua_pushusertype(m_luaState.get(), ResourceManager::instance(), "Rangers::ResourceManager");
    lua_setglobal(m_luaState.get(), "resources");
    lua_atpanic(m_luaState.get(), LuaWidget::luaErrorHandler);
}

int LuaWidget::luaErrorHandler(lua_State* state)
{
    std::wstring error = fromUTF8(lua_tostring(state, 1));
    lua_getglobal(state, "this");
    LuaWidget *widget = static_cast<LuaWidget*>(tolua_tousertype(state, -1, 0));
    assert(widget != 0);
    Log::error() << "[LuaWidget] " << error;
    Engine::instance()->markWidgetDeleting(widget);
    return 0;
}
}
