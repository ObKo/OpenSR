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

#include "Widget.h"
#include <iostream>

namespace Rangers
{
Rect Widget::getBoundingRect() const
{
    Rect r;
    r.x1 = m_position.x;
    r.y1 = m_position.y;
    r.x2 = m_position.x + m_width;
    r.y2 = m_position.y + m_height;
    return r;
}

Widget::Widget(Object *parent): Object(parent), m_currentChild(0)
{
}

Widget::Widget(float w, float h,  Object *parent): m_width(w), m_height(h), Object(parent), m_currentChild(0)
{
}

Widget::Widget(const Rangers::Widget& other): Object(other)
{
    m_childWidgets = other.m_childWidgets;
    m_currentChild = other.m_currentChild;
    m_leftMouseButtonPressed = other.m_leftMouseButtonPressed;
    m_width = other.m_width;
    m_height = other.m_height;
    markToUpdate();
}


void Widget::mouseMove(int x, int y)
{
    lock();
    /*for (std::list<Widget*>::const_reverse_iterator i = childWidgets.rbegin(); i != childWidgets.rend(); i++)
    {
        Rect bb = (*i)->getBoundingRect();
        Vector pos = (*i)->position();
        if ((bb.x1 + pos.x < x) && (bb.x2 + pos.x > x) && (bb.y1 + pos.y < y) && (bb.y2 + pos.y > y))
        {
            if ((*i) != currentChildWidget)
            {
                if (currentChildWidget)
                    currentChildWidget->mouseLeave();
                currentChildWidget = *i;
                currentChildWidget->mouseEnter();
            }
            (*i)->mouseMove(x - bb.x1 - pos.x, y - bb.y1 - pos.y);
            unlock();
            return;
        }
    }*/
    if (m_currentChild)
        m_currentChild->mouseLeave();
    m_currentChild = 0;
    unlock();
}

void Widget::mouseEnter()
{
    m_leftMouseButtonPressed = false;
}

void Widget::mouseLeave()
{
    m_leftMouseButtonPressed = false;
}

void Widget::keyPressed(SDL_keysym key)
{

}

void Widget::mouseDown(uint8_t key, int x, int y)
{
    if (key == SDL_BUTTON_LEFT)
        m_leftMouseButtonPressed = true;
}

void Widget::mouseUp(uint8_t key, int x, int y)
{
    if (m_leftMouseButtonPressed && (key == SDL_BUTTON_LEFT))
    {
        m_leftMouseButtonPressed = false;
        mouseClick(x, y);
    }
}

void Widget::mouseClick(int x, int y)
{

}

void Widget::addWidget(Widget* w)
{
    if (w->parent() == this)
        return;
    lock();
    addChild(w);
    for (std::list<Widget*>::iterator i = m_childWidgets.begin(); i != m_childWidgets.end(); i++)
    {
        if ((*i)->layer() > w->layer())
        {
            m_childWidgets.insert(i, w);
            unlock();
            return;
        }
    }
    m_childWidgets.push_back(w);
    unlock();
}

void Widget::removeWidget(Widget* w)
{
    lock();
    m_childWidgets.remove(w);
    removeChild(w);
    if (m_currentChild == w)
        m_currentChild = 0;
    unlock();
}

int Widget::height() const
{
    return m_height;
}

int Widget::width() const
{
    return m_width;
}

Widget& Widget::operator=(const Rangers::Widget& other)
{
    if (this == &other)
        return *this;

    Object::operator=(other);
    //(std::list<Widget*>::iterator i = m_childWidgets.begin(); i != m_childWidgets.end(); i++)
    //    i->set

    m_childWidgets = other.m_childWidgets;
    m_currentChild = other.m_currentChild;
    m_leftMouseButtonPressed = other.m_leftMouseButtonPressed;

    m_width = other.m_width;
    m_height = other.m_height;
    markToUpdate();

    Object::operator=(other);
    return *this;
}
}


