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

namespace Rangers
{
void LineEditWidget::draw()
{
    if (!prepareDraw())
        return;

    m_label.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glLineWidth(1);

    if (m_cursorVisible)
        glDrawArrays(GL_LINES, 0, 10);
    else
        glDrawArrays(GL_LINES, 0, 8);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

LineEditWidget::LineEditWidget(float w, float h, boost::shared_ptr< Font > font, Widget* parent):
    Widget(parent)
{
    if (!font)
        font = Engine::instance()->coreFont();
    m_label = Label(L"", this, font, POSITION_X_LEFT, POSITION_Y_TOP);
    m_height = h > font->size() + 4 ? h : font->size() + 4;
    m_width = w;
    m_borderBuffer = 0;
    m_label.setPosition(5, 2);
    m_position = 0;
    m_cursorTime = 0;
    m_cursorVisible = true;
    markToUpdate();
}

LineEditWidget::LineEditWidget(Widget* parent): Widget(parent)
{
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_position = 0;
    m_cursorVisible = false;
    m_cursorTime = 0;
}

LineEditWidget::LineEditWidget(const Rangers::LineEditWidget& other): Widget(other)
{
    m_label = other.m_label;
    m_text = other.m_text;
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_position = 0;
    m_cursorVisible = false;
    m_cursorTime = 0;
}

LineEditWidget& LineEditWidget::operator=(const Rangers::LineEditWidget& other)
{
    if (this == &other)
        return *this;

    m_label = other.m_label;
    m_text = other.m_text;
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_position = 0;
    m_cursorVisible = false;
    m_cursorTime = 0;

    Widget::operator=(other);
    return *this;
}



void LineEditWidget::processMain()
{
    lock();
    int cursorPosition = m_label.font()->calculateStringWidth(m_text.begin(), m_text.begin() + m_position) + 6;
    if (!m_borderBuffer)
    {
        m_borderVertices = new Vertex[10];
        glGenBuffers(1, &m_borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 10, m_borderVertices, GL_DYNAMIC_DRAW);
        delete m_borderVertices;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);
    m_borderVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    m_borderVertices[0].x = 1;
    m_borderVertices[0].y = 1;
    m_borderVertices[1].x = m_width - 1;
    m_borderVertices[1].y = 1;
    m_borderVertices[2].x = m_width - 1;
    m_borderVertices[2].y = 1;
    m_borderVertices[3].x = m_width - 1;
    m_borderVertices[3].y = m_height - 1;
    m_borderVertices[4].x = m_width - 1;
    m_borderVertices[4].y = m_height - 1;
    m_borderVertices[5].x = 1;
    m_borderVertices[5].y = m_height - 1;
    m_borderVertices[6].x = 1;
    m_borderVertices[6].y = m_height - 1;
    m_borderVertices[7].x = 1;
    m_borderVertices[7].y = 1;
    m_borderVertices[8].x = cursorPosition;
    m_borderVertices[8].y = m_height - 2;
    m_borderVertices[9].x = cursorPosition;
    m_borderVertices[9].y = 2;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}

void LineEditWidget::processLogic(int dt)
{
    m_cursorTime += dt;
    if (m_cursorTime > 500)
    {
        m_cursorVisible = !m_cursorVisible;
        m_cursorTime = 0;
    }
}


void LineEditWidget::keyPressed(SDL_keysym key)
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
    m_text = s;
    m_position = s.length();
    m_label.setText(s);
    markToUpdate();
}

std::wstring LineEditWidget::text() const
{
    return m_text;
}
}
