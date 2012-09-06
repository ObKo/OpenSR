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

#include "Widget.h"
#include <iostream>
#include "Log.h"
#include "Action.h"
#include "ActionListener.h"

namespace Rangers
{
Rect Widget::getBoundingRect() const
{
    lock();
    Rect r;
    r.x = 0;
    r.y = 0;
    r.width = m_width;
    r.height = m_height;
    for (std::list<Widget*>::const_reverse_iterator i = m_childWidgets.rbegin(); i != m_childWidgets.rend(); i++)
    {
        Rect childRect = (*i)->getBoundingRect();
        Vector position = (*i)->position();
        childRect.x += position.x;
        childRect.y += position.y;
        r += childRect;
    }
    unlock();
    return r;
}

Widget::Widget(Widget *parent): Object(parent), m_currentChild(0), m_width(0), m_height(0), m_leftMouseButtonPressed(false), m_focused(false)
{
    if (parent)
        parent->addWidget(this);
}

Widget::Widget(float w, float h, Widget *parent): m_width(w), m_height(h), Object(parent), m_currentChild(0), m_leftMouseButtonPressed(false), m_focused(false)
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
    m_focused = false;
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(other.m_parent)) != 0)
        wparent->addWidget(this);
    markToUpdate();
}

Widget::~Widget()
{
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(m_parent)) != 0)
        wparent->removeWidget(this);
}

void Widget::mouseMove(const Vector& p)
{
    lock();
    for (std::list<Widget*>::reverse_iterator i = m_childWidgets.rbegin(); i != m_childWidgets.rend(); ++i)
    {
        Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
        if (bb.contains(p))
        {
            if ((*i) != m_currentChild)
            {
                if (m_currentChild)
                    m_currentChild->mouseLeave();
                m_currentChild = *i;
                m_currentChild->mouseEnter();
            }
            (*i)->mouseMove((*i)->mapFromParent(p));
            unlock();
            return;
        }
    }
    if (m_currentChild)
        m_currentChild->mouseLeave();
    m_currentChild = 0;
    unlock();
}

void Widget::mouseMove(float x, float y)
{
    mouseMove(Vector(x, y));
}

void Widget::mouseDown(uint8_t key, float x, float y)
{
    mouseDown(key, Vector(x, y));
}

void Widget::mouseUp(uint8_t key, float x, float y)
{
    mouseUp(key, Vector(x, y));
}

void Widget::mouseClick(float x, float y)
{
    mouseClick(Vector(x, y));
}

void Widget::mouseEnter()
{
    lock();
    m_leftMouseButtonPressed = false;
    unlock();
}

void Widget::mouseLeave()
{
    lock();
    if (m_currentChild)
        m_currentChild->mouseLeave();
    m_currentChild = 0;
    m_leftMouseButtonPressed = false;
    unlock();
}

void Widget::mouseDown(uint8_t key, const Vector& p)
{
    lock();
    if (m_currentChild)
    {
        m_currentChild->mouseDown(key, m_currentChild->mapFromParent(p));
    }
    if (key == SDL_BUTTON_LEFT)
        m_leftMouseButtonPressed = true;
    unlock();
}

void Widget::mouseUp(uint8_t key, const Vector& p)
{
    lock();
    if (m_currentChild)
    {
        m_currentChild->mouseUp(key, m_currentChild->mapFromParent(p));
    }
    if (m_leftMouseButtonPressed && (key == SDL_BUTTON_LEFT))
    {
        m_leftMouseButtonPressed = false;
        mouseClick(p);
    }
    unlock();
}

void Widget::mouseClick(const Vector &p)
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

int Widget::minWidth() const
{
    return -1;
}

int Widget::minHeight() const
{
    return -1;
}

int Widget::preferredWidth() const
{
    return -1;
}

int Widget::preferredHeight() const
{
    return -1;
}

int Widget::maxWidth() const
{
    return -1;
}

int Widget::maxHeight() const
{
    return -1;
}

Widget& Widget::operator=(const Rangers::Widget& other)
{
    if (this == &other)
        return *this;

    Object::operator=(other);

    m_focused = false;

    m_childWidgets = other.m_childWidgets;
    m_currentChild = other.m_currentChild;
    m_leftMouseButtonPressed = other.m_leftMouseButtonPressed;

    m_width = other.m_width;
    m_height = other.m_height;
    m_listeners = other.m_listeners;
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(other.m_parent)) != 0)
        wparent->addWidget(this);
    markToUpdate();
    return *this;
}

void Widget::addListener(ActionListener* listener)
{
    lock();
    m_listeners.push_back(listener);
    unlock();
}

void Widget::removeListener(ActionListener* listener)
{
    lock();
    m_listeners.remove(listener);
    unlock();
}

void Widget::action(const Action& action)
{
    lock();
    if (!m_listeners.size())
    {
        unlock();
        return;
    }
    std::list<ActionListener*>::iterator end = m_listeners.end();
    for (std::list<ActionListener*>::iterator i = m_listeners.begin(); i != end; ++i)
        (*i)->actionPerformed(action);
    unlock();
}

bool Widget::isFocused() const
{
    return m_focused;
}

void Widget::focus()
{
    lock();
    m_focused = true;
    unlock();
}

void Widget::unFocus()
{
    lock();
    m_focused = false;
    unlock();
}

void Widget::setGeometry(int width, int height)
{
    lock();
    m_width = width;
    m_height = height;
    markToUpdate();
    unlock();
}

void Widget::setHeight(int height)
{
    lock();
    m_height = height;
    markToUpdate();
    unlock();
}

void Widget::setWidth(int width)
{
    lock();
    m_width = width;
    markToUpdate();
    unlock();
}
}
