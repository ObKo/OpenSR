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
#include <SDL.h>
#include "Action.h"
#include "ActionListener.h"

#include "private/Widget_p.h"

namespace Rangers
{
WidgetPrivate::WidgetPrivate(): ObjectPrivate()
{
    m_currentChild = 0;
    m_width = 0;
    m_height = 0;
    m_leftMouseButtonPressed = false;
    m_focused = false;
}

Widget::Widget(Widget *parent): Object(*(new WidgetPrivate()), parent)
{
    RANGERS_D(Widget);

    if (parent)
        parent->addWidget(this);
}

Widget::Widget(float w, float h, Widget *parent): Object(*(new WidgetPrivate()), parent)
{
    RANGERS_D(Widget);

    d->m_width = w;
    d->m_height = h;

    if (parent)
        parent->addWidget(this);
}

Widget::Widget(const Rangers::Widget& other): Object(*(new WidgetPrivate()), other)
{
    RANGERS_D(Widget);

    d->m_childWidgets = other.d_func()->m_childWidgets;
    d->m_currentChild = other.d_func()->m_currentChild;
    d->m_leftMouseButtonPressed = other.d_func()->m_leftMouseButtonPressed;
    d->m_width = other.d_func()->m_width;
    d->m_height = other.d_func()->m_height;
    d->m_listeners = other.d_func()->m_listeners;
    d->m_focused = false;
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(other.d_func()->m_parent)) != 0)
        wparent->addWidget(this);
    markToUpdate();
}

Widget::Widget(WidgetPrivate &p, const Widget& other): Object(p, other)
{
    RANGERS_D(Widget);

    d->m_childWidgets = other.d_func()->m_childWidgets;
    d->m_currentChild = other.d_func()->m_currentChild;
    d->m_leftMouseButtonPressed = other.d_func()->m_leftMouseButtonPressed;
    d->m_width = other.d_func()->m_width;
    d->m_height = other.d_func()->m_height;
    d->m_listeners = other.d_func()->m_listeners;
    d->m_focused = false;
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(other.d_func()->m_parent)) != 0)
        wparent->addWidget(this);
    markToUpdate();
}

Widget::Widget(WidgetPrivate &p, Widget *parent): Object(p, parent)
{
    RANGERS_D(Widget);

    if (parent)
        parent->addWidget(this);
}

Widget::~Widget()
{
    RANGERS_D(Widget);
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(d->m_parent)) != 0)
        wparent->removeWidget(this);
}

void Widget::mouseMove(const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    for (std::list<Widget*>::reverse_iterator i = d->m_childWidgets.rbegin(); i != d->m_childWidgets.rend(); ++i)
    {
        Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
        if (bb.contains(p))
        {
            if ((*i) != d->m_currentChild)
            {
                if (d->m_currentChild)
                    d->m_currentChild->mouseLeave();
                d->m_currentChild = *i;
                d->m_currentChild->mouseEnter();
            }
            (*i)->mouseMove((*i)->mapFromParent(p));
            unlock();
            return;
        }
    }
    if (d->m_currentChild)
        d->m_currentChild->mouseLeave();
    d->m_currentChild = 0;
    unlock();
}

Rect Widget::getBoundingRect() const
{
    RANGERS_D(const Widget);
    lock();
    Rect r;
    r.x = 0;
    r.y = 0;
    r.width = d->m_width;
    r.height = d->m_height;
    for (std::list<Widget*>::const_reverse_iterator i = d->m_childWidgets.rbegin(); i != d->m_childWidgets.rend(); i++)
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
    RANGERS_D(Widget);
    lock();
    d->m_leftMouseButtonPressed = false;
    unlock();
}

void Widget::mouseLeave()
{
    RANGERS_D(Widget);
    lock();
    if (d->m_currentChild)
        d->m_currentChild->mouseLeave();
    d->m_currentChild = 0;
    d->m_leftMouseButtonPressed = false;
    unlock();
}

