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
    ScrollArea(const std::wstring& topButtonTexture, const std::wstring& bottomButtonTexture,
               const std::wstring& scrollTopTexture, const std::wstring& scrollBottomTexture,
               const std::wstring& scrollCenterTexture, WidgetNode *node, Widget* parent = 0);

    virtual void draw() const;
    virtual void processMain();
    virtual Rect getBoundingRect() const;

    void setWidth(float width);
    void setHeight(float height);

    void setNode(WidgetNode *node);

    virtual void mouseMove(int x, int y);

    virtual void actionPerformed(const Action &action);

private:
    Button m_topButton;
    Button m_bottomButton;
    Button m_leftButton;
    Button m_rightButton;
    Sprite m_scrollVTop;
    Sprite m_scrollVBottom;
    Sprite m_scrollVCenter;
    Sprite m_scrollHTop;
    Sprite m_scrollHBottom;
    Sprite m_scrollHCenter;
    WidgetNode *m_node;

    float vSize, vPosition;
    float hSize, hPosition;
};
}

#endif // RANGERS_SCROLLAREA_H
