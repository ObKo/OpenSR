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

using namespace Rangers;

Rect Widget::getBoundingRect() const
{
    Rect r;
    r.x1 = objPosition.x;
    r.y1 = objPosition.y;
    r.x2 = objPosition.x + widgetWidth;
    r.y2 = objPosition.y + widgetHeight;
    return r;
}

Widget::Widget(Object *parent): Object(parent)
{
}

Widget::Widget(float w, float h,  Object *parent): widgetWidth(w), widgetHeight(h), Object(parent)
{
}

Widget::Widget(const Rangers::Widget& other): Object(other)
{
    widgetWidth = other.widgetWidth;
    widgetHeight = other.widgetHeight;
    markToUpdate();
}


void Widget::mouseMove(int x, int y)
{
    for (std::list<Widget*>::const_reverse_iterator i = childWidgets.rbegin(); i != childWidgets.rend(); i++)
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
            return;
        }
    }
    if (currentChildWidget)
        currentChildWidget->mouseLeave();
    currentChildWidget = 0;
}

void Widget::mouseEnter()
{

}

void Widget::mouseLeave()
{

}

void Widget::keyPressed(SDL_keysym key)
{

}

void Widget::addWidget(Widget* w)
{
    if(w->parent() == this)
        return;
    lock();
    addChild(w);
    for (std::list<Widget*>::iterator i = childWidgets.begin(); i != childWidgets.end(); i++)
    {
        if ((*i)->layer() > w->layer())
        {
            childWidgets.insert(i, w);
            unlock();
            return;
        }
    }
    objectChilds.push_back(w);
    unlock();
}

void Widget::removeWidget(Widget* w)
{
    lock();
    childWidgets.remove(w);
    removeChild(w);
    if(currentChildWidget == w)
	currentChildWidget = 0;
    unlock();
}

int Widget::height() const
{
    return widgetHeight;
}

int Widget::width() const
{
    return widgetWidth;
}

Widget& Widget::operator=(const Rangers::Widget& other)
{
    if(this == &other)
	return *this;
    
    widgetWidth = other.widgetWidth;
    widgetHeight = other.widgetHeight;
    markToUpdate();
    
    ::Object::operator=(other);    
    return *this;
}



