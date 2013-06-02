/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_WIDGET_P_H
#define RANGERS_WIDGET_P_H

#include "Object_p.h"

namespace Rangers
{
class Widget;
class ActionListener;
class WidgetPrivate: public ObjectPrivate
{
    RANGERS_DECLARE_PUBLIC(Widget)
public:
    WidgetPrivate();

    std::list<boost::shared_ptr<Widget> > childWidgets;
    std::list<boost::shared_ptr<ActionListener> > listeners;
    boost::weak_ptr<Widget> currentChild;
    int width, height;
    bool leftMouseButtonPressed;
    bool focused;
    bool visible;
};
}

#endif