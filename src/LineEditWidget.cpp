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
    //TODO: Cursor color
    if (!prepareDraw())
        return;

    if (m_background)
        m_background->draw();

    m_label.draw();

    if (m_cursorVisible)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor3ub((m_style.color >> 16) & 0xff, (m_style.color >> 8) & 0xff, m_style.color & 0xff);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, m_cursorBuffer);

        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
        glLineWidth(1);


        glDrawArrays(GL_LINES, 0, 2);

        glDisableClientState(GL_ARRAY_BUFFER);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    endDraw();
}

LineEditWidget::LineEditWidget(float w, float h, boost::shared_ptr< Font > font, Widget* parent):
    Widget(parent), m_background(0)
{
    if (!font)
        font = Engine::instance()->coreFont();
    m_label = Label(L"", this, font, POSITION_X_LEFT, POSITION_Y_TOP);
    m_height = h > font->size() + 4 ? h : font->size() + 4;
    m_width = w;
    m_style.color = 0xffffffff;
    init();
}

LineEditWidget::LineEditWidget(Widget* parent): Widget(parent), m_background(0)
{
    m_cursorVertices = 0;
    m_cursorBuffer = 0;
    m_position = 0;
    m_cursorVisible = false;
    m_cursorTime = 0;
}

LineEditWidget::LineEditWidget(const LineEditStyle& style, Widget* parent): Widget(parent), m_background(0)
{
    m_style = style;
    init();
}

void LineEditWidget::init()
{
    if (m_style.background.type == ResourceDescriptor::NINEPATCH)
    {
        m_background = new NinePatch(boost::get<NinePatchDescriptor>(m_style.background.resource), this);
    }
    else if (m_style.background.type == ResourceDescriptor::SPRITE)
    {
        m_background = new Sprite(boost::get<TextureRegion>(m_style.background.resource), this);
    }
    if ((m_style.font.path != L"") && (m_style.font.size > 0))
    {
        m_label = Label(m_text, this, ResourceManager::instance()->loadFont(m_style.font.path, m_style.font.size));
        m_label.setColor(((m_style.color >> 16) & 0xff) / 255.0f, ((m_style.color >> 8) & 0xff) / 255.0f, (m_style.color & 0xff) / 255.0f, ((m_style.color >> 24) & 0xff) / 255.0f);
    }
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    addListener(new LineEditWidgetListener());
    m_position = 0;
    m_cursorTime = 0;
    m_cursorVisible = true;
    m_cursorBuffer = 0;
    m_cursorVertices = 0;
    markToUpdate();
}

LineEditWidget::LineEditWidget(const Rangers::LineEditWidget& other): Widget(other)
{
    if (this == &other)
        return;
    m_style = other.m_style;
    m_label = other.m_label;
    m_text = other.m_text;
    init();
}

LineEditWidget::~LineEditWidget()
{
    if (m_background)
        delete m_background;
}

LineEditWidget& LineEditWidget::operator=(const Rangers::LineEditWidget& other)
{
    if (this == &other)
        return *this;

    m_style = other.m_style;
    m_label = other.m_label;
    m_text = other.m_text;
    init();

    Widget::operator=(other);
    return *this;
}



void LineEditWidget::processMain()
{
    if (m_label.needUpdate())
        m_label.processMain();

    lock();

    if (m_background)
    {
        m_background->setGeometry(m_width, m_height);
    }

    int cursorPosition = m_label.font()->calculateStringWidth(m_text.begin(), m_text.begin() + m_position);
    if (!m_cursorBuffer)
    {
        m_cursorVertices = new Vertex[2];
        glGenBuffers(1, &m_cursorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_cursorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, m_cursorVertices, GL_DYNAMIC_DRAW);
        delete[] m_cursorVertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_cursorBuffer);
    m_cursorVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (m_style.contentRect.width < 0 || m_style.contentRect.height < 0)
    {
        m_label.setPosition(0, 0);
        m_cursorVertices[0].x = cursorPosition;
        m_cursorVertices[0].y = m_label.height();
        m_cursorVertices[1].x = cursorPosition;
        m_cursorVertices[1].y = 0;
    }
    else
    {
        m_label.setPosition(m_style.contentRect.x, m_style.contentRect.y);
        m_cursorVertices[0].x = m_style.contentRect.x + cursorPosition;
        m_cursorVertices[0].y = m_label.height();
        m_cursorVertices[1].x = m_style.contentRect.x + cursorPosition;
        m_cursorVertices[1].y = m_style.contentRect.y;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}

void LineEditWidget::processLogic(int dt)
{
    lock();
    m_cursorTime += dt;
    if (m_cursorTime > 500)
    {
        m_cursorVisible = !m_cursorVisible;
        m_cursorTime = 0;
    }
    unlock();
}


void LineEditWidget::keyPressed(const SDL_keysym& key)
{
    lock();

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
    if (m_position < 0) m_position = 0;
    if (m_position > m_text.length()) m_position = m_text.length();
    m_label.setText(m_text);
    markToUpdate();

    unlock();
}

void LineEditWidget::setText(const std::wstring& s)
{
    lock();
    m_text = s;
    m_position = s.length();
    m_label.setText(s);
    markToUpdate();
    unlock();
}

std::wstring LineEditWidget::text() const
{
    return m_text;
}
}
