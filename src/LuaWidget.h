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

#ifndef RANGERS_LUAWIDGET_H
#define RANGERS_LUAWIDGET_H

#include "Widget.h"
#include "ActionListener.h"

extern "C"
{
#include <lua.h>
}

namespace Rangers
{
//TODO: min/max/preferred sizes.
class LuaWidget: public Widget
{
public:
    class LuaActionListener: public ActionListener
    {
    public:
        LuaActionListener(LuaWidget *widget);
        void actionPerformed(const Action &action);

        LuaWidget* associatedWidget() const;
        void setAssociatedWidget(LuaWidget *widget);

    private:
        LuaWidget *m_widget;
    };

    LuaWidget(const char *data, size_t size, const std::string& name, Widget *parent = 0);
    LuaWidget(const std::wstring& name, Widget *parent = 0);
    LuaWidget(Widget *parent = 0);

    virtual ~LuaWidget();

    void dispose();

    virtual void draw() const;

    virtual Rect getBoundingRect() const;

    virtual void mouseEnter();
    virtual void mouseLeave();
    virtual void mouseMove(const Vector &p);
    virtual void mouseDown(uint8_t key, const Vector &p);
    virtual void mouseUp(uint8_t key, const Vector &p);
    virtual void mouseClick(const Vector &p);

    virtual void processLogic(int dt);

private:
    LuaWidget(const LuaWidget& other);
    LuaWidget& operator=(const LuaWidget& other);

    void initLuaState();

    boost::shared_ptr<lua_State> m_luaState;
    LuaActionListener *m_actionListener;

    static int luaErrorHandler(lua_State* state);
};
}

#endif // LUAWIDGET_H
