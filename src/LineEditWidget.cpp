/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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
#include "Engine.h"
#include "Font.h"
#include "ActionListener.h"
#include "Action.h"
#include <boost/variant/get.hpp>
#include "ResourceManager.h"
#include "NinePatch.h"
#include "Log.h"

#include "private/LineEditWidget_p.h"

namespace Rangers
{
class LineEditWidget::LineEditWidgetListener: public ActionListener
{
public:
    void actionPerformed(const Action &action)
    {
        if (LineEditWidget *w = dynamic_cast<LineEditWidget *>(action.source()))
        {
            if (action.type() == Action::KEY_PRESSED)
                w->keyPressed(boost::get<SDL_keysym>(action.argument()));
        }
    }
};

void LineEditWidget::draw() const
{
    RANGERS_D(const LineEditWidget);
    if (!prepareDraw())
        return;

    if (d->m_background)
        d->m_background->draw();

    d->m_label.draw();

    if (isFocused() && d->m_cursorVisible && d->m_cursorBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor3ub((d->m_style.color >> 24) & 0xff, (d->m_style.color >> 16) & 0xff, (d->m_style.color >> 8) & 0xff);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, d->m_cursorBuffer);

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
    d->m_background = 0;
    if (!font)
        font = Engine::instance()->coreFont();
    d->m_label = Label(L"", this, font, POSITION_X_LEFT, POSITION_Y_TOP);
    d->m_height = h > font->size() + 4 ? h : font->size() + 4;
    d->m_width = w;
    d->m_style.color = 0xffffffff;
    init();
}

LineEditWidget::LineEditWidget(Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    d->m_background = 0;
    d->m_cursorVertices = 0;
    d->m_cursorBuffer = 0;
    d->m_position = 0;
    d->m_cursorVisible = false;
    d->m_cursorTime = 0;
    d->m_stringOffset = 0;
}

LineEditWidget::LineEditWidget(const LineEditStyle& style, Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    d->m_background = 0;
    d->m_style = style;
    init();
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
    RANGERS_D(LineEditWidget);
    d->m_background = 0;
    d->m_cursorVertices = 0;
    d->m_cursorBuffer = 0;
    d->m_position = 0;
    d->m_cursorVisible = false;
    d->m_cursorTime = 0;
    d->m_stringOffset = 0;
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, const LineEditWidget& other):  Widget(p, other)
{
    RANGERS_D(LineEditWidget);
    d->m_style = other.d_func()->m_style;
    d->m_label = other.d_func()->m_label;
    d->m_text = other.d_func()->m_text;
    init();
}

void LineEditWidget::init()
{
    RANGERS_D(LineEditWidget);
    if (d->m_style.background.type == ResourceDescriptor::NINEPATCH)
    {
        d->m_background = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.background.resource), this);
    }
    else if (d->m_style.background.type == ResourceDescriptor::SPRITE)
    {
        d->m_background = new Sprite(boost::get<TextureRegion>(d->m_style.background.resource), this);
    }
    if ((d->m_style.font.path != L"") && (d->m_style.font.size > 0))
    {
        d->m_label = Label(d->m_text, this, ResourceManager::instance()->loadFont(d->m_style.font.path, d->m_style.font.size));
        d->m_label.setColor(((d->m_style.color >> 24) & 0xff) / 255.0f, ((d->m_style.color >> 16) & 0xff) / 255.0f, ((d->m_style.color >> 8) & 0xff) / 255.0f, ((d->m_style.color) & 0xff) / 255.0f);
    }
    if (d->m_style.contentRect.valid() && d->m_label.font() && d->m_background)
    {
        d->m_height = std::max(d->m_background->normalHeight() - d->m_style.contentRect.height + d->m_label.font()->size(), d->m_background->normalHeight());
        d->m_width = std::max(d->m_background->normalWidth(), d->m_style.contentRect.width);
    }
    else if (d->m_label.font() && d->m_background)
    {
        d->m_height = std::max(d->m_background->normalHeight(), (float)d->m_label.font()->size());
        d->m_width = d->m_background->normalWidth();
    }
    else if (d->m_style.contentRect.valid() && d->m_label.font())
    {
        d->m_height = std::max(d->m_style.contentRect.height, (float)d->m_label.font()->size());
        d->m_width = d->m_style.contentRect.width;
    }
    d->m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    addListener(new LineEditWidgetListener());
    d->m_position = 0;
    d->m_cursorTime = 0;
    d->m_cursorVisible = false;
    d->m_cursorBuffer = 0;
    d->m_cursorVertices = 0;
    d->m_stringOffset = 0;
    markToUpdate();
}

void LineEditWidget::updateText()
{
    RANGERS_D(LineEditWidget);
    if (!d->m_label.font())
        return;
    lock();
    int maxChars;
    if (d->m_position <= d->m_stringOffset)
    {
        d->m_stringOffset = d->m_position > 0 ? d->m_position - 1 : 0;
    }
    else
    {
        std::wstring::iterator start = d->m_text.begin() + d->m_stringOffset;
        std::wstring::iterator end = d->m_text.begin() + d->m_position;
        while ((maxChars = d->m_label.font()->maxChars(start, end, d->m_width)) < end - start)
        {
            d->m_stringOffset = (end - maxChars) - d->m_text.begin() - 1;
            start = d->m_text.begin() + d->m_stringOffset;
        }
    }
    maxChars = d->m_label.font()->maxChars(d->m_text.begin() + d->m_stringOffset, d->m_text.end(), d->m_width);
    d->m_label.setText(d->m_text.substr(d->m_stringOffset, maxChars));
    unlock();
}

