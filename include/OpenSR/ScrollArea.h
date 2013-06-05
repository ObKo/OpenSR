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

#ifndef RANGERS_SCROLLAREA_H
#define RANGERS_SCROLLAREA_H

#include "OpenSR/Widget.h"

namespace Rangers
{
class WidgetNode;
class ScrollAreaPrivate;
struct ScrollBarStyle;
class RANGERS_ENGINE_API ScrollArea: public Widget
{
    RANGERS_DECLARE_PRIVATE(ScrollArea)
public:
    ScrollArea(Widget* parent = 0);
    ScrollArea(const ScrollBarStyle& style, boost::shared_ptr<WidgetNode> node = boost::shared_ptr<WidgetNode>(), Widget *parent = 0);

    virtual void draw() const;
    virtual void processMain();
    virtual Rect getBoundingRect() const;

    void setWidth(float width);
    void setHeight(float height);

    void setNode(boost::shared_ptr<WidgetNode> node);

    virtual void mouseMove(const Vector &p);

    virtual void processLogic(int dt);

protected:
    ScrollArea(ScrollAreaPrivate &p, Widget *parent = 0);

    RANGERS_DISABLE_COPY(ScrollArea)
};
}

#endif // RANGERS_SCROLLAREA_H
