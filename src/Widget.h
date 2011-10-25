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

#ifndef RANGERS_WIDGET_H
#define RANGERS_WIDGET_H

#include "Object.h"

namespace Rangers
{
class ActionListener;
class Action;
class Widget: public Object
{
public:
    Widget(Widget *parent = 0);
    Widget(float w, float h, Widget *parent = 0);
    Widget(const Widget& other);

    virtual void draw() const = 0;

    virtual Rect getBoundingRect() const;
    virtual void mouseMove(int x, int y);
    virtual void mouseEnter();
    virtual void mouseLeave();

    virtual void mouseDown(uint8_t key, int x, int y);
    virtual void mouseUp(uint8_t key, int x, int y);
    virtual void mouseClick(int x, int y);

    int width() const;
    int height() const;

    Widget& operator=(const Widget& other);

    void addWidget(Widget *w);
    void removeWidget(Widget *w);

    void addListener(ActionListener* listener);
    void removeListener(ActionListener* listener);

    void action(const Action& action);

protected:
    std::list<Widget *> m_childWidgets;
    std::list<ActionListener*> m_listeners;
    Widget *m_currentChild;
    int m_width, m_height;
    bool m_leftMouseButtonPressed;
};
}

#endif // WIDGET_H
