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

#include "OpenSR/Widget.h"
#include "OpenSR/private/Widget_p.h"

#include <SDL.h>

#include "OpenSR/Action.h"
#include "OpenSR/ActionListener.h"
#include "OpenSR/Engine.h"

namespace Rangers
{
WidgetPrivate::WidgetPrivate(): ObjectPrivate()
{
    width = 0;
    height = 0;
    leftMouseButtonPressed = false;
    focused = false;
    visible = true;
}

Widget::Widget(): Object(*(new WidgetPrivate()))
{
    RANGERS_D(Widget);
}

Widget::Widget(float w, float h): Object(*(new WidgetPrivate()))
{
    RANGERS_D(Widget);

    d->width = w;
    d->height = h;
}

Widget::Widget(WidgetPrivate &p): Object(p)
{
}

Widget::~Widget()
{
    RANGERS_D(Widget);
    //FIXME: Ugly downcasting
    //Widget* wparent;
    //if ((wparent = dynamic_cast<Widget*>(d->parent)) != 0)
    //    wparent->removeWidget(this);

    Engine::instance().widgetDestroyed(this);
}

void Widget::mouseMove(const Vector& p)
{
    RANGERS_D(Widget);
    lock();
    for (std::list<boost::shared_ptr<Widget> >::reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); ++i)
    {
        if ((*i)->isVisible() && (*i)->containsPoint((*i)->mapFromParent(p)))
        {
            boost::shared_ptr<Widget> child = d->currentChild.lock();
            if ((*i) != child)
            {
                if (child)
                    child->action(Action(child, Action::MOUSE_LEAVE));
                d->currentChild = *i;
                (*i)->action(Action((*i), Action::MOUSE_ENTER));
            }
            (*i)->mouseMove((*i)->mapFromParent(p));
            unlock();
            return;
        }
    }
    if (boost::shared_ptr<Widget> child = d->currentChild.lock())
        child->action(Action(child, Action::MOUSE_LEAVE));
    d->currentChild = boost::weak_ptr<Widget>();
    unlock();
}

bool Widget::containsPoint(const Vector &p) const
{
    RANGERS_D(const Widget);

    if ((p.x >= 0) && (p.x <= d->width) && (p.y >= 0) && (p.y <= d->height))
        return true;

    for (std::list<boost::shared_ptr<Widget> >::const_reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); ++i)
    {
        if ((*i)->containsPoint((*i)->mapFromParent(p)))
            return true;
    }
}

Rect Widget::boundingRect() const
{
    RANGERS_D(const Widget);

    Rect r;

    r.x = 0.0f;
    r.y = 0.0f;
    r.width = d->width;
    r.height = d->height;

    for (std::list<boost::shared_ptr<Widget> >::const_reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); ++i)
    {
        r += (*i)->mapToParent((*i)->boundingRect());
    }
    return r;
}

void Widget::mouseMove(float x, float y)
{
    mouseMove(Vector(x, y));
}

void Widget::addWidget(boost::shared_ptr<Widget> w)
{
    RANGERS_D(Widget);
    lock();

    addChild(w);

    for (std::list<boost::shared_ptr<Widget> >::iterator i = d->childWidgets.begin(); i != d->childWidgets.end(); ++i)
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

void Widget::removeWidget(boost::shared_ptr<Widget> w)
{
    RANGERS_D(Widget);
    lock();
    d->childWidgets.remove(w);
    removeChild(w);
    if (d->currentChild.lock() == w)
        d->currentChild = boost::weak_ptr<Widget>();
    unlock();
}

std::list<boost::shared_ptr<Widget> > Widget::childWidgets() const
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

void Widget::addListener(boost::shared_ptr<ActionListener> listener)
{
    RANGERS_D(Widget);
    lock();
    d->listeners.push_back(listener);
    unlock();
}

void Widget::removeListener(boost::shared_ptr<ActionListener> listener)
{
    RANGERS_D(Widget);
    lock();
    d->listeners.remove(listener);
    unlock();
}

void Widget::action(const Action& a)
{
    RANGERS_D(Widget);
    lock();
    if (a.source().get() != this)
    {
        unlock();
        return;
    }
    boost::shared_ptr<Widget> currentChild = d->currentChild.lock();
    switch (a.type())
    {
    case Action::MOUSE_ENTER:
        d->leftMouseButtonPressed = false;
        break;

    case Action::MOUSE_LEAVE:
        if (currentChild)
            currentChild->action(Action(currentChild, Action::MOUSE_LEAVE));

        d->currentChild = boost::weak_ptr<Widget>();
        d->leftMouseButtonPressed = false;
        break;

    case Action::MOUSE_DOWN:
        if (boost::get<uint8_t>(a.argument()) == SDL_BUTTON_LEFT)
            d->leftMouseButtonPressed = true;
        if (currentChild)
            currentChild->action(Action(currentChild, Action::MOUSE_DOWN, a.argument()));
        break;

    case Action::MOUSE_UP:
        if (d->leftMouseButtonPressed && (boost::get<uint8_t>(a.argument()) == SDL_BUTTON_LEFT))
        {
            d->leftMouseButtonPressed = false;
            action(Action(a.source(), Action::MOUSE_CLICK, a.argument()));
        }
        if (currentChild)
            currentChild->action(Action(currentChild, Action::MOUSE_UP, a.argument()));
        break;
    }
    if (!d->listeners.size())
    {
        unlock();
        return;
    }
    std::list<boost::shared_ptr<ActionListener> > listeners = d->listeners;
    std::list<boost::shared_ptr<ActionListener> >::iterator end = listeners.end();
    for (std::list<boost::shared_ptr<ActionListener> >::iterator i = listeners.begin(); i != end; ++i)
    {
        boost::shared_ptr<ActionListener> l = *i;
        l->actionPerformed(a);
    }
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
