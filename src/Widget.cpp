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
#include "Log.h"
#include "Action.h"
#include "ActionListener.h"

namespace Rangers
{
Rect Widget::getBoundingRect() const
{
    Rect r;
    r.x1 = 0;
    r.y1 = 0;
    r.x2 = m_width;
    r.y2 = m_height;
    for (std::list<Widget*>::const_reverse_iterator i = m_childWidgets.rbegin(); i != m_childWidgets.rend(); i++)
    {
        Rect childRect = (*i)->getBoundingRect();
        Vector position = (*i)->position();
        childRect.x1 += position.x;
        childRect.x2 += position.x;
        childRect.y1 += position.y;
        childRect.y2 += position.y;
        r += childRect;
    }
    return r;
}

Widget::Widget(Widget *parent): Object(parent), m_currentChild(0), m_width(0), m_height(0), m_leftMouseButtonPressed(false)
{
    if (parent)
        parent->addWidget(this);
}

Widget::Widget(float w, float h, Widget *parent): m_width(w), m_height(h), Object(parent), m_currentChild(0), m_leftMouseButtonPressed(false)
{
    if (parent)
        parent->addWidget(this);
}

Widget::Widget(const Rangers::Widget& other): Object(other)
{
    m_childWidgets = other.m_childWidgets;
    m_currentChild = other.m_currentChild;
    m_leftMouseButtonPressed = other.m_leftMouseButtonPressed;
    m_width = other.m_width;
    m_height = other.m_height;
    m_listeners = other.m_listeners;
    markToUpdate();
}

void Widget::mouseMove(int x, int y)
{
    lock();
    for (std::list<Widget*>::const_reverse_iterator i = m_childWidgets.rbegin(); i != m_childWidgets.rend(); i++)
    {
        Rect bb = (*i)->getBoundingRect();
        Vector pos = (*i)->position();
        if ((bb.x1 + pos.x < x) && (bb.x2 + pos.x > x) && (bb.y1 + pos.y < y) && (bb.y2 + pos.y > y))
        {
            if ((*i) != m_currentChild)
            {
                if (m_currentChild)
                {
                    m_currentChild->mouseLeave();
                }
                m_currentChild = *i;
                m_currentChild->mouseEnter();
            }
            (*i)->mouseMove(x - bb.x1 - pos.x, y - bb.y1 - pos.y);
            unlock();
            return;
        }
    }
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
    if (m_currentChild)
        m_currentChild->mouseLeave();
    m_currentChild = 0;
    m_leftMouseButtonPressed = false;
}

void Widget::mouseDown(uint8_t key, int x, int y)
{
    lock();
    if (key == SDL_BUTTON_LEFT)
        m_leftMouseButtonPressed = true;
    if (m_currentChild)
    {
        Rect b = m_currentChild->getBoundingRect();
        Vector pos = m_currentChild->position();
        m_currentChild->mouseDown(key, x - b.x1 - pos.x, y - b.y1 - pos.y);
    }
    unlock();
}

void Widget::mouseUp(uint8_t key, int x, int y)
{
    lock();
    if (m_currentChild)
    {
        Rect b = m_currentChild->getBoundingRect();
        Vector pos = m_currentChild->position();
        m_currentChild->mouseUp(key, x - b.x1 - pos.x, y - b.y1 - pos.y);
    }
    if (m_leftMouseButtonPressed && (key == SDL_BUTTON_LEFT))
    {
        m_leftMouseButtonPressed = false;
        mouseClick(x, y);
    }
    unlock();
}

void Widget::mouseClick(int x, int y)
{
}

void Widget::addWidget(Widget* w)
{
    lock();
    if (w->parent() != this)
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
    m_listeners = other.m_listeners;
    markToUpdate();

    Object::operator=(other);
    return *this;
}

void Widget::addListener(ActionListener* listener)
{
    m_listeners.push_back(listener);
}

void Widget::removeListener(ActionListener* listener)
{
    m_listeners.remove(listener);
}

void Widget::action(const Action& action)
{
    lock();
    if (!m_listeners.size())
    {
        unlock();
        return;
    }
    for (std::list<ActionListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); i++)
        (*i)->actionPerformed(action);
    unlock();
}
}
