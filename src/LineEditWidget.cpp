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
class LineEditWidget::LineEditWidgetListener: public ActionListener
{
public:
    void actionPerformed(const Action &action)
    {
        if (LineEditWidget *w = dynamic_cast<LineEditWidget *>(action.source()))
        {
            if (action.type() == Action::KEY_PRESSED)
                w->d_func()->keyPressed(boost::get<SDL_keysym>(action.argument()));
        }
    }
};

LineEditWidgetPrivate::LineEditWidgetPrivate()
{
    m_background = 0;
    m_cursorVertices = 0;
    m_cursorBuffer = 0;
    m_position = 0;
    m_cursorVisible = false;
    m_cursorTime = 0;
    m_stringOffset = 0;
}

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
    if (!font)
        font = Engine::instance()->coreFont();
    d->m_label = Label(L"", this, font, POSITION_X_LEFT, POSITION_Y_TOP);
    d->m_height = h > font->size() + 4 ? h : font->size() + 4;
    d->m_width = w;
    d->m_style.color = 0xffffffff;
    d->init();
}

LineEditWidget::LineEditWidget(Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
}

LineEditWidget::LineEditWidget(const LineEditStyle& style, Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    d->m_style = style;
    d->init();
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, const LineEditWidget& other):  Widget(p, other)
{
    RANGERS_D(LineEditWidget);
    d->m_style = other.d_func()->m_style;
    d->m_label = other.d_func()->m_label;
    d->m_text = other.d_func()->m_text;
    d->init();
}

void LineEditWidgetPrivate::init()
{
    RANGERS_Q(LineEditWidget);
    if (m_style.background.type == ResourceDescriptor::NINEPATCH)
    {
        m_background = new NinePatch(boost::get<NinePatchDescriptor>(m_style.background.resource), q);
    }
    else if (m_style.background.type == ResourceDescriptor::SPRITE)
    {
        m_background = new Sprite(boost::get<TextureRegion>(m_style.background.resource), q);
    }
    if ((m_style.font.path != L"") && (m_style.font.size > 0))
    {
        m_label = Label(m_text, q, ResourceManager::instance()->loadFont(m_style.font.path, m_style.font.size));
        m_label.setColor(((m_style.color >> 24) & 0xff) / 255.0f, ((m_style.color >> 16) & 0xff) / 255.0f, ((m_style.color >> 8) & 0xff) / 255.0f, ((m_style.color) & 0xff) / 255.0f);
    }
    if (m_style.contentRect.valid() && m_label.font() && m_background)
    {
        m_height = std::max(m_background->normalHeight() - m_style.contentRect.height + m_label.font()->size(), m_background->normalHeight());
        m_width = std::max(m_background->normalWidth(), m_style.contentRect.width);
    }
    else if (m_label.font() && m_background)
    {
        m_height = std::max(m_background->normalHeight(), (float)m_label.font()->size());
        m_width = m_background->normalWidth();
    }
    else if (m_style.contentRect.valid() && m_label.font())
    {
        m_height = std::max(m_style.contentRect.height, (float)m_label.font()->size());
        m_width = m_style.contentRect.width;
    }
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    q->addListener(new LineEditWidget::LineEditWidgetListener());
    m_position = 0;
    m_cursorTime = 0;
    m_cursorVisible = false;
    m_cursorBuffer = 0;
    m_cursorVertices = 0;
    m_stringOffset = 0;
    q->markToUpdate();
}

void LineEditWidgetPrivate::updateText()
{
    RANGERS_Q(LineEditWidget);
    if (!m_label.font())
        return;
    q->lock();
    int maxChars;
    if (m_position <= m_stringOffset)
    {
        m_stringOffset = m_position > 0 ? m_position - 1 : 0;
    }
    else
    {
        std::wstring::iterator start = m_text.begin() + m_stringOffset;
        std::wstring::iterator end = m_text.begin() + m_position;
        while ((maxChars = m_label.font()->maxChars(start, end, m_width)) < end - start)
        {
            m_stringOffset = (end - maxChars) - m_text.begin() - 1;
            start = m_text.begin() + m_stringOffset;
        }
    }
    maxChars = m_label.font()->maxChars(m_text.begin() + m_stringOffset, m_text.end(), m_width);
    m_label.setText(m_text.substr(m_stringOffset, maxChars));
    q->unlock();
}

LineEditWidget::LineEditWidget(const Rangers::LineEditWidget& other): Widget(*(new LineEditWidgetPrivate()), other)
{
    RANGERS_D(LineEditWidget);
    d->m_style = other.d_func()->m_style;
    d->m_label = other.d_func()->m_label;
    d->m_text = other.d_func()->m_text;
    d->init();
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
    d->init();

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


void LineEditWidgetPrivate::keyPressed(const SDL_keysym& key)
{
    RANGERS_Q(LineEditWidget);
    q->lock();

    if (key.sym == SDLK_RETURN);
    else if (key.sym == SDLK_LEFT)
        m_position--;
    else if (key.sym == SDLK_RIGHT)
        m_position++;
    else if (key.sym == SDLK_ESCAPE)
        m_text.clear();
    else if (key.sym == SDLK_BACKSPACE)
    {
        if (m_text.size())
            if (m_position > 0)
                m_text.erase(m_position - 1, 1);
        m_position--;
    }
    else if (key.sym == SDLK_DELETE)
    {
        if (m_text.size())
            if (m_position < m_text.length())
                m_text.erase(m_position, 1);
    }
    else if (key.unicode && !(key.mod & (KMOD_ALT | KMOD_META | KMOD_CTRL)))
    {
        if (key.unicode == '\t')
            m_text.insert(m_position, 1, ' ');
        else
            m_text.insert(m_position, 1, key.unicode);
        m_position++;
    }
    if (m_position < 0)
        m_position = 0;
    if (m_position > m_text.length())
        m_position = m_text.length();
    //m_label.setText(m_text);
    updateText();
    q->markToUpdate();

    q->unlock();
}

void LineEditWidget::setText(const std::wstring& s)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->m_text = s;
    d->m_position = 0;
    d->m_stringOffset = 0;
    d->updateText();
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
