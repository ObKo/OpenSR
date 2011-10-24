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

#ifndef LUAWIDGET_H
#define LUAWIDGET_H

#include "Widget.h"
#include "ActionListener.h"

extern "C"
{
#include <lua.h>
}

namespace Rangers
{
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
    LuaWidget(const LuaWidget& other);

    virtual void draw() const;

    virtual Rect getBoundingRect() const;
    virtual void mouseMove(int x, int y);
    virtual void mouseEnter();
    virtual void mouseLeave();
    virtual void keyPressed(SDL_keysym key);

    virtual void mouseDown(uint8_t key, int x, int y);
    virtual void mouseUp(uint8_t key, int x, int y);
    virtual void mouseClick(int x, int y);

    virtual void processMain();
    virtual void processLogic(int dt);

    LuaWidget& operator=(const LuaWidget& other);

private:
    void initLuaState();

    boost::shared_ptr<lua_State> m_luaState;
    LuaActionListener *m_actionListener;
};
}

#endif // LUAWIDGET_H
