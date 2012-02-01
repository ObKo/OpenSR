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

#ifndef RANGERS_SCROLLAREA_H
#define RANGERS_SCROLLAREA_H
#include "Widget.h"
#include "Button.h"
#include "ActionListener.h"

namespace Rangers
{
class WidgetNode;
class ScrollArea: public Widget, ActionListener
{
public:
    ScrollArea(const ScrollBarStyle& style, WidgetNode *node = 0, Widget *parent = 0);

    virtual void draw() const;
    virtual void processMain();
    virtual Rect getBoundingRect() const;

    void setWidth(float width);
    void setHeight(float height);

    void setNode(WidgetNode *node);

    virtual void mouseMove(const Vector &p);
    virtual void mouseEnter();
    virtual void mouseLeave();

    virtual void mouseDown(uint8_t key, const Vector &p);
    virtual void mouseUp(uint8_t key, const Vector &p);

    virtual void actionPerformed(const Action &action);

private:
    void updateScrollPosition();
    Button m_top, m_bottom, m_left, m_right;
    Button m_vScroll, m_hScroll;
    float m_vSize, m_vPosition;
    float m_hSize, m_hPosition;
    WidgetNode *m_node;
    float m_scrollStart;
    enum {NONE, VERTICAL, HORIZONTAL} m_scrollDrag;
};
}

#endif // RANGERS_SCROLLAREA_H