void Widget::mouseDown(uint8_t key, const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    if (d->m_currentChild)
    {
        d->m_currentChild->mouseDown(key, d->m_currentChild->mapFromParent(p));
    }
    if (key == SDL_BUTTON_LEFT)
        d->m_leftMouseButtonPressed = true;
    unlock();
}

void Widget::mouseUp(uint8_t key, const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    if (d->m_currentChild)
    {
        d->m_currentChild->mouseUp(key, d->m_currentChild->mapFromParent(p));
    }
    if (d->m_leftMouseButtonPressed && (key == SDL_BUTTON_LEFT))
    {
        d->m_leftMouseButtonPressed = false;
        mouseClick(p);
    }
    unlock();
}

void Widget::mouseClick(const Vector &p)
{
}

void Widget::addWidget(Widget* w)
{
    RANGERS_D(Widget);
    lock();
    if (w->parent() != this)
        addChild(w);
    for (std::list<Widget*>::iterator i = d->m_childWidgets.begin(); i != d->m_childWidgets.end(); i++)
    {
        if ((*i)->layer() > w->layer())
        {
            d->m_childWidgets.insert(i, w);
            unlock();
            return;
        }
    }
    d->m_childWidgets.push_back(w);
    unlock();
}

void Widget::removeWidget(Widget* w)
{
    RANGERS_D(Widget);
    lock();
    d->m_childWidgets.remove(w);
    removeChild(w);
    if (d->m_currentChild == w)
        d->m_currentChild = 0;
    unlock();
}

int Widget::height() const
{
    RANGERS_D(const Widget);
    return d->m_height;
}

int Widget::width() const
{
    RANGERS_D(const Widget);
    return d->m_width;
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

    RANGERS_D(Widget);

    Object::operator=(other);

    d->m_focused = false;

    d->m_childWidgets = other.d_func()->m_childWidgets;
    d->m_currentChild = other.d_func()->m_currentChild;
    d->m_leftMouseButtonPressed = other.d_func()->m_leftMouseButtonPressed;

    d->m_width = other.d_func()->m_width;
    d->m_height = other.d_func()->m_height;
    d->m_listeners = other.d_func()->m_listeners;
    //FIXME: Ugly downcasting
    Widget* wparent;
    if ((wparent = dynamic_cast<Widget*>(other.d_func()->m_parent)) != 0)
        wparent->addWidget(this);
    markToUpdate();
    return *this;
}

void Widget::addListener(ActionListener* listener)
{
    RANGERS_D(Widget);
    lock();
    d->m_listeners.push_back(listener);
    unlock();
}

void Widget::removeListener(ActionListener* listener)
{
    RANGERS_D(Widget);
    lock();
    d->m_listeners.remove(listener);
    unlock();
}

void Widget::action(const Action& action)
{
    RANGERS_D(Widget);
    lock();
    if (!d->m_listeners.size())
    {
        unlock();
        return;
    }
    std::list<ActionListener*>::iterator end = d->m_listeners.end();
    for (std::list<ActionListener*>::iterator i = d->m_listeners.begin(); i != end; ++i)
        (*i)->actionPerformed(action);
    unlock();
}

bool Widget::isFocused() const
{
    RANGERS_D(const Widget);
    return d->m_focused;
}

void Widget::focus()
{
    RANGERS_D(Widget);
    lock();
    d->m_focused = true;
    unlock();
}

void Widget::unFocus()
{
    RANGERS_D(Widget);
    lock();
    d->m_focused = false;
    unlock();
}

void Widget::setGeometry(int width, int height)
{
    RANGERS_D(Widget);
    lock();
    d->m_width = width;
    d->m_height = height;
    markToUpdate();
    unlock();
}

void Widget::setHeight(int height)
{
    RANGERS_D(Widget);
    lock();
    d->m_height = height;
    markToUpdate();
    unlock();
}

void Widget::setWidth(int width)
{
    RANGERS_D(Widget);
    lock();
    d->m_width = width;
    markToUpdate();
    unlock();
}
}
