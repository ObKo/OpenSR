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
    node = 0;
    scrollDrag = ScrollAreaPrivate::NONE;
    vPosition = 0.0f;
    hPosition = 0.0f;
    hSize = 0.0f;
    vSize = 0.0f;
    bottom = 0;
    top = 0;
    left = 0;
    right = 0;
    vScroll = 0;
    hScroll = 0;
}

ScrollArea::ScrollArea(const ScrollBarStyle& style, WidgetNode *node, Widget *parent):
    Widget(*(new ScrollAreaPrivate()), parent)
{
    RANGERS_D(ScrollArea);

    d->scrollDrag = ScrollAreaPrivate::NONE;
    d->top = new Button(style.upButton, this);
    d->top->addListener(this);
    d->bottom = new Button(style.downButton, this);
    d->bottom->addListener(this);

    d->left = new Button(style.upButton, this);
    d->left->setRotation(90);
    d->left->addListener(this);
    d->right = new Button(style.downButton, this);
    d->right->setRotation(90);
    d->right->addListener(this);

    d->vScroll = new Button(style.scroll, this);
    d->hScroll = new Button(style.scroll, this);
    d->hScroll->setRotation(90);

    setNode(node);
}

ScrollArea::ScrollArea(Widget *parent):
    Widget(*(new ScrollAreaPrivate()), parent)
{
}

ScrollArea::ScrollArea(ScrollAreaPrivate &p, Widget *parent):
    Widget(p, parent)
{
    RANGERS_D(ScrollArea);
    d->left = new Button(this);
    d->right = new Button(this);
    d->top = new Button(this);
    d->bottom = new Button(this);
    d->vScroll = new Button(this);
    d->hScroll = new Button(this);
}

void ScrollArea::draw() const
{
    RANGERS_D(const ScrollArea);
    if (!prepareDraw())
        return;

    Rect screenRect = mapToScreen(Rect(0, 0, d->width, d->height));
    glEnable(GL_SCISSOR_TEST);
    if (d->hSize > 1.0f)
    {
        screenRect.y += d->left->width();
        screenRect.height -= d->left->width();
    }
    if (d->vSize > 1.0f)
    {
        screenRect.width -= d->top->width();
    }
    glScissor(screenRect.x, screenRect.y, screenRect.width, screenRect.height);
    if (d->node)
        d->node->draw();
    glDisable(GL_SCISSOR_TEST);
    if (d->hSize > 1.0f)
    {
        d->hScroll->draw();
        d->left->draw();
        d->right->draw();
    }
    if (d->vSize > 1.0f)
    {
        d->vScroll->draw();
        d->top->draw();
        d->bottom->draw();
    }
    endDraw();
}

void ScrollArea::setNode(WidgetNode* node)
{
    RANGERS_D(ScrollArea);
    if (d->node)
        removeWidget(d->node);
    d->node = node;
    if (d->node)
    {
        addWidget(d->node);
    }
    markToUpdate();
}

void ScrollArea::processMain()
{
    RANGERS_D(ScrollArea);
    d->vSize = 0.0f;
    d->hSize = 0.0f;
    if (d->node)
    {
        Rect nodeBB = d->node->getBoundingRect();
        if (nodeBB.width > d->width)
        {
            d->hSize = nodeBB.width / d->width;
        }
        if (nodeBB.height > d->height)
        {
            d->vSize = nodeBB.height / d->height;
        }
        if (d->hSize > 1.0f)
        {
            float scrollSize;
            if (d->vSize > 1.0f)
            {
                d->left->setPosition(0, d->height);
                d->right->setPosition(d->width - d->right->height() - d->bottom->width(), d->height);
                scrollSize = (d->width - d->bottom->width() - d->right->height() - d->left->height()) / (d->hSize);
            }
            else
            {
                d->left->setPosition(0, d->height);
                d->right->setPosition(d->width - d->right->height(), d->height);
                scrollSize = (d->width - d->right->height() - d->left->height()) / (d->hSize);
            }
            d->hScroll->setHeight(scrollSize);
        }
        if (d->vSize > 1.0f)
        {
            float scrollSize;
            if (d->hSize > 1.0f)
            {
                d->top->setPosition(d->width - d->top->width(), 0);
                d->bottom->setPosition(d->width - d->top->width(), d->height - d->right->width() - d->bottom->height());

                scrollSize = (d->height - d->right->width() - d->bottom->height() - d->top->height()) / (d->vSize);
            }
            else
            {
                d->top->setPosition(d->width - d->top->width(), 0);
                d->bottom->setPosition(d->width - d->top->width(), d->height - d->bottom->height());

                scrollSize = (d->height - d->bottom->height() - d->top->height()) / (d->vSize);
            }
            d->vScroll->setHeight(scrollSize);
        }
        d->hScroll->setPosition(d->left->height(), d->height);
        d->vScroll->setPosition(d->width - d->top->width(), d->top->height());
    }
}