LineEditWidget::LineEditWidget(const Rangers::LineEditWidget& other): Widget(*(new LineEditWidgetPrivate()), other)
{
    RANGERS_D(LineEditWidget);
    d->m_style = other.d_func()->m_style;
    d->m_label = other.d_func()->m_label;
    d->m_text = other.d_func()->m_text;
    init();
}

LineEditWidget::~LineEditWidget()
{
    RANGERS_D(LineEditWidget);
    if (d->m_background)
        delete d->m_background;
}

LineEditWidget& LineEditWidget::operator=(const Rangers::LineEditWidget& other)
{
    if (this == &other)
        return *this;

    RANGERS_D(LineEditWidget);
    d->m_style = other.d_func()->m_style;
    d->m_label = other.d_func()->m_label;
    d->m_text = other.d_func()->m_text;
    init();

    Widget::operator=(other);
    return *this;
}

void LineEditWidget::mouseClick(const Vector& p)
{
    lock();
    RANGERS_D(LineEditWidget);
    Engine::instance()->focusWidget(this);
    unlock();
    Widget::mouseClick(p);
}

void LineEditWidget::processMain()
{
    RANGERS_D(LineEditWidget);
    
    if (d->m_label.needUpdate())
        d->m_label.processMain();

    lock();

    if (d->m_background)
    {
        d->m_background->setGeometry(d->m_width, d->m_height);
    }

    int cursorPosition = d->m_label.font()->calculateStringWidth(d->m_text.begin() + d->m_stringOffset, d->m_text.begin() + d->m_position);
    if (!d->m_cursorBuffer)
    {
        d->m_cursorVertices = new Vertex[2];
        glGenBuffers(1, &d->m_cursorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->m_cursorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, d->m_cursorVertices, GL_DYNAMIC_DRAW);
        delete[] d->m_cursorVertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->m_cursorBuffer);
    d->m_cursorVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!d->m_style.contentRect.valid())
    {
        d->m_label.setPosition(0, 0);
        d->m_cursorVertices[0].x = cursorPosition - 0.5f;
        d->m_cursorVertices[0].y = d->m_label.height();
        d->m_cursorVertices[1].x = cursorPosition - 0.5f;
        d->m_cursorVertices[1].y = 0;
    }
    else
    {
        d->m_label.setPosition(d->m_style.contentRect.x, d->m_style.contentRect.y);
        d->m_cursorVertices[0].x = d->m_style.contentRect.x + cursorPosition - 0.5f;
        d->m_cursorVertices[0].y = d->m_label.height();
        d->m_cursorVertices[1].x = d->m_style.contentRect.x + cursorPosition - 0.5f;
        d->m_cursorVertices[1].y = d->m_style.contentRect.y;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}

void LineEditWidget::processLogic(int dt)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->m_cursorTime += dt;
    if (d->m_cursorTime > 500)
    {
        d->m_cursorVisible = !d->m_cursorVisible;
        d->m_cursorTime = 0;
    }
    unlock();
}


void LineEditWidget::keyPressed(const SDL_keysym& key)
{
    lock();
    RANGERS_D(LineEditWidget);

    if (key.sym == SDLK_RETURN);
    else if (key.sym == SDLK_LEFT)
        d->m_position--;
    else if (key.sym == SDLK_RIGHT)
        d->m_position++;
    else if (key.sym == SDLK_ESCAPE)
        d->m_text.clear();
    else if (key.sym == SDLK_BACKSPACE)
    {
        if (d->m_text.size())
            if (d->m_position > 0)
                d->m_text.erase(d->m_position - 1, 1);
        d->m_position--;
    }
    else if (key.sym == SDLK_DELETE)
    {
        if (d->m_text.size())
            if (d->m_position < d->m_text.length())
                d->m_text.erase(d->m_position, 1);
    }
    else if (key.unicode && !(key.mod & (KMOD_ALT | KMOD_META | KMOD_CTRL)))
    {
        if (key.unicode == '\t')
            d->m_text.insert(d->m_position, 1, ' ');
        else
            d->m_text.insert(d->m_position, 1, key.unicode);
        d->m_position++;
    }
    if (d->m_position < 0) d->m_position = 0;
    if (d->m_position > d->m_text.length()) d->m_position = d->m_text.length();
    //m_label.setText(m_text);
    updateText();
    markToUpdate();

    unlock();
}

void LineEditWidget::setText(const std::wstring& s)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->m_text = s;
    d->m_position = 0;
    d->m_stringOffset = 0;
    updateText();
    markToUpdate();
    unlock();
}

std::wstring LineEditWidget::text() const
{
    RANGERS_D(const LineEditWidget);
    return d->m_text;
}

int LineEditWidget::minHeight() const
{
    RANGERS_D(const LineEditWidget);
    if (d->m_background && d->m_label.font())
        return std::max(d->m_label.font()->size(), (int)d->m_background->normalHeight());

    if (d->m_label.font())
        return d->m_label.font()->size();

    return Widget::minHeight();
}

int LineEditWidget::minWidth() const
{
    RANGERS_D(const LineEditWidget);
    std::wstring w = L"W";
    if (d->m_background && d->m_label.font())
        return std::max(d->m_label.font()->calculateStringWidth(w.begin(), w.end()), (int)d->m_background->normalHeight());

    if (d->m_label.font())
        return d->m_label.font()->calculateStringWidth(w.begin(), w.end());

    return Widget::minHeight();
}

int LineEditWidget::preferredHeight() const
{
    RANGERS_D(const LineEditWidget);
    if (d->m_background && d->m_label.font() && d->m_style.contentRect.valid())
        return d->m_background->normalHeight() + d->m_label.font()->size() - d->m_style.contentRect.height;

    return minHeight();
}

int LineEditWidget::maxHeight() const
{
    return preferredHeight();
}
}
