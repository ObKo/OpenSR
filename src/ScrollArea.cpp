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

#include "private/ScrollArea_p.h"

namespace Rangers
{
ScrollAreaPrivate::ScrollAreaPrivate()
{
    m_node = 0;
    m_scrollDrag = ScrollAreaPrivate::NONE;
    m_vPosition = 0.0f;
    m_hPosition = 0.0f;
    m_hSize = 0.0f;
    m_vSize = 0.0f;
}

ScrollArea::ScrollArea(const ScrollBarStyle& style, WidgetNode *node, Widget *parent):
    Widget(*(new ScrollAreaPrivate()), parent)
{
    RANGERS_D(ScrollArea);

    d->m_scrollDrag = ScrollAreaPrivate::NONE;
    d->m_top = Button(style.upButton, this);
    d->m_top.addListener(this);
    d->m_bottom = Button(style.downButton, this);
    d->m_bottom.addListener(this);

    d->m_left = Button(style.upButton, this);
    d->m_left.setRotation(90);
    d->m_left.addListener(this);
    d->m_right = Button(style.downButton, this);
    d->m_right.setRotation(90);
    d->m_right.addListener(this);

    d->m_vScroll = Button(style.scroll, this);
    d->m_hScroll = Button(style.scroll, this);
    d->m_hScroll.setRotation(90);

    setNode(node);
}

ScrollArea::ScrollArea(Widget *parent):
    Widget(*(new ScrollAreaPrivate()), parent)
{
}

ScrollArea::ScrollArea(ScrollAreaPrivate &p, Widget *parent):
    Widget(p, parent)
{
}

void ScrollArea::draw() const
{
    RANGERS_D(const ScrollArea);
    if (!prepareDraw())
        return;

    Rect screenRect = mapToScreen(Rect(0, 0, d->m_width, d->m_height));
    glEnable(GL_SCISSOR_TEST);
    if (d->m_hSize > 1.0f)
    {
        screenRect.y += d->m_left.width();
        screenRect.height -= d->m_left.width();
    }
    if (d->m_vSize > 1.0f)
    {
        screenRect.width -= d->m_top.width();
    }
    glScissor(screenRect.x, screenRect.y, screenRect.width, screenRect.height);
    if (d->m_node)
        d->m_node->draw();
    glDisable(GL_SCISSOR_TEST);
    if (d->m_hSize > 1.0f)
    {
        d->m_hScroll.draw();
        d->m_left.draw();
        d->m_right.draw();
    }
    if (d->m_vSize > 1.0f)
    {
        d->m_vScroll.draw();
        d->m_top.draw();
        d->m_bottom.draw();
    }
    endDraw();
}

void ScrollArea::setNode(WidgetNode* node)
{
    RANGERS_D(ScrollArea);
    if (d->m_node)
        removeWidget(d->m_node);
    d->m_node = node;
    if (d->m_node)
    {
        addWidget(d->m_node);
    }
    markToUpdate();
}

void ScrollArea::processMain()
{
    RANGERS_D(ScrollArea);
    d->m_vSize = 0.0f;
    d->m_hSize = 0.0f;
    if (d->m_node)
    {
        Rect nodeBB = d->m_node->getBoundingRect();
        if (nodeBB.width > d->m_width)
        {
            d->m_hSize = nodeBB.width / d->m_width;
        }
        if (nodeBB.height > d->m_height)
        {
            d->m_vSize = nodeBB.height / d->m_height;
        }
        if (d->m_hSize > 1.0f)
        {
            float scrollSize;
            if (d->m_vSize > 1.0f)
            {
                d->m_left.setPosition(0, d->m_height);
                d->m_right.setPosition(d->m_width - d->m_right.height() - d->m_bottom.width(), d->m_height);
                scrollSize = (d->m_width - d->m_bottom.width() - d->m_right.height() - d->m_left.height()) / (d->m_hSize);
            }
            else
            {
                d->m_left.setPosition(0, d->m_height);
                d->m_right.setPosition(d->m_width - d->m_right.height(), d->m_height);
                scrollSize = (d->m_width - d->m_right.height() - d->m_left.height()) / (d->m_hSize);
            }
            d->m_hScroll.setHeight(scrollSize);
        }
        if (d->m_vSize > 1.0f)
        {
            float scrollSize;
            if (d->m_hSize > 1.0f)
            {
                d->m_top.setPosition(d->m_width - d->m_top.width(), 0);
                d->m_bottom.setPosition(d->m_width - d->m_top.width(), d->m_height - d->m_right.width() - d->m_bottom.height());

                scrollSize = (d->m_height - d->m_right.width() - d->m_bottom.height() - d->m_top.height()) / (d->m_vSize);
            }
            else
            {
                d->m_top.setPosition(d->m_width - d->m_top.width(), 0);
                d->m_bottom.setPosition(d->m_width - d->m_top.width(), d->m_height - d->m_bottom.height());

                scrollSize = (d->m_height - d->m_bottom.height() - d->m_top.height()) / (d->m_vSize);
            }
            d->m_vScroll.setHeight(scrollSize);
        }
        d->m_hScroll.setPosition(d->m_left.height(), d->m_height);
        d->m_vScroll.setPosition(d->m_width - d->m_top.width(), d->m_top.height());
    }
}

void ScrollArea::setWidth(float width)
{
    RANGERS_D(ScrollArea);
    d->m_width = width;
    markToUpdate();
}

void ScrollArea::setHeight(float height)
{
    RANGERS_D(ScrollArea);
    d->m_height = height;
    markToUpdate();
}

Rect ScrollArea::getBoundingRect() const
{
    RANGERS_D(const ScrollArea);
    if (d->m_scrollDrag == ScrollAreaPrivate::NONE)
    {
        return Rect(0, 0, d->m_width, d->m_height);
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
    RANGERS_D(ScrollArea);
    if (!d->m_node)
        return;

    if (d->m_scrollDrag == ScrollAreaPrivate::NONE)
    {
        if ((p.x < d->m_width - d->m_top.width()) && (p.y < d->m_height - d->m_left.width()))
        {
            if (d->m_currentChild)
                d->m_currentChild->mouseLeave();
            d->m_node->mouseMove(d->m_node->mapFromParent(p));
        }
        else
        {
            for (std::list<Widget*>::reverse_iterator i = d->m_childWidgets.rbegin(); i != d->m_childWidgets.rend(); ++i)
            {
                if ((*i) == d->m_node)
                    continue;
                Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
                if (bb.contains(p))
                {
                    if ((*i) != d->m_currentChild)
                    {
                        if (d->m_currentChild)
                            d->m_currentChild->mouseLeave();
                        d->m_currentChild = *i;
                        d->m_currentChild->mouseEnter();
                    }
                    (*i)->mouseMove((*i)->mapFromParent(p));
                    unlock();
                    return;
                }
            }
            if (d->m_currentChild)
                d->m_currentChild->mouseLeave();
            d->m_currentChild = 0;
        }
    }
    else if (d->m_scrollDrag == ScrollAreaPrivate::VERTICAL)
    {
        float dy = p.y - d->m_scrollStart;
        float scrollSize;
        if (d->m_hSize > 1.0f)
        {
            scrollSize = (d->m_height - d->m_right.width() - d->m_bottom.height() - d->m_top.height()) / (d->m_vSize);
        }
        else
        {
            scrollSize = (d->m_height - d->m_bottom.height() - d->m_top.height()) / (d->m_vSize);
        }
        d->m_vPosition -= dy / scrollSize;
        d->updateScrollPosition();
        d->m_scrollStart = p.y;
    }
    else if (d->m_scrollDrag == ScrollAreaPrivate::HORIZONTAL)
    {
        float dx = p.x - d->m_scrollStart;
        float scrollSize;
        if (d->m_vSize > 1.0f)
        {
            scrollSize = (d->m_width - d->m_bottom.width() - d->m_right.height() - d->m_left.height()) / (d->m_hSize);
        }
        else
        {
            scrollSize = (d->m_width - d->m_right.height() - d->m_left.height()) / (d->m_hSize);
        }
        d->m_hPosition -= dx / scrollSize;
        d->updateScrollPosition();
        d->m_scrollStart = p.x;
    }
    unlock();
}

void ScrollArea::processLogic(int dt)
{
    RANGERS_D(ScrollArea);
    if (d->m_node)
        d->m_node->processLogic(dt);
}

void ScrollArea::mouseEnter()
{
    Widget::mouseEnter();
}

void ScrollArea::mouseLeave()
{
    RANGERS_D(ScrollArea);
    d->m_scrollDrag = ScrollAreaPrivate::NONE;
    Widget::mouseLeave();
}

void ScrollArea::mouseDown(uint8_t key, const Vector &p)
{
    Widget::mouseDown(key, p);
    lock();
    RANGERS_D(ScrollArea);
    if (key == SDL_BUTTON_WHEELUP)
    {
        if (d->m_vSize > 1.0f)
        {
            d->m_vPosition += 0.1f;
            d->updateScrollPosition();
        }
    }
    else if (key == SDL_BUTTON_WHEELDOWN)
    {
        if (d->m_vSize > 1.0f)
        {
            d->m_vPosition -= 0.1f;
            d->updateScrollPosition();
        }
    }
    else if (d->m_leftMouseButtonPressed)
    {
        if (d->m_vScroll.mapToParent(d->m_vScroll.getBoundingRect()).contains(p))
        {
            d->m_scrollDrag = ScrollAreaPrivate::VERTICAL;
            d->m_scrollStart = p.y;
        }
        else if (d->m_hScroll.mapToParent(d->m_hScroll.getBoundingRect()).contains(p))
        {
            d->m_scrollDrag = ScrollAreaPrivate::HORIZONTAL;
            d->m_scrollStart = p.x;
        }
        else
        {
            if (d->m_node)
                d->m_node->mouseDown(key, d->m_node->mapFromParent(p));
        }
    }
    unlock();
}

void ScrollArea::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(ScrollArea);
    if (d->m_leftMouseButtonPressed)
    {
        if (d->m_scrollDrag != ScrollAreaPrivate::NONE)
        {
            if (!getBoundingRect().contains(p))
                mouseLeave();
            Button *scroll = 0;
            if (d->m_scrollDrag == ScrollAreaPrivate::VERTICAL)
                scroll = &d->m_vScroll;
            if (d->m_scrollDrag == ScrollAreaPrivate::HORIZONTAL)
                scroll = &d->m_hScroll;
            if ((scroll) && (!scroll->mapToParent(scroll->getBoundingRect()).contains(p)))
            {
                scroll->mouseLeave();
                d->m_currentChild = 0;
            }
            d->m_scrollDrag = ScrollAreaPrivate::NONE;
        }
        else
        {
            if (d->m_node)
                d->m_node->mouseUp(key, d->m_node->mapFromParent(p));
        }
    }
    else
        Widget::mouseUp(key, p);
    unlock();
}

void ScrollAreaPrivate::updateScrollPosition()
{
    RANGERS_Q(ScrollArea);
    q->lock();
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
        m_hScroll.setPosition((int)(- m_hPosition * scrollSize + m_left.height()), m_height);

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
        m_vScroll.setPosition(m_width - m_top.width(), (int)(- m_vPosition * scrollSize + m_top.height()));
    }

    m_node->setPosition((int)(m_hPosition * m_width), (int)(m_vPosition * m_height));
    q->unlock();
}

void ScrollArea::actionPerformed(const Action& action)
{
    RANGERS_D(ScrollArea);
    if (action.type() != Rangers::Action::BUTTON_CLICKED)
        return;

    if (!d->m_node)
        return;

    lock();
    if (action.source() == &d->m_left)
        d->m_hPosition += 0.1f;
    else if (action.source() == &d->m_right)
        d->m_hPosition -= 0.1f;
    else if (action.source() == &d->m_top)
        d->m_vPosition += 0.1f;
    else if (action.source() == &d->m_bottom)
        d->m_vPosition -= 0.1f;

    d->updateScrollPosition();
    unlock();
}
}