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
#include "Font.h"
#include "Action.h"
#include "ActionListener.h"
#include <boost/variant/get.hpp>

namespace Rangers
{
class ConsoleWidget::ConsoleLineEditListener: public ActionListener
{
public:
    void actionPerformed(const Action &action)
    {
        if (ConsoleWidget *w = dynamic_cast<ConsoleWidget*>(action.source()->parent()))
        {
            if (action.type() == Action::KEY_PRESSED)
            {
                SDL_keysym key = boost::get<SDL_keysym>(action.argument());
                if (key.sym == SDLK_RETURN)
                {
                    if (w->m_lineEdit.text() != L"")
                    {
                        w->m_commandHistory.push_back(w->m_lineEdit.text());
                        w->m_historyPosition = -1;
                        Engine::instance()->execCommand(w->m_lineEdit.text());
                        w->m_lineEdit.setText(L"");
                    }
                }
                else if (key.sym == SDLK_UP)
                {
                    if (w->m_historyPosition == -1 && w->m_lineEdit.text() != L"")
                    {
                        w->m_commandHistory.push_back(w->m_lineEdit.text());
                        w->m_historyPosition = w->m_commandHistory.size() - 1;
                    }

                    if ((w->m_historyPosition == -1) && (w->m_commandHistory.size() > 0))
                        w->m_historyPosition = w->m_commandHistory.size();

                    if (w->m_historyPosition > 0)
                    {
                        w->m_historyPosition--;
                        w->m_lineEdit.setText(w->m_commandHistory[w->m_historyPosition]);
                    }

                    //markToUpdate();
                }
                else if (key.sym == SDLK_DOWN)
                {
                    if (w->m_historyPosition < w->m_commandHistory.size() - 1)
                    {
                        w->m_historyPosition++;
                        w->m_lineEdit.setText(w->m_commandHistory[w->m_historyPosition]);
                    }
                    else
                    {
                        w->m_historyPosition = -1;
                        w->m_lineEdit.setText(L"");
                    }

                    //markToUpdate();
                }
            }
        }
    }
};

ConsoleWidget::ConsoleWidget(float w, float h, Widget* parent): Widget(w, h, parent)
{
    int editSize = Engine::instance()->serviceFont()->size() + 2;
    m_lineEdit = LineEditWidget(w - 8, Engine::instance()->serviceFont()->size(), Engine::instance()->serviceFont(), this);
    m_logLabel = ColorLabel("", this, Engine::instance()->serviceFont(), POSITION_X_LEFT, POSITION_Y_TOP);
    m_logLabel.setPosition(4, 4);
    m_logLabel.setFixedSize(w - 8, h - editSize - 8);
    m_lineEdit.setPosition(4, h - editSize);
    m_consoleLines = (h - editSize - 8) / Engine::instance()->serviceFont()->size();
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_texture = 0;
    markToUpdate();
    m_historyPosition = -1;
    m_lineEdit.addListener(new ConsoleLineEditListener());
}

ConsoleWidget::ConsoleWidget(const Rangers::ConsoleWidget& other): Widget(other)
{
    m_lineEdit = other.m_lineEdit;
    m_logLabel = other.m_logLabel;
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_texture = 0;
    m_consoleLines = other.m_consoleLines;

    m_historyPosition = other.m_historyPosition;
    m_commandHistory = other.m_commandHistory;
    m_lineEdit.addListener(new ConsoleLineEditListener());
    markToUpdate();
}

ConsoleWidget::ConsoleWidget(Widget* parent): Widget(parent)
{
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_texture = 0;
    m_historyPosition = -1;
    m_lineEdit.addListener(new ConsoleLineEditListener());
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



void ConsoleWidget::draw() const
{
    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));

    glColor3f(0, 0, 0);
    glDrawArrays(GL_QUADS, 0, 4);

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(1);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    glDrawArrays(GL_LINES, 5, 2);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);


    m_lineEdit.draw();
    m_logLabel.draw();
    endDraw();
}

void ConsoleWidget::processLogic(int dt)
{
    lock();
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
    unlock();

    Widget::processLogic(dt);
}

void ConsoleWidget::processMain()
{
    if (m_lineEdit.needUpdate())
        m_lineEdit.processMain();
    lock();

    if (!m_borderBuffer)
    {
        m_borderVertices = new Vertex[6];
        glGenBuffers(1, &m_borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 7, m_borderVertices, GL_DYNAMIC_DRAW);
        delete m_borderVertices;
    }

    if (!m_texture)
    {
        glGenTextures(1, &m_texture);
        unsigned char pattern[16] = {0x80, 0xC0, 0xC0, 0x80,
                                     0xC0, 0x80, 0x80, 0xC0,
                                     0xC0, 0x80, 0x80, 0xC0,
                                     0x80, 0xC0, 0xC0, 0x80
                                    };
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 4, 4, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_borderBuffer);

    m_borderVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    m_borderVertices[0].x = 1;
    m_borderVertices[0].y = m_height;
    m_borderVertices[0].u = 0;
    m_borderVertices[0].v = m_height / 4.0f;
    m_borderVertices[1].x = 1;
    m_borderVertices[1].y = 1;
    m_borderVertices[1].u = 0;
    m_borderVertices[1].v = 0;
    m_borderVertices[2].x = m_width - 1;
    m_borderVertices[2].y = 1;
    m_borderVertices[2].u = m_width / 4.0f;
    m_borderVertices[2].v = 0;
    m_borderVertices[3].x = m_width - 1;
    m_borderVertices[3].y = m_height;
    m_borderVertices[3].u = m_width / 4.0f;
    m_borderVertices[3].v = m_height / 4.0f;

    m_borderVertices[4].x = 1;
    m_borderVertices[4].y = m_height;
    m_borderVertices[5].x = 1;
    m_borderVertices[5].y = m_height - m_lineEdit.height() - 1;
    m_borderVertices[6].x = m_width - 1;
    m_borderVertices[6].y = m_height - m_lineEdit.height() - 1;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}
}

