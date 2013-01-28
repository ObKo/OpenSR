/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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
#include <Engine.h>

namespace Rangers
{
WidgetPrivate::WidgetPrivate(): ObjectPrivate()
{
    currentChild = 0;
    width = 0;
    height = 0;
    leftMouseButtonPressed = false;
    focused = false;
    visible = true;
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

    d->width = w;
    d->height = h;

    if (parent)
        parent->addWidget(this);
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
    if ((wparent = dynamic_cast<Widget*>(d->parent)) != 0)
        wparent->removeWidget(this);

    Engine::instance().widgetDestroyed(this);
}

void Widget::mouseMove(const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    for (std::list<Widget*>::reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); ++i)
    {
        Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
        if ((*i)->isVisible() && bb.contains(p))
        {
            if ((*i) != d->currentChild)
            {
                if (d->currentChild)
                    d->currentChild->action(Action(d->currentChild, Action::MOUSE_LEAVE));
                d->currentChild = *i;
                d->currentChild->action(Action(d->currentChild, Action::MOUSE_ENTER));
            }
            (*i)->mouseMove((*i)->mapFromParent(p));
            unlock();
            return;
        }
    }
    if (d->currentChild)
        d->currentChild->action(Action(d->currentChild, Action::MOUSE_LEAVE));
    d->currentChild = 0;
    unlock();
}

Rect Widget::getBoundingRect() const
{
    RANGERS_D(const Widget);
    lock();
    Rect r;
    r.x = 0;
    r.y = 0;
    r.width = d->width;
    r.height = d->height;
    for (std::list<Widget*>::const_reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); i++)
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

void Widget::mouseDown(uint8_t key, const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    if (d->currentChild)
    {
        d->currentChild->mouseDown(key, d->currentChild->mapFromParent(p));
    }
    if (key == SDL_BUTTON_LEFT)
        d->leftMouseButtonPressed = true;
    unlock();
}

void Widget::mouseUp(uint8_t key, const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    if (d->currentChild)
    {
        d->currentChild->mouseUp(key, d->currentChild->mapFromParent(p));
    }
    if (d->leftMouseButtonPressed && (key == SDL_BUTTON_LEFT))
    {
        d->leftMouseButtonPressed = false;
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
    for (std::list<Widget*>::iterator i = d->childWidgets.begin(); i != d->childWidgets.end(); i++)
    {
        if ((*i)->layer() > w->layer())
        {
            d->childWidgets.insert(i, w);
            unlock();
            return;
        }
    }
    d->childWidgets.push_back(w);
    unlock();
}

void Widget::removeWidget(Widget* w)
{
    RANGERS_D(Widget);
    lock();
    d->childWidgets.remove(w);
    removeChild(w);
    if (d->currentChild == w)
        d->currentChild = 0;
    unlock();
}

std::list<Widget *> Widget::childWidgets() const
{
    RANGERS_D(const Widget);
    return d->childWidgets;
}

int Widget::height() const
{
    RANGERS_D(const Widget);
    return d->height;
}

int Widget::width() const
{
    RANGERS_D(const Widget);
    return d->width;
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

void Widget::addListener(ActionListener* listener)
{
    RANGERS_D(Widget);
    lock();
    d->listeners.push_back(listener);
    unlock();
}

void Widget::removeListener(ActionListener* listener)
{
    RANGERS_D(Widget);
    lock();
    d->listeners.remove(listener);
    unlock();
}

void Widget::action(const Action& action)
{
    RANGERS_D(Widget);
    lock();
    if (action.source() != this)
    {
        unlock();
        return;
    }
    switch (action.type())
    {
    case Action::MOUSE_ENTER:
        d->leftMouseButtonPressed = false;
        break;

    case Action::MOUSE_LEAVE:
        if (d->currentChild)
            d->currentChild->action(Action(d->currentChild, Action::MOUSE_LEAVE));
        d->currentChild = 0;
        d->leftMouseButtonPressed = false;
        break;
    }
    if (!d->listeners.size())
    {
        unlock();
        return;
    }
    std::list<ActionListener*>::iterator end = d->listeners.end();
    for (std::list<ActionListener*>::iterator i = d->listeners.begin(); i != end; ++i)
        (*i)->actionPerformed(action);
    unlock();
}

bool Widget::isFocused() const
{
    RANGERS_D(const Widget);
    return d->focused;
}

void Widget::focus()
{
    RANGERS_D(Widget);
    lock();
    d->focused = true;
    unlock();
}

void Widget::unFocus()
{
    RANGERS_D(Widget);
    lock();
    d->focused = false;
    unlock();
}

void Widget::setGeometry(int width, int height)
{
    RANGERS_D(Widget);
    lock();
    d->width = width;
    d->height = height;
    markToUpdate();
    unlock();
}

void Widget::setHeight(int height)
{
    RANGERS_D(Widget);
    lock();
    d->height = height;
    markToUpdate();
    unlock();
}

void Widget::setWidth(int width)
{
    RANGERS_D(Widget);
    lock();
    d->width = width;
    markToUpdate();
    unlock();
}

bool Widget::isVisible() const
{
    RANGERS_D(const Widget);
    return d->visible;
}

void Widget::setVisible(bool visible)
{
    RANGERS_D(Widget);
    d->visible = visible;
    if (!visible)
        Engine::instance().widgetHide(this);
}

bool Widget::prepareDraw() const
{
    RANGERS_D(const Widget);
    return d->visible && Object::prepareDraw();
}

}
