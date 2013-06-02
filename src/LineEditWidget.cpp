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

#include "LineEditWidget.h"
#include <SDL.h>
#include "Engine.h"
#include "Font.h"
#include "ActionListener.h"
#include "Action.h"
#include <boost/variant/get.hpp>
#include "ResourceManager.h"
#include "NinePatch.h"

#include "private/LineEditWidget_p.h"

namespace Rangers
{

void LineEditWidgetPrivate::LineEditWidgetListener::actionPerformed(const Action &action)
{
    boost::shared_ptr<LineEditWidget> q = boost::static_pointer_cast<LineEditWidget>(action.source());
    LineEditWidgetPrivate *d = q->d_func();

    switch (action.type())
    {
    case Action::MOUSE_CLICK:
        d->position = d->label->font()->maxChars(d->text.begin() + d->stringOffset, d->text.end(),
                      d->mousePosition.x/* - style.contentRect.x*/);
        d->updateText();
        q->markToUpdate();
        Engine::instance().focusWidget(q.get());
        break;
    case Action::KEY_PRESSED:
        d->keyPressed(boost::get<SDL_keysym>(action.argument()));
        break;
    }
}

LineEditWidgetPrivate::LineEditWidgetPrivate()
{
    cursorVertices = 0;
    cursorBuffer = 0;
    position = 0;
    cursorVisible = false;
    cursorTime = 0;
    stringOffset = 0;
    lineEditListener = boost::shared_ptr<LineEditWidgetListener>(new LineEditWidgetListener());
}

void LineEditWidget::draw() const
{
    RANGERS_D(const LineEditWidget);
    if (!prepareDraw())
        return;

    if (d->background)
        d->background->draw();

    d->label->draw();

    if (isFocused() && d->cursorVisible && d->cursorBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor3ub((d->style.color >> 24) & 0xff, (d->style.color >> 16) & 0xff, (d->style.color >> 8) & 0xff);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, d->cursorBuffer);

        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
        glLineWidth(1);

        glDrawArrays(GL_LINES, 0, 2);

        glDisableClientState(GL_ARRAY_BUFFER);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    endDraw();
}

LineEditWidget::LineEditWidget(float w, float h, boost::shared_ptr< Font > font, Widget* parent):
    Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    if (!font)
        font = Engine::instance().coreFont();

    d->label = boost::shared_ptr<Label>(new Label(L"", 0, font, POSITION_X_LEFT, POSITION_Y_TOP));
    addChild(d->label);

    d->height = h > font->size() + 4 ? h : font->size() + 4;
    d->width = w;
    d->style.color = 0xffffffff;
    d->init();
}

LineEditWidget::LineEditWidget(Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
}

LineEditWidget::LineEditWidget(const LineEditStyle& style, Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    d->style = style;
    d->init();
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
}

void LineEditWidgetPrivate::init()
{
    RANGERS_Q(LineEditWidget);
    if (style.background.type == ResourceDescriptor::NINEPATCH)
    {
        background = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(style.background.resource)));
        q->addChild(background);
    }
    else if (style.background.type == ResourceDescriptor::SPRITE)
    {
        background = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(style.background.resource)));
        q->addChild(background);
    }
    if ((style.font.path != L"") && (style.font.size > 0))
    {
        label = boost::shared_ptr<Label>(new Label(text, 0, ResourceManager::instance().loadFont(style.font.path, style.font.size)));
        label->setColor(((style.color >> 24) & 0xff) / 255.0f, ((style.color >> 16) & 0xff) / 255.0f, ((style.color >> 8) & 0xff) / 255.0f, ((style.color) & 0xff) / 255.0f);
        q->addChild(label);
    }
    else
    {
        if (!label)
        {
            label = boost::shared_ptr<Label>(new Label());
            q->addChild(label);
        }
    }
    if (style.contentRect.valid() && label->font() && background)
    {
        height = std::max(background->normalHeight() - style.contentRect.height + label->font()->size(), background->normalHeight());
        width = std::max(background->normalWidth(), style.contentRect.width);
    }
    else if (label->font() && background)
    {
        height = std::max(background->normalHeight(), (float)label->font()->size());
        width = background->normalWidth();
    }
    else if (style.contentRect.valid() && label->font())
    {
        height = std::max(style.contentRect.height, (float)label->font()->size());
        width = style.contentRect.width;
    }
    label->setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    position = 0;
    cursorTime = 0;
    cursorVisible = false;
    cursorBuffer = 0;
    cursorVertices = 0;
    stringOffset = 0;
    q->addListener(lineEditListener);
    q->markToUpdate();
}

void LineEditWidget::mouseMove(const Vector &p)
{
    RANGERS_D(LineEditWidget);
    d->mousePosition = p;
}

void LineEditWidgetPrivate::updateText()
{
    RANGERS_Q(LineEditWidget);
    if (!label->font())
        return;
    q->lock();

    Rect realContentRect;
    if (style.contentRect.valid() && background)
    {
        realContentRect.x = style.contentRect.x;
        realContentRect.y = style.contentRect.y;
        realContentRect.width = (width - background->normalWidth() + style.contentRect.width);
        realContentRect.height = (height - background->normalHeight() + style.contentRect.height);
    }
    else
    {
        realContentRect.x = 0;
        realContentRect.y = 0;
        realContentRect.width = width;
        realContentRect.height = height;
    }

    int maxChars;
    if (position <= stringOffset)
    {
        stringOffset = position > 0 ? position - 1 : 0;
    }
    else
    {
        std::wstring::iterator start = text.begin() + stringOffset;
        std::wstring::iterator end = text.begin() + position;
        while ((maxChars = label->font()->maxChars(start, end, width)) < end - start)
        {
            stringOffset = (end - maxChars) - text.begin();
            start = text.begin() + stringOffset;
        }
    }
    maxChars = label->font()->maxChars(text.begin() + stringOffset, text.end(), realContentRect.width);
    label->setText(text.substr(stringOffset, maxChars));
    q->unlock();
}

LineEditWidget::~LineEditWidget()
{
    RANGERS_D(LineEditWidget);
    /*if (d->background)
        delete d->background;

    if (d->label)
        delete d->label;*/
}

void LineEditWidget::processMain()
{
    RANGERS_D(LineEditWidget);

    if (d->label->needUpdate())
        d->label->processMain();

    lock();

    if (d->background)
    {
        d->background->setGeometry(d->width, d->height);
    }

    int cursorPosition = d->label->font()->calculateStringWidth(d->text.begin() + d->stringOffset, d->text.begin() + d->position);
    if (!d->cursorBuffer)
    {
        d->cursorVertices = new Vertex[2];
        glGenBuffers(1, &d->cursorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->cursorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, d->cursorVertices, GL_DYNAMIC_DRAW);
        delete[] d->cursorVertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->cursorBuffer);
    d->cursorVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!d->style.contentRect.valid())
    {
        d->label->setPosition(0, 0);
        d->cursorVertices[0].x = cursorPosition - 0.5f;
        d->cursorVertices[0].y = d->label->height();
        d->cursorVertices[1].x = cursorPosition - 0.5f;
        d->cursorVertices[1].y = 0;
    }
    else
    {
        d->label->setPosition(d->style.contentRect.x, d->style.contentRect.y);
        d->cursorVertices[0].x = d->style.contentRect.x + cursorPosition - 0.5f;
        d->cursorVertices[0].y = d->label->height();
        d->cursorVertices[1].x = d->style.contentRect.x + cursorPosition - 0.5f;
        d->cursorVertices[1].y = d->style.contentRect.y;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}

void LineEditWidget::processLogic(int dt)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->cursorTime += dt;
    if (d->cursorTime > 500)
    {
        d->cursorVisible = !d->cursorVisible;
        d->cursorTime = 0;
    }
    unlock();
}


void LineEditWidgetPrivate::keyPressed(const SDL_keysym& key)
{
    RANGERS_Q(LineEditWidget);
    q->lock();

    if (key.sym == SDLK_RETURN);
    else if (key.sym == SDLK_LEFT)
        position--;
    else if (key.sym == SDLK_RIGHT)
        position++;
    else if (key.sym == SDLK_ESCAPE)
        text.clear();
    else if (key.sym == SDLK_BACKSPACE)
    {
        if (text.size())
            if (position > 0)
                text.erase(position - 1, 1);
        position--;
    }
    else if (key.sym == SDLK_DELETE)
    {
        if (text.size())
            if (position < text.length())
                text.erase(position, 1);
    }
    else if (key.unicode && !(key.mod & (KMOD_ALT | KMOD_META | KMOD_CTRL)))
    {
        if (key.unicode == '\t')
            text.insert(position, 1, ' ');
        else
            text.insert(position, 1, key.unicode);
        position++;
    }
    if (position < 0)
        position = 0;
    if (position > text.length())
        position = text.length();
    //label.setText(text);
    updateText();
    q->markToUpdate();

    q->unlock();
}

void LineEditWidget::setText(const std::wstring& s)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->text = s;
    d->position = 0;
    d->stringOffset = 0;
    d->updateText();
    markToUpdate();
    unlock();
}

std::wstring LineEditWidget::text() const
{
    RANGERS_D(const LineEditWidget);
    return d->text;
}

int LineEditWidget::minHeight() const
{
    RANGERS_D(const LineEditWidget);
    if (d->background && d->label->font())
        return std::max(d->label->font()->size(), (int)d->background->normalHeight());

    if (d->label->font())
        return d->label->font()->size();

    return Widget::minHeight();
}

int LineEditWidget::minWidth() const
{
    RANGERS_D(const LineEditWidget);
    std::wstring w = L"W";
    if (d->background && d->label->font())
        return std::max(d->label->font()->calculateStringWidth(w.begin(), w.end()), (int)d->background->normalHeight());

    if (d->label->font())
        return d->label->font()->calculateStringWidth(w.begin(), w.end());

    return Widget::minHeight();
}

int LineEditWidget::preferredHeight() const
{
    RANGERS_D(const LineEditWidget);
    if (d->background && d->label->font() && d->style.contentRect.valid())
        return d->background->normalHeight() + d->label->font()->size() - d->style.contentRect.height;

    return minHeight();
}

int LineEditWidget::maxHeight() const
{
    return preferredHeight();
}
}