void ScrollArea::setWidth(float width)
{
    RANGERS_D(ScrollArea);
    d->width = width;
    markToUpdate();
}

void ScrollArea::setHeight(float height)
{
    RANGERS_D(ScrollArea);
    d->height = height;
    markToUpdate();
}

Rect ScrollArea::getBoundingRect() const
{
    RANGERS_D(const ScrollArea);
    if (d->scrollDrag == ScrollAreaPrivate::NONE)
    {
        return Rect(0, 0, d->width, d->height);
    }
    else
    {
        //FIXME: dirty hack
        Vector topLeft = mapFromScreen(Vector(0, 0));
        Vector bottomRight = mapFromScreen(Vector(Engine::instance().screenWidth(), Engine::instance().screenHeight()));
        return Rect(std::min(topLeft.x, bottomRight.x), std::min(topLeft.y, bottomRight.y), std::max(topLeft.x, bottomRight.x), std::max(topLeft.y, bottomRight.y));
    }
}

void ScrollArea::mouseMove(const Vector &p)
{
    lock();
    RANGERS_D(ScrollArea);
    if (!d->node)
        return;

    if (d->scrollDrag == ScrollAreaPrivate::NONE)
    {
        if ((p.x < d->width - d->top->width()) && (p.y < d->height - d->left->width()))
        {
            if (d->currentChild)
                d->currentChild->mouseLeave();
            d->node->mouseMove(d->node->mapFromParent(p));
        }
        else
        {
            for (std::list<Widget*>::reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); ++i)
            {
                if ((*i) == d->node)
                    continue;
                Rect bb = (*i)->mapToParent((*i)->getBoundingRect());
                if (bb.contains(p))
                {
                    if ((*i) != d->currentChild)
                    {
                        if (d->currentChild)
                            d->currentChild->mouseLeave();
                        d->currentChild = *i;
                        d->currentChild->mouseEnter();
                    }
                    (*i)->mouseMove((*i)->mapFromParent(p));
                    unlock();
                    return;
                }
            }
            if (d->currentChild)
                d->currentChild->mouseLeave();
            d->currentChild = 0;
        }
    }
    else if (d->scrollDrag == ScrollAreaPrivate::VERTICAL)
    {
        float dy = p.y - d->scrollStart;
        float scrollSize;
        if (d->hSize > 1.0f)
        {
            scrollSize = (d->height - d->right->width() - d->bottom->height() - d->top->height()) / (d->vSize);
        }
        else
        {
            scrollSize = (d->height - d->bottom->height() - d->top->height()) / (d->vSize);
        }
        d->vPosition -= dy / scrollSize;
        d->updateScrollPosition();
        d->scrollStart = p.y;
    }
    else if (d->scrollDrag == ScrollAreaPrivate::HORIZONTAL)
    {
        float dx = p.x - d->scrollStart;
        float scrollSize;
        if (d->vSize > 1.0f)
        {
            scrollSize = (d->width - d->bottom->width() - d->right->height() - d->left->height()) / (d->hSize);
        }
        else
        {
            scrollSize = (d->width - d->right->height() - d->left->height()) / (d->hSize);
        }
        d->hPosition -= dx / scrollSize;
        d->updateScrollPosition();
        d->scrollStart = p.x;
    }
    unlock();
}

void ScrollArea::processLogic(int dt)
{
    RANGERS_D(ScrollArea);
    if (d->node)
        d->node->processLogic(dt);
}

void ScrollArea::mouseEnter()
{
    Widget::mouseEnter();
}

void ScrollArea::mouseLeave()
{
    RANGERS_D(ScrollArea);
    d->scrollDrag = ScrollAreaPrivate::NONE;
    Widget::mouseLeave();
}

void ScrollArea::mouseDown(uint8_t key, const Vector &p)
{
    Widget::mouseDown(key, p);
    lock();
    RANGERS_D(ScrollArea);
    if (key == SDL_BUTTON_WHEELUP)
    {
        if (d->vSize > 1.0f)
        {
            d->vPosition += 0.1f;
            d->updateScrollPosition();
        }
    }
    else if (key == SDL_BUTTON_WHEELDOWN)
    {
        if (d->vSize > 1.0f)
        {
            d->vPosition -= 0.1f;
            d->updateScrollPosition();
        }
    }
    else if (d->leftMouseButtonPressed)
    {
        if (d->vScroll->mapToParent(d->vScroll->getBoundingRect()).contains(p))
        {
            d->scrollDrag = ScrollAreaPrivate::VERTICAL;
            d->scrollStart = p.y;
        }
        else if (d->hScroll->mapToParent(d->hScroll->getBoundingRect()).contains(p))
        {
            d->scrollDrag = ScrollAreaPrivate::HORIZONTAL;
            d->scrollStart = p.x;
        }
        else
        {
            if (d->node)
                d->node->mouseDown(key, d->node->mapFromParent(p));
        }
    }
    unlock();
}

void ScrollArea::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(ScrollArea);
    if (d->leftMouseButtonPressed)
    {
        if (d->scrollDrag != ScrollAreaPrivate::NONE)
        {
            if (!getBoundingRect().contains(p))
                mouseLeave();
            Button *scroll = 0;
            if (d->scrollDrag == ScrollAreaPrivate::VERTICAL)
                scroll = d->vScroll;
            if (d->scrollDrag == ScrollAreaPrivate::HORIZONTAL)
                scroll = d->hScroll;
            if ((scroll) && (!scroll->mapToParent(scroll->getBoundingRect()).contains(p)))
            {
                scroll->mouseLeave();
                d->currentChild = 0;
            }
            d->scrollDrag = ScrollAreaPrivate::NONE;
        }
        else
        {
            if (d->node)
                d->node->mouseUp(key, d->node->mapFromParent(p));
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
    if (vPosition >= 0.0f)
        vPosition = 0.0f;
    if (vPosition < - vSize + 1.0f)
        vPosition = - vSize + 1.0f;
    if (hPosition < - hSize + 1.0f)
        hPosition = - hSize + 1.0f;
    if (hPosition >= 0.0f)
        hPosition = 0.0f;

    if (hSize > 1.0f)
    {
        float scrollSize;
        if (vSize > 1.0f)
        {
            scrollSize = (width - bottom->width() - right->height() - left->height()) / (hSize);
        }
        else
        {
            scrollSize = (width - right->height() - left->height()) / (hSize);
        }
        hScroll->setPosition((int)(- hPosition * scrollSize + left->height()), height);

    }
    if (vSize > 1.0f)
    {
        float scrollSize;
        if (hSize > 1.0f)
        {
            scrollSize = (height - right->width() - bottom->height() - top->height()) / (vSize);
        }
        else
        {
            scrollSize = (height - bottom->height() - top->height()) / (vSize);
        }
        vScroll->setPosition(width - top->width(), (int)(- vPosition * scrollSize + top->height()));
    }

    node->setPosition((int)(hPosition * width), (int)(vPosition * height));
    q->unlock();
}

void ScrollArea::actionPerformed(const Action& action)
{
    RANGERS_D(ScrollArea);
    if (action.type() != Rangers::Action::BUTTON_CLICKED)
        return;

    if (!d->node)
        return;

    lock();
    if (action.source() == d->left)
        d->hPosition += 0.1f;
    else if (action.source() == d->right)
        d->hPosition -= 0.1f;
    else if (action.source() == d->top)
        d->vPosition += 0.1f;
    else if (action.source() == d->bottom)
        d->vPosition -= 0.1f;

    d->updateScrollPosition();
    unlock();
}
}