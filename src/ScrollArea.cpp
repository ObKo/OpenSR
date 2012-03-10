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
#include "Engine.h"

namespace Rangers
{
ScrollArea::ScrollArea(const ScrollBarStyle& style, WidgetNode *node, Widget *parent):
    Widget(parent), m_node(0), m_scrollDrag(NONE)
{
    m_top = Button(style.upButton, this);
    m_top.addListener(this);
    m_bottom = Button(style.downButton, this);
    m_bottom.addListener(this);

    m_left = Button(style.upButton, this);
    m_left.setRotation(90);
    m_left.addListener(this);
    m_right = Button(style.downButton, this);
    m_right.setRotation(90);
    m_right.addListener(this);

    m_vScroll = Button(style.scroll, this);
    m_hScroll = Button(style.scroll, this);
    m_hScroll.setRotation(90);

    m_vPosition = 0.0f;
    m_hPosition = 0.0f;
    m_hSize = 0.0f;
    m_vSize = 0.0f;
    setNode(node);
}

void ScrollArea::draw() const
{
    prepareDraw();
    Rect screenRect = mapToScreen(Rect(0, 0, m_width, m_height));
    glEnable(GL_SCISSOR_TEST);
    glScissor(screenRect.x1, screenRect.y1 + m_left.width(), screenRect.x2 - screenRect.x1 - m_top.width(), screenRect.y2 - screenRect.y1 - m_left.width());
    if (m_node)
        m_node->draw();
    glDisable(GL_SCISSOR_TEST);
    if (m_hSize > 1.0f)
    {
        m_hScroll.draw();
        m_left.draw();
        m_right.draw();
    }
    if (m_vSize > 1.0f)
    {
        m_vScroll.draw();
        m_top.draw();
        m_bottom.draw();
    }
    endDraw();
}

void ScrollArea::setNode(WidgetNode* node)
{
    if (m_node)
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
    m_vSize = 0.0f;
    m_hSize = 0.0f;
    if (m_node)
    {
        Rect nodeBB = m_node->getBoundingRect();
        if (nodeBB.x2 - nodeBB.x1 > m_width)
        {
            m_hSize = (nodeBB.x2 - nodeBB.x1) / m_width;
        }
        if (nodeBB.y2 - nodeBB.y1 > m_width)
        {
            m_vSize = (nodeBB.y2 - nodeBB.y1) / m_height;
        }
        if (m_hSize > 1.0f)
        {
            float scrollSize;
            if (m_vSize > 1.0f)
            {
                m_left.setPosition(0, m_height);
                m_right.setPosition(m_width - m_right.height() - m_bottom.width(), m_height);
                scrollSize = (m_width - m_bottom.width() - m_right.height() - m_left.height()) / (m_hSize);
            }
            else
            {
                m_left.setPosition(0, m_height);
                m_right.setPosition(m_width - m_right.height(), m_height);
                scrollSize = (m_width - m_right.height() - m_left.height()) / (m_hSize);
            }
            m_hScroll.setHeight(scrollSize);
        }
        if (m_vSize > 1.0f)
        {
            float scrollSize;
            if (m_hSize > 1.0f)
            {
                m_top.setPosition(m_width - m_top.width(), 0);
                m_bottom.setPosition(m_width - m_top.width(), m_height - m_right.width() - m_bottom.height());

                scrollSize = (m_height - m_right.width() - m_bottom.height() - m_top.height()) / (m_vSize);
            }
            else
            {
                m_top.setPosition(m_width - m_top.width(), 0);
                m_bottom.setPosition(m_width - m_top.width(), m_height - m_bottom.height());

                scrollSize = (m_height - m_bottom.height() - m_top.height()) / (m_vSize);
            }
            m_vScroll.setHeight(scrollSize);
        }
        m_hScroll.setPosition(m_left.height(), m_height);
        m_vScroll.setPosition(m_width - m_top.width(), m_top.height());
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
    if (m_scrollDrag == NONE)
    {
        return Rect(0, 0, m_width, m_height);
    }
    else
    {
        //FIXME: dirty hack
        Vector topLeft = mapFromScreen(Vector(0, 0));
        Vector bottomRight = mapFromScreen(Vector(Engine::instance()->screenWidth(), Engine::instance()->screenHeight()));
        return Rect(std::min(topLeft.x, bottomRight.x), std::min(topLeft.y, bottomRight.y), std::max(topLeft.x, bottomRight.x), std::max(topLeft.y, bottomRight.y));
    }
}

void ScrollArea::mouseMove(const Vector &p)
{
    lock();
    if (!m_node)
        return;

    if (m_scrollDrag == NONE)
    {
        if ((p.x < m_width - m_top.width()) && (p.y < m_height - m_left.width()))
        {
            if (m_currentChild)
                m_currentChild->mouseLeave();
            m_node->mouseMove(m_node->mapFromParent(p));
        }
        else
        {
            for (std::list<Widget*>::reverse_iterator i = m_childWidgets.rbegin(); i != m_childWidgets.rend(); ++i)
            {
                if ((*i) == m_node)
                    continue;
                Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
                if (bb.contains(p))
                {
                    if ((*i) != m_currentChild)
                    {
                        if (m_currentChild)
                            m_currentChild->mouseLeave();
                        m_currentChild = *i;
                        m_currentChild->mouseEnter();
                    }
                    (*i)->mouseMove((*i)->mapFromParent(p));
                    unlock();
                    return;
                }
            }
            if (m_currentChild)
                m_currentChild->mouseLeave();
            m_currentChild = 0;
        }
    }
    else if (m_scrollDrag == VERTICAL)
    {
        float dy = p.y - m_scrollStart;
        float scrollSize;
        if (m_hSize > 1.0f)
        {
            scrollSize = (m_height - m_right.width() - m_bottom.height() - m_top.height()) / (m_vSize);
        }
        else
        {
            scrollSize = (m_height - m_bottom.height() - m_top.height()) / (m_vSize);
        }
        m_vPosition -= dy / scrollSize;
        updateScrollPosition();
        m_scrollStart = p.y;
    }
    else if (m_scrollDrag == HORIZONTAL)
    {
        float dx = p.x - m_scrollStart;
        float scrollSize;
        if (m_vSize > 1.0f)
        {
            scrollSize = (m_width - m_bottom.width() - m_right.height() - m_left.height()) / (m_hSize);
        }
        else
        {
            scrollSize = (m_width - m_right.height() - m_left.height()) / (m_hSize);
        }
        m_hPosition -= dx / scrollSize;
        updateScrollPosition();
        m_scrollStart = p.x;
    }
    unlock();
}

void ScrollArea::mouseEnter()
{
    Widget::mouseEnter();
}

void ScrollArea::mouseLeave()
{
    m_scrollDrag = NONE;
    Widget::mouseLeave();
}

void ScrollArea::mouseDown(uint8_t key, const Vector &p)
{
    Widget::mouseDown(key, p);
    if (key == SDL_BUTTON_WHEELUP)
    {
        m_vPosition += 10.0f / m_height;
        updateScrollPosition();
    }
    else if (key == SDL_BUTTON_WHEELDOWN)
    {
        m_vPosition -= 10.0f / m_height;
        updateScrollPosition();
    }
    else if (m_leftMouseButtonPressed)
    {
        if (m_vScroll.mapToParent(m_vScroll.getBoundingRect()).contains(p))
        {
            m_scrollDrag = VERTICAL;
            m_scrollStart = p.y;
        }
        else if (m_hScroll.mapToParent(m_hScroll.getBoundingRect()).contains(p))
        {
            m_scrollDrag = HORIZONTAL;
            m_scrollStart = p.x;
        }
    }
}

void ScrollArea::mouseUp(uint8_t key, const Vector &p)
{
    if (m_leftMouseButtonPressed)
    {
        Widget::mouseUp(key, p);
        if (m_scrollDrag != NONE)
        {
            if (!getBoundingRect().contains(p))
                mouseLeave();
            Button *scroll = 0;
            if (m_scrollDrag == VERTICAL)
                scroll = &m_vScroll;
            if (m_scrollDrag == HORIZONTAL)
                scroll = &m_hScroll;
            if ((scroll) && (!scroll->mapToParent(scroll->getBoundingRect()).contains(p)))
            {
                scroll->mouseLeave();
                m_currentChild = 0;
            }
            m_scrollDrag = NONE;
        }
    }
    else
        Widget::mouseUp(key, p);
}

void ScrollArea::updateScrollPosition()
{
    if (m_vPosition >= 0.0f)
        m_vPosition = 0.0f;
    if (m_vPosition < - m_vSize + 1.0f)
        m_vPosition = - m_vSize + 1.0f;
    if (m_hPosition < - m_hSize + 1.0f)
        m_hPosition = - m_hSize + 1.0f;
    if (m_hPosition >= 0.0f)
        m_hPosition = 0.0f;

    if (m_hSize > 1.0f)
    {
        float scrollSize;
        if (m_vSize > 1.0f)
        {
            scrollSize = (m_width - m_bottom.width() - m_right.height() - m_left.height()) / (m_hSize);
        }
        else
        {
            scrollSize = (m_width - m_right.height() - m_left.height()) / (m_hSize);
        }
        m_hScroll.setPosition((int)(-m_hPosition * scrollSize + m_left.height()), m_height);

    }
    if (m_vSize > 1.0f)
    {
        float scrollSize;
        if (m_hSize > 1.0f)
        {
            scrollSize = (m_height - m_right.width() - m_bottom.height() - m_top.height()) / (m_vSize);
        }
        else
        {
            scrollSize = (m_height - m_bottom.height() - m_top.height()) / (m_vSize);
        }
        m_vScroll.setPosition(m_width - m_top.width(), (int)(-m_vPosition * scrollSize + m_top.height()));
    }

    m_node->setPosition((int)(m_hPosition * m_width), (int)(m_vPosition * m_height));
}

void ScrollArea::actionPerformed(const Action& action)
{
    if (action.type() != Rangers::Action::BUTTON_CLICKED)
        return;

    if (!m_node)
        return;

    if (action.source() == &m_left)
        m_hPosition += 0.1f;
    else if (action.source() == &m_right)
        m_hPosition -= 0.1f;
    else if (action.source() == &m_top)
        m_vPosition += 0.1f;
    else if (action.source() == &m_bottom)
        m_vPosition -= 0.1f;

    updateScrollPosition();
}
}