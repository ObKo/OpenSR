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

#include "ConsoleWidget.h"
#include <SDL.h>
#include "Engine.h"
#include "Log.h"
#include "Font.h"
#include "Action.h"
#include "ActionListener.h"
#include <boost/variant/get.hpp>

#include "private/ConsoleWidget_p.h"

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
                    if (w->d_func()->m_lineEdit.text() != L"")
                    {
                        w->d_func()->m_commandHistory.push_back(w->d_func()->m_lineEdit.text());
                        w->d_func()->m_historyPosition = -1;
                        Engine::instance()->execCommand(w->d_func()->m_lineEdit.text());
                        w->d_func()->m_lineEdit.setText(L"");
                    }
                }
                else if (key.sym == SDLK_UP)
                {
                    if (w->d_func()->m_historyPosition == -1 && w->d_func()->m_lineEdit.text() != L"")
                    {
                        w->d_func()->m_commandHistory.push_back(w->d_func()->m_lineEdit.text());
                        w->d_func()->m_historyPosition = w->d_func()->m_commandHistory.size() - 1;
                    }

                    if ((w->d_func()->m_historyPosition == -1) && (w->d_func()->m_commandHistory.size() > 0))
                        w->d_func()->m_historyPosition = w->d_func()->m_commandHistory.size();

                    if (w->d_func()->m_historyPosition > 0)
                    {
                        w->d_func()->m_historyPosition--;
                        w->d_func()->m_lineEdit.setText(w->d_func()->m_commandHistory[w->d_func()->m_historyPosition]);
                    }

                    //markToUpdate();
                }
                else if (key.sym == SDLK_DOWN)
                {
                    if (w->d_func()->m_historyPosition < w->d_func()->m_commandHistory.size() - 1)
                    {
                        w->d_func()->m_historyPosition++;
                        w->d_func()->m_lineEdit.setText(w->d_func()->m_commandHistory[w->d_func()->m_historyPosition]);
                    }
                    else
                    {
                        w->d_func()->m_historyPosition = -1;
                        w->d_func()->m_lineEdit.setText(L"");
                    }

                    //markToUpdate();
                }
            }
        }
    }
};

ConsoleWidgetPrivate::ConsoleWidgetPrivate()
{
    m_borderVertices = 0;
    m_borderBuffer = 0;
    m_texture = 0;
    m_consoleLines = 0;
    m_historyPosition = -1;
}

ConsoleWidget::ConsoleWidget(float w, float h, Widget* parent): Widget(*(new ConsoleWidgetPrivate()), parent)
{
    RANGERS_D(ConsoleWidget);
    d->m_width = w;
    d->m_height = h;
    int editSize = Engine::instance()->serviceFont()->size() + 2;
    d->m_lineEdit = LineEditWidget(w - 8, Engine::instance()->serviceFont()->size(), Engine::instance()->serviceFont(), this);
    d->m_logLabel = ColorLabel("", this, Engine::instance()->serviceFont(), POSITION_X_LEFT, POSITION_Y_TOP);
    d->m_logLabel.setPosition(4, 4);
    d->m_logLabel.setFixedSize(w - 8, h - editSize - 8);
    d->m_lineEdit.setPosition(4, h - editSize);
    d->m_consoleLines = (h - editSize - 8) / Engine::instance()->serviceFont()->size();
    d->m_lineEdit.addListener(new ConsoleLineEditListener());
    markToUpdate();
}

ConsoleWidget::ConsoleWidget(const Rangers::ConsoleWidget& other): Widget(*(new ConsoleWidgetPrivate()), other)
{
    RANGERS_D(ConsoleWidget);
    d->m_lineEdit = other.d_func()->m_lineEdit;
    d->m_logLabel = other.d_func()->m_logLabel;
    d->m_consoleLines = other.d_func()->m_consoleLines;

    d->m_historyPosition = other.d_func()->m_historyPosition;
    d->m_commandHistory = other.d_func()->m_commandHistory;
    d->m_lineEdit.addListener(new ConsoleLineEditListener());
    markToUpdate();
}

ConsoleWidget::ConsoleWidget(Widget* parent): Widget(*(new ConsoleWidgetPrivate()), parent)
{
    RANGERS_D(ConsoleWidget);
    d->m_lineEdit.addListener(new ConsoleLineEditListener());
}


ConsoleWidget& ConsoleWidget::operator=(const Rangers::ConsoleWidget& other)
{
    if (this == &other)
        return *this;

    RANGERS_D(ConsoleWidget);
    d->m_lineEdit = other.d_func()->m_lineEdit;
    d->m_logLabel = other.d_func()->m_logLabel;
    d->m_borderVertices = 0;
    d->m_borderBuffer = 0;
    d->m_consoleLines = other.d_func()->m_consoleLines;

    d->m_historyPosition = other.d_func()->m_historyPosition;
    d->m_commandHistory = other.d_func()->m_commandHistory;
    markToUpdate();

    Widget::operator=(other);
    return *this;
}

ConsoleWidget::ConsoleWidget(ConsoleWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
    RANGERS_D(ConsoleWidget);
    d->m_lineEdit.addListener(new ConsoleLineEditListener());
}

ConsoleWidget::ConsoleWidget(ConsoleWidgetPrivate &p, const ConsoleWidget& other): Widget(p, other)
{
    RANGERS_D(ConsoleWidget);
    d->m_lineEdit = other.d_func()->m_lineEdit;
    d->m_logLabel = other.d_func()->m_logLabel;
    d->m_consoleLines = other.d_func()->m_consoleLines;

    d->m_historyPosition = other.d_func()->m_historyPosition;
    d->m_commandHistory = other.d_func()->m_commandHistory;
    d->m_lineEdit.addListener(new ConsoleLineEditListener());
    markToUpdate();
}

void ConsoleWidget::draw() const
{
    RANGERS_D(const ConsoleWidget);
    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, d->m_texture);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->m_borderBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

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

    d->m_lineEdit.draw();
    d->m_logLabel.draw();
    endDraw();
}

void ConsoleWidget::processLogic(int dt)
{
    lock();
    RANGERS_D(ConsoleWidget);
    d->m_lineEdit.processLogic(dt);

    if (Log::instance()->needUpdate())
    {
        std::list<LogEntry> l = Log::instance()->getLastLines(d->m_consoleLines);
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

        d->m_logLabel.setText(text);
    }
    unlock();

    Widget::processLogic(dt);
}

void ConsoleWidget::processMain()
{
    //if (d->m_lineEdit.needUpdate())
    //    d->m_lineEdit.processMain();
    lock();
    RANGERS_D(ConsoleWidget);

    if (!d->m_borderBuffer)
    {
        d->m_borderVertices = new Vertex[6];
        glGenBuffers(1, &d->m_borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->m_borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 7, d->m_borderVertices, GL_DYNAMIC_DRAW);
        delete d->m_borderVertices;
    }

    if (!d->m_texture)
    {
        glGenTextures(1, &d->m_texture);
        unsigned char pattern[16] = {0x80, 0xC0, 0xC0, 0x80,
                                     0xC0, 0x80, 0x80, 0xC0,
                                     0xC0, 0x80, 0x80, 0xC0,
                                     0x80, 0xC0, 0xC0, 0x80
                                    };
        glBindTexture(GL_TEXTURE_2D, d->m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 4, 4, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->m_borderBuffer);

    d->m_borderVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    d->m_borderVertices[0].x = 1;
    d->m_borderVertices[0].y = d->m_height;
    d->m_borderVertices[0].u = 0;
    d->m_borderVertices[0].v = d->m_height / 4.0f;
    d->m_borderVertices[1].x = 1;
    d->m_borderVertices[1].y = 1;
    d->m_borderVertices[1].u = 0;
    d->m_borderVertices[1].v = 0;
    d->m_borderVertices[2].x = d->m_width - 1;
    d->m_borderVertices[2].y = 1;
    d->m_borderVertices[2].u = d->m_width / 4.0f;
    d->m_borderVertices[2].v = 0;
    d->m_borderVertices[3].x = d->m_width - 1;
    d->m_borderVertices[3].y = d->m_height;
    d->m_borderVertices[3].u = d->m_width / 4.0f;
    d->m_borderVertices[3].v = d->m_height / 4.0f;

    d->m_borderVertices[4].x = 1;
    d->m_borderVertices[4].y = d->m_height;
    d->m_borderVertices[5].x = 1;
    d->m_borderVertices[5].y = d->m_height - d->m_lineEdit.height() - 1;
    d->m_borderVertices[6].x = d->m_width - 1;
    d->m_borderVertices[6].y = d->m_height - d->m_lineEdit.height() - 1;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}
}

