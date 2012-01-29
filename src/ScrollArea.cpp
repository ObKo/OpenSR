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

#include "ScrollArea.h"
#include "WidgetNode.h"
#include "Action.h"

namespace Rangers
{
ScrollArea::ScrollArea(const std::wstring& topButtonTexture, const std::wstring& bottomButtonTexture,
                       const std::wstring& scrollTopTexture, const std::wstring& scrollBottomTexture,
                       const std::wstring& scrollCenterTexture, WidgetNode *node, Widget* parent): 
                       Widget(parent), m_node(0)
{
    m_topButton = Button(topButtonTexture, this);
    m_topButton.addListener(this);
    m_bottomButton = Button(bottomButtonTexture, this);
    m_bottomButton.addListener(this);
    m_leftButton = m_topButton;
    m_leftButton.setRotation(90);
    m_rightButton = m_bottomButton;
    m_rightButton.setRotation(90);
    m_scrollVTop = Sprite(scrollTopTexture, this);
    m_scrollVBottom = Sprite(scrollBottomTexture, this);
    m_scrollVCenter = Sprite(scrollCenterTexture, this);
    m_scrollVCenter.setTextureScaling(TEXTURE_TILE_Y);
    m_scrollHTop = m_scrollVTop;
    m_scrollHTop.setRotation(90);
    m_scrollHBottom = m_scrollVBottom;
    m_scrollHBottom.setRotation(90);
    m_scrollHCenter = m_scrollVCenter;
    m_scrollHCenter.setRotation(90);
    m_scrollVCenter.setTextureScaling(TEXTURE_TILE_Y);
    vPosition = 0.0f;
    hPosition = 0.0f;
    hSize = 0.0f;
    vSize = 0.0f;
	setNode(node);
}

void ScrollArea::draw() const
{
    prepareDraw();
    Rect screenRect = mapToScreen(getBoundingRect());
    glEnable(GL_SCISSOR_TEST);
    glScissor(screenRect.x1, screenRect.y1 + m_leftButton.width(), screenRect.x2 - screenRect.x1 - m_topButton.width(), screenRect.y2 - screenRect.y1 - m_leftButton.width());
    if (m_node)
        m_node->draw();
    glDisable(GL_SCISSOR_TEST);
    if (hSize > 1.0f)
    {
        m_scrollHCenter.draw();
        m_scrollHTop.draw();
        m_scrollHBottom.draw();
        m_leftButton.draw();
        m_rightButton.draw();
    }
    if (vSize > 1.0f)
    {
        m_scrollVCenter.draw();
        m_scrollVTop.draw();
        m_scrollVBottom.draw();
        m_topButton.draw();
        m_bottomButton.draw();
    }
    endDraw();
}

void ScrollArea::setNode(WidgetNode* node)
{
	if(m_node)
        removeWidget(m_node);
    m_node = node;
    if (m_node)
    {
        addWidget(m_node);
    }
    markToUpdate();
}

void ScrollArea::processMain()
{
    vSize = 0.0f;
    hSize = 0.0f;
    if (m_node)
    {
        Rect nodeBB = m_node->getBoundingRect();
        if (nodeBB.x2 - nodeBB.x1 > m_width)
        {
            hSize = (nodeBB.x2 - nodeBB.x1) / m_width;
        }
        if (nodeBB.y2 - nodeBB.y1 > m_width)
        {
            vSize = (nodeBB.y2 - nodeBB.y1) / m_height;
        }
        if (hSize > 1.0f)
        {
            float scrollSize;
            if (vSize > 1.0f)
            {
                m_leftButton.setPosition(0, m_height);
                m_rightButton.setPosition(m_width - m_rightButton.height() - m_bottomButton.width(), m_height);
                scrollSize = (m_width - m_bottomButton.width() - m_rightButton.height() - m_leftButton.height()) / (hSize);
            }
            else
            {
                m_leftButton.setPosition(0, m_height);
                m_rightButton.setPosition(m_width - m_rightButton.height(), m_height);
                scrollSize = (m_width - m_rightButton.height() - m_leftButton.height()) / (hSize);
            }
            m_scrollHCenter.setHeight(scrollSize + 2.0f - m_scrollHBottom.height() - m_scrollHTop.height());
        }
        if (vSize > 1.0f)
        {
            float scrollSize;
            if (hSize > 1.0f)
            {
                m_topButton.setPosition(m_width - m_topButton.width(), 0);
                m_bottomButton.setPosition(m_width - m_topButton.width(), m_height - m_rightButton.width() - m_bottomButton.height());

                scrollSize = (m_height - m_rightButton.width() - m_bottomButton.height() - m_topButton.height()) / (vSize);
            }
            else
            {
                m_topButton.setPosition(m_width - m_topButton.width(), 0);
                m_bottomButton.setPosition(m_width - m_topButton.width(), m_height - m_bottomButton.height());

                scrollSize = (m_height - m_bottomButton.height() - m_topButton.height()) / (vSize);
            }
            m_scrollVCenter.setHeight(scrollSize + 2.0f - m_scrollVBottom.height() - m_scrollVTop.height());
        }
        m_scrollHTop.setPosition(m_leftButton.height(), m_height);
        m_scrollHCenter.setPosition(m_leftButton.height() + m_scrollHTop.height() - 1.0f, m_height);
        m_scrollHBottom.setPosition(m_leftButton.height() + m_scrollHTop.height() + m_scrollHCenter.height() - 1.0f, m_height);
        m_scrollVTop.setPosition(m_width - m_topButton.width(), m_topButton.height());
        m_scrollVCenter.setPosition(m_width - m_topButton.width(), m_topButton.height() + m_scrollVTop.height() - 0.5f);
        m_scrollVBottom.setPosition(m_width - m_topButton.width(), m_topButton.height() + m_scrollVTop.height() + m_scrollVCenter.height() - 1.0f);
    }
}

void ScrollArea::setWidth(float width)
{
    m_width = width;
    markToUpdate();
}

void ScrollArea::setHeight(float height)
{
    m_height = height;
    markToUpdate();
}

Rect ScrollArea::getBoundingRect() const
{
    return Rect(0, 0, m_width, m_height);
}

void ScrollArea::mouseMove(int x, int y)
{
    if (!m_node)
        return;
    if ((x < m_width - m_topButton.width()) && (y < m_height - m_leftButton.width()))
    {
        Vector m = m_node->mapFromParent(Vector(x, y));
        m_node->mouseMove(m.x, m.y);
    }
    else
    {
        for (std::list<Widget*>::reverse_iterator i = m_childWidgets.rbegin(); i != m_childWidgets.rend(); ++i)
        {
            if ((*i) == m_node)
                continue;
            Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
            Vector mouse = Vector(x, y);
            if (bb.contains(mouse))
            {
                if ((*i) != m_currentChild)
                {
                    if (m_currentChild)
                        m_currentChild->mouseLeave();
                    m_currentChild = *i;
                    m_currentChild->mouseEnter();
                }
                Vector m = (*i)->mapFromParent(mouse);
                (*i)->mouseMove(m.x, m.y);
                unlock();
                return;
            }
        }
        if (m_currentChild)
            m_currentChild->mouseLeave();
        m_currentChild = 0;
    }
}

void ScrollArea::actionPerformed(const Action& action)
{
    if (action.type() != Rangers::Action::BUTTON_CLICKED)
        return;

    if (!m_node)
        return;

    if (action.source() == &m_leftButton)
    {
        hPosition += 0.1f;
        if (hPosition >= 0.0f)
            hPosition = 0.0f;
    }
    else if (action.source() == &m_rightButton)
    {
        hPosition -= 0.1f;
        if (hPosition < - hSize + 1.0f)
            hPosition = - hSize + 1.0f;
    }
    else if (action.source() == &m_topButton)
    {
        vPosition += 0.1f;
        if (vPosition >= 0.0f)
            vPosition = 0.0f;
    }
    else if (action.source() == &m_bottomButton)
    {
        vPosition -= 0.1f;
        if (vPosition < - vSize + 1.0f)
            vPosition = - vSize + 1.0f;
    }

    if (hSize > 1.0f)
    {
        float scrollSize;
        if (vSize > 1.0f)
        {
            scrollSize = (m_width - m_bottomButton.width() - m_rightButton.height() - m_leftButton.height()) / (hSize);
        }
        else
        {
            scrollSize = (m_width - m_rightButton.height() - m_leftButton.height()) / (hSize);
        }
        m_scrollHTop.setPosition((int)(-hPosition * scrollSize + m_leftButton.height()), m_height);
        m_scrollHCenter.setPosition((int)(-hPosition * scrollSize + m_leftButton.height() + m_scrollHTop.height() - 1.0f), m_height);
        m_scrollHBottom.setPosition((int)(-hPosition * scrollSize + m_leftButton.height() + m_scrollHTop.height() + m_scrollHCenter.height() - 2.0f), m_height);

    }
    if (vSize > 1.0f)
    {
        float scrollSize;
        if (hSize > 1.0f)
        {
            scrollSize = (m_height - m_rightButton.width() - m_bottomButton.height() - m_topButton.height()) / (vSize);
        }
        else
        {
            scrollSize = (m_height - m_bottomButton.height() - m_topButton.height()) / (vSize);
        }
        m_scrollVTop.setPosition(m_width - m_topButton.width(), (int)(-vPosition * scrollSize + m_topButton.height()));
        m_scrollVCenter.setPosition(m_width - m_topButton.width(), (int)(-vPosition * scrollSize + m_topButton.height() + m_scrollVTop.height() - 1.0f));
        m_scrollVBottom.setPosition(m_width - m_topButton.width(), (int)(-vPosition * scrollSize + m_topButton.height() + m_scrollVTop.height() + m_scrollVCenter.height() - 2.0f));

    }

    m_node->setPosition(hPosition * m_width, vPosition * m_height);
}
}