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



void Widget::mouseMove(int x, int y)
{

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

float Widget::height() const
{
    return widgetHeight;
}

float Widget::width() const
{
    return widgetWidth;
}

