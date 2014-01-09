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

#include "OpenSR/ScrollArea.h"
#include "OpenSR/private/ScrollArea_p.h"

#include "OpenSR/WidgetNode.h"
#include "OpenSR/Action.h"
#include "OpenSR/Engine.h"

namespace
{
const float SCROLL_INDENT = 5.0f;
}

namespace Rangers
{
ScrollAreaPrivate::ScrollAreaPrivate()
{
    scrollDrag = ScrollAreaPrivate::NONE;
    vPosition = 0.0f;
    hPosition = 0.0f;
    hSize = 0.0f;
    vSize = 0.0f;
}

ScrollAreaPrivate::ScrollAreaListener::ScrollAreaListener(ScrollArea *owner): scrollArea(owner)
{
}

void ScrollAreaPrivate::ScrollAreaListener::actionPerformed(const Action &action)
{
    ScrollAreaPrivate *d = scrollArea->d_func();
    if (action.source().get() == scrollArea)
    {
        switch (action.type())
        {
        case Action::MOUSE_LEAVE:
            d->scrollDrag = ScrollAreaPrivate::NONE;
            break;
        case Action::MOUSE_DOWN:
        {
            uint8_t key = boost::get<uint8_t>(action.argument());
            //TODO: Port wheel to SDL 2.0
            /*if (key == SDL_BUTTON_WHEELUP)
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
            }*/
        }
        break;
        }
    }
    else if (d->vScroll == action.source() || d->hScroll == action.source())
    {
        switch (action.type())
        {
        case Action::MOUSE_DOWN:
        {
            if (d->leftMouseButtonPressed)
            {
                if (d->vScroll == action.source())
                {
                    d->scrollDrag = ScrollAreaPrivate::VERTICAL;
                    d->scrollStart = d->lastMousePosition.y;
                }
                else if (d->hScroll == action.source())
                {
                    d->scrollDrag = ScrollAreaPrivate::HORIZONTAL;
                    d->scrollStart = d->lastMousePosition.x;
                }
            }
        }
        break;
        case Action::MOUSE_UP:
        {
            if (!d->leftMouseButtonPressed)
            {
                if (d->scrollDrag != ScrollAreaPrivate::NONE)
                {
                    boost::shared_ptr<Button> scroll;
                    if (d->scrollDrag == ScrollAreaPrivate::VERTICAL)
                        scroll = d->vScroll;
                    if (d->scrollDrag == ScrollAreaPrivate::HORIZONTAL)
                        scroll = d->hScroll;
                    if (scroll && (!scroll->containsPoint(scroll->mapFromParent(d->lastMousePosition))))
                    {
                        scroll->action(Action(scroll, Action::MOUSE_LEAVE));
                        d->currentChild = boost::weak_ptr<Widget>();
                    }
                    d->scrollDrag = ScrollAreaPrivate::NONE;
                }
            }
        }
        case Action::MOUSE_LEAVE:
            d->scrollDrag = ScrollAreaPrivate::NONE;
            break;
        }
    }
    else if (action.type() == Action::BUTTON_CLICKED)
    {
        if (action.source() == d->left)
            d->hPosition += 0.1f;
        else if (action.source() == d->right)
            d->hPosition -= 0.1f;
        else if (action.source() == d->top)
            d->vPosition += 0.1f;
        else if (action.source() == d->bottom)
            d->vPosition -= 0.1f;
        d->updateScrollPosition();
    }
}

ScrollArea::ScrollArea(const ScrollBarStyle& style, boost::shared_ptr<WidgetNode> node):
    Widget(*(new ScrollAreaPrivate()))
{
    RANGERS_D(ScrollArea);

    d->scrollAreaListener = boost::shared_ptr<ScrollAreaPrivate::ScrollAreaListener>(new ScrollAreaPrivate::ScrollAreaListener(this));

    d->scrollDrag = ScrollAreaPrivate::NONE;
    d->top = boost::shared_ptr<Button>(new Button(style.upButton));
    d->top->addListener(d->scrollAreaListener);
    addWidget(d->top);

    d->bottom = boost::shared_ptr<Button>(new Button(style.downButton));
    d->bottom->addListener(d->scrollAreaListener);
    addWidget(d->bottom);

    d->left = boost::shared_ptr<Button>(new Button(style.upButton));
    d->left->setRotation(90);
    d->left->addListener(d->scrollAreaListener);
    addWidget(d->left);

    d->right = boost::shared_ptr<Button>(new Button(style.downButton));
    d->right->setRotation(90);
    d->right->addListener(d->scrollAreaListener);
    addWidget(d->right);

    d->vScroll = boost::shared_ptr<Button>(new Button(style.scroll));
    addWidget(d->vScroll);

    d->hScroll = boost::shared_ptr<Button>(new Button(style.scroll));
    d->hScroll->setRotation(90);
    addWidget(d->hScroll);

    addListener(d->scrollAreaListener);
    d->vScroll->addListener(d->scrollAreaListener);
    d->hScroll->addListener(d->scrollAreaListener);

    setNode(node);
}

ScrollArea::ScrollArea():
    Widget(*(new ScrollAreaPrivate()))
{
}

ScrollArea::ScrollArea(ScrollAreaPrivate &p):
    Widget(p)
{
    RANGERS_D(ScrollArea);
    d->left = boost::shared_ptr<Button>(new Button());
    d->right = boost::shared_ptr<Button>(new Button());
    d->top = boost::shared_ptr<Button>(new Button());
    d->bottom = boost::shared_ptr<Button>(new Button());
    d->vScroll = boost::shared_ptr<Button>(new Button());
    d->hScroll = boost::shared_ptr<Button>(new Button());
}

void ScrollArea::draw() const
{
    RANGERS_D(const ScrollArea);
    if (!prepareDraw())
        return;

    Rect screenRect = mapToScreen(Rect(0, 0, d->width, d->height));
    glEnable(GL_SCISSOR_TEST);
    /*if (d->hSize > 1.0f)
    {
        screenRect.y += d->left->width();
        screenRect.height -= d->left->width();
    }
    if (d->vSize > 1.0f)
    {
        screenRect.width -= d->top->width();
    }*/
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

void ScrollArea::setNode(boost::shared_ptr<WidgetNode> node)
{
    RANGERS_D(ScrollArea);
    if (d->node)
        removeWidget(d->node);
    d->node = node;
    if (d->node)
        addWidget(d->node);
    markToUpdate();
}

void ScrollArea::processMain()
{
    Widget::processMain();

    RANGERS_D(ScrollArea);
    d->vSize = 0.0f;
    d->hSize = 0.0f;
    if (d->node)
    {
        Rect nodeBB = d->node->boundingRect();
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
            d->left->setPosition(0, d->height + d->left->width() + SCROLL_INDENT);
            d->right->setPosition(d->width - d->right->height(), d->height + d->left->width() + SCROLL_INDENT);
            scrollSize = (d->width - d->right->height() - d->left->height()) / (d->hSize);
            d->hScroll->setHeight(scrollSize);
        }
        if (d->vSize > 1.0f)
        {
            float scrollSize;

            d->top->setPosition(d->width + SCROLL_INDENT, 0);
            d->bottom->setPosition(d->width + SCROLL_INDENT, d->height - d->bottom->height());

            scrollSize = (d->height - d->bottom->height() - d->top->height()) / (d->vSize);

            d->vScroll->setHeight(scrollSize);
        }
        d->hScroll->setPosition(d->left->height(), d->height + d->left->width() + SCROLL_INDENT);
        d->vScroll->setPosition(d->width + SCROLL_INDENT, d->top->height());
    }
}

bool ScrollArea::containsPoint(const Vector &p) const
{
    RANGERS_D(const ScrollArea);
    if (d->scrollDrag == ScrollAreaPrivate::NONE)
    {
        return Widget::containsPoint(p);
    }
    else
    {
        return true;
    }
}

Rect ScrollArea::boundingRect() const
{
    RANGERS_D(const ScrollArea);
    Rect r;
    r.x = 0.0f;
    r.y = 0.0f;
    if (d->vSize > 1.0f)
        r.width = d->width + d->top->width() + SCROLL_INDENT;
    else
        r.width = d->width;
    if (d->hSize > 1.0f)
        r.height = d->height + d->left->width() + SCROLL_INDENT;
    else
        r.height = d->height;
    return r;
}

void ScrollArea::mouseMove(const Vector &p)
{
    lock();
    RANGERS_D(ScrollArea);
    if (!d->node)
        return;

    d->lastMousePosition = p;

    if (d->scrollDrag == ScrollAreaPrivate::NONE)
    {
        boost::shared_ptr<Widget> currentChild = d->currentChild.lock();
        if ((p.x < d->width) && (p.y < d->height))
        {
            if (currentChild != d->node)
            {
                if (currentChild)
                    currentChild->action(Action(currentChild, Action::MOUSE_LEAVE));

                d->currentChild = d->node;
                d->node->action(Action(d->node, Action::MOUSE_ENTER));
            }
            d->node->mouseMove(d->node->mapFromParent(p));
        }
        else
        {
            for (std::list<boost::shared_ptr<Widget> >::reverse_iterator i = d->childWidgets.rbegin(); i != d->childWidgets.rend(); ++i)
            {
                if ((*i) == d->node)
                    continue;
                if ((*i)->containsPoint((*i)->mapFromParent(p)))
                {
                    if ((*i) != currentChild)
                    {
                        if (currentChild)
                            currentChild->action(Action(currentChild, Action::MOUSE_LEAVE));
                        d->currentChild = *i;
                        (*i)->action(Action(*i, Action::MOUSE_ENTER));
                    }
                    (*i)->mouseMove((*i)->mapFromParent(p));
                    unlock();
                    return;
                }
            }
            if (currentChild)
                currentChild->action(Action(currentChild, Action::MOUSE_LEAVE));
            d->currentChild = boost::weak_ptr<Widget>();
        }
    }
    else if (d->scrollDrag == ScrollAreaPrivate::VERTICAL)
    {
        float dy = p.y - d->scrollStart;
        float scrollSize;
        scrollSize = (d->height - d->bottom->height() - d->top->height()) / (d->vSize);
        d->vPosition -= dy / scrollSize;
        d->updateScrollPosition();
        d->scrollStart = p.y;
    }
    else if (d->scrollDrag == ScrollAreaPrivate::HORIZONTAL)
    {
        float dx = p.x - d->scrollStart;
        float scrollSize;
        scrollSize = (d->width - d->right->height() - d->left->height()) / (d->hSize);
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
        scrollSize = (width - right->height() - left->height()) / (hSize);
        hScroll->setPosition((int)(- hPosition * scrollSize + left->height()), height + left->width() + SCROLL_INDENT);
    }
    if (vSize > 1.0f)
    {
        float scrollSize;
        scrollSize = (height - bottom->height() - top->height()) / (vSize);
        vScroll->setPosition(width + SCROLL_INDENT, (int)(- vPosition * scrollSize + top->height()));
    }

    node->setPosition((int)(hPosition * width), (int)(vPosition * height));
    q->unlock();
}

boost::shared_ptr<WidgetNode> ScrollArea::node() const
{
    RANGERS_D(const ScrollArea);
    return d->node;
}
}