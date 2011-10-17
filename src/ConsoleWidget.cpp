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

#include "ConsoleWidget.h"
#include "Engine.h"
#include "Log.h"

namespace Rangers
{
ConsoleWidget::ConsoleWidget(float w, float h, Widget* parent): Widget(w, h, parent)
{
    int editSize = Engine::instance()->serviceFont()->size() + 4;
    m_lineEdit = LineEditWidget(w, editSize, Engine::instance()->serviceFont(), this);
    m_logLabel = ColorLabel("", this, Engine::instance()->serviceFont(), POSITION_X_LEFT, POSITION_Y_TOP);
    m_logLabel.setPosition(4, 4);
    m_logLabel.setFixedSize(w - 8, h - editSize - 8);
    m_lineEdit.setPosition(0, h - editSize);
    m_consoleLines = (h - editSize - 8) / Engine::instance()->serviceFont()->size();
    m_borderVertices = 0;
    m_borderBuffer = 0;
    markToUpdate();
    m_historyPosition = -1;
}

ConsoleWidget::ConsoleWidget(const Rangers::ConsoleWidget& other): Widget(other)
{
    m_lineEdit = other.m_lineEdit;
    m_logLabel = other.m_logLabel;
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_consoleLines = other.m_consoleLines;

    m_historyPosition = other.m_historyPosition;
    m_commandHistory = other.m_commandHistory;
    markToUpdate();
}

ConsoleWidget::ConsoleWidget(Widget* parent): Widget(parent)
{
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_historyPosition = -1;
}


ConsoleWidget& ConsoleWidget::operator=(const Rangers::ConsoleWidget& other)
{
    if (this == &other)
        return *this;

    m_lineEdit = other.m_lineEdit;
    m_logLabel = other.m_logLabel;
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_consoleLines = other.m_consoleLines;

    m_historyPosition = other.m_historyPosition;
    m_commandHistory = other.m_commandHistory;
    markToUpdate();

    Widget::operator=(other);
    return *this;
}



void ConsoleWidget::draw()
{
    if (!prepareDraw())
        return;

    m_lineEdit.draw();
    m_logLabel.draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glLineWidth(1);

    glDrawArrays(GL_LINES, 0, 6);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_VERTEX_ARRAY);

    endDraw();
}

void ConsoleWidget::keyPressed(SDL_keysym key)
{
    if (key.sym == SDLK_RETURN)
    {
        if (m_lineEdit.text() != L"")
        {
            m_commandHistory.push_back(m_lineEdit.text());
            m_historyPosition = -1;
            Engine::instance()->execCommand(m_lineEdit.text());
            m_lineEdit.setText(L"");
        }
    }
    else if (key.sym == SDLK_UP)
    {
        if (m_historyPosition == -1 && m_lineEdit.text() != L"")
        {
            m_commandHistory.push_back(m_lineEdit.text());
            m_historyPosition = m_commandHistory.size() - 1;
        }

        if ((m_historyPosition == -1) && (m_commandHistory.size() > 0))
            m_historyPosition = m_commandHistory.size();

        if (m_historyPosition > 0)
        {
            m_historyPosition--;
            m_lineEdit.setText(m_commandHistory[m_historyPosition]);
        }

        //markToUpdate();
    }
    else if (key.sym == SDLK_DOWN)
    {
        if (m_historyPosition < m_commandHistory.size() - 1)
        {
            m_historyPosition++;
            m_lineEdit.setText(m_commandHistory[m_historyPosition]);
        }
        else
        {
            m_historyPosition = -1;
            m_lineEdit.setText(L"");
        }

        //markToUpdate();
    }
    else
        m_lineEdit.keyPressed(key);
}

void ConsoleWidget::processLogic(int dt)
{
    m_lineEdit.processLogic(dt);

    if (Log::instance()->needUpdate())
    {
        std::list<LogEntry> l = Log::instance()->getLastLines(m_consoleLines);
        std::wstring text;

        for (std::list<LogEntry>::const_iterator i = l.begin(); i != l.end(); i++)
        {
            const LogEntry& e = *i;

            switch (e.m_level)
            {

            case LERROR:
                text += L"\\cFF0000";
                break;

            case LWARNING:
                text += L"\\cFFFF00";
                break;

            case LINFO:
                text += L"\\c00FF00";
                break;

            case LDEBUG:
                text += L"\\cFFFFFF";
                break;
            }

            text += e.m_text + L"\\cR\n";
        }

        m_logLabel.setText(text);
    }

    Widget::processLogic(dt);
}

void ConsoleWidget::processMain()
{
    lock();

    if (!m_borderBuffer)
    {
        m_borderVertices = new Vertex[6];
        glGenBuffers(1, &m_borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, m_borderVertices, GL_DYNAMIC_DRAW);
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
    m_borderVertices[3].y = m_height - m_lineEdit.height();
    m_borderVertices[4].x = 1;
    m_borderVertices[4].y = 1;
    m_borderVertices[5].x = 1;
    m_borderVertices[5].y = m_height - m_lineEdit.height();

    glUnmapBuffer(GL_ARRAY_BUFFER);

    m_lineEdit.processMain();
    m_logLabel.processMain();
    unlock();
    Widget::processMain();
}
}

