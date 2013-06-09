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

#include "OpenSR/ConsoleWidget.h"
#include <SDL.h>
#include "OpenSR/Engine.h"
#include "OpenSR/Log.h"
#include "OpenSR/Font.h"
#include "OpenSR/Action.h"
#include "OpenSR/ActionListener.h"
#include <boost/variant/get.hpp>

#include "OpenSR/private/ConsoleWidget_p.h"

namespace Rangers
{
void ConsoleWidgetPrivate::ConsoleLineEditListener::actionPerformed(const Action &action)
{
    if (ConsoleWidget *w = dynamic_cast<ConsoleWidget*>(action.source()->parent()))
    {
        if (action.type() == Action::KEY_PRESSED)
        {
            SDL_Keysym key = boost::get<SDL_Keysym>(action.argument());
            if (key.sym == SDLK_RETURN)
            {
                if (w->d_func()->lineEdit->text() != L"")
                {
                    w->d_func()->commandHistory.push_back(w->d_func()->lineEdit->text());
                    w->d_func()->historyPosition = -1;
                    Engine::instance().execCommand(w->d_func()->lineEdit->text());
                    w->d_func()->lineEdit->setText(L"");
                }
            }
            else if (key.sym == SDLK_UP)
            {
                if (w->d_func()->historyPosition == -1 && w->d_func()->lineEdit->text() != L"")
                {
                    w->d_func()->commandHistory.push_back(w->d_func()->lineEdit->text());
                    w->d_func()->historyPosition = w->d_func()->commandHistory.size() - 1;
                }

                if ((w->d_func()->historyPosition == -1) && (w->d_func()->commandHistory.size() > 0))
                    w->d_func()->historyPosition = w->d_func()->commandHistory.size();

                if (w->d_func()->historyPosition > 0)
                {
                    w->d_func()->historyPosition--;
                    w->d_func()->lineEdit->setText(w->d_func()->commandHistory[w->d_func()->historyPosition]);
                }

                //markToUpdate();
            }
            else if (key.sym == SDLK_DOWN)
            {
                if (w->d_func()->historyPosition < w->d_func()->commandHistory.size() - 1)
                {
                    w->d_func()->historyPosition++;
                    w->d_func()->lineEdit->setText(w->d_func()->commandHistory[w->d_func()->historyPosition]);
                }
                else
                {
                    w->d_func()->historyPosition = -1;
                    w->d_func()->lineEdit->setText(L"");
                }

                //markToUpdate();
            }
        }
    }
}

ConsoleWidgetPrivate::ConsoleWidgetPrivate()
{
    borderVertices = 0;
    borderBuffer = 0;
    texture = 0;
    consoleLines = 0;
    historyPosition = -1;
    listener = boost::shared_ptr<ConsoleLineEditListener>(new ConsoleLineEditListener());
}

ConsoleWidget::ConsoleWidget(float w, float h, Widget* parent): Widget(*(new ConsoleWidgetPrivate()), parent)
{
    RANGERS_D(ConsoleWidget);
    d->width = w;
    d->height = h;
    int editSize = Engine::instance().serviceFont()->size() + 2;
    d->lineEdit = boost::shared_ptr<LineEdit>(new LineEdit(w - 8, Engine::instance().serviceFont()->size(), Engine::instance().serviceFont()));
    d->logLabel = boost::shared_ptr<ColorLabel>(new ColorLabel("", 0, Engine::instance().serviceFont(), POSITION_X_LEFT, POSITION_Y_TOP));
    d->logLabel->setPosition(4, 4);
    d->logLabel->setFixedSize(w - 8, h - editSize - 8);
    d->lineEdit->setPosition(4, h - editSize);
    d->consoleLines = (h - editSize - 8) / Engine::instance().serviceFont()->size();
    d->lineEdit->addListener(d->listener);

    addWidget(d->lineEdit);
    addChild(d->logLabel);

    markToUpdate();
}

ConsoleWidget::ConsoleWidget(Widget* parent): Widget(*(new ConsoleWidgetPrivate()), parent)
{
}

ConsoleWidget::ConsoleWidget(ConsoleWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
}

ConsoleWidget::~ConsoleWidget()
{
    RANGERS_D(ConsoleWidget);

    //delete d->lineEdit;
    //delete d->logLabel;
}

void ConsoleWidget::draw() const
{
    RANGERS_D(const ConsoleWidget);
    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, d->texture);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->borderBuffer);

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

    d->lineEdit->draw();
    d->logLabel->draw();
    endDraw();
}

void ConsoleWidget::processLogic(int dt)
{
    lock();
    RANGERS_D(ConsoleWidget);
    d->lineEdit->processLogic(dt);

    if (Log::instance()->needUpdate())
    {
        std::list<LogEntry> l = Log::instance()->getLastLines(d->consoleLines);
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

        d->logLabel->setText(text);
    }
    unlock();

    Widget::processLogic(dt);
}

void ConsoleWidget::processMain()
{
    //if (d->lineEdit.needUpdate())
    //    d->lineEdit.processMain();
    lock();
    RANGERS_D(ConsoleWidget);

    if (!d->borderBuffer)
    {
        d->borderVertices = new Vertex[7];
        glGenBuffers(1, &d->borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 7, d->borderVertices, GL_DYNAMIC_DRAW);
        delete[] d->borderVertices;
    }

    if (!d->texture)
    {
        glGenTextures(1, &d->texture);
        unsigned char pattern[16] = {0x80, 0xC0, 0xC0, 0x80,
                                     0xC0, 0x80, 0x80, 0xC0,
                                     0xC0, 0x80, 0x80, 0xC0,
                                     0x80, 0xC0, 0xC0, 0x80
                                    };
        glBindTexture(GL_TEXTURE_2D, d->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 4, 4, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->borderBuffer);

    d->borderVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    d->borderVertices[0].x = 0.5f;
    d->borderVertices[0].y = d->height + 0.5f;
    d->borderVertices[0].u = 0;
    d->borderVertices[0].v = d->height / 4.0f;
    d->borderVertices[1].x = 0.5f;
    d->borderVertices[1].y = 0.5f;
    d->borderVertices[1].u = 0;
    d->borderVertices[1].v = 0;
    d->borderVertices[2].x = d->width - 0.5f;
    d->borderVertices[2].y = 0.5f;
    d->borderVertices[2].u = d->width / 4.0f;
    d->borderVertices[2].v = 0;
    d->borderVertices[3].x = d->width - 0.5f;
    d->borderVertices[3].y = d->height + 0.5f;
    d->borderVertices[3].u = d->width / 4.0f;
    d->borderVertices[3].v = d->height / 4.0f;

    d->borderVertices[4].x = 0.5f;
    d->borderVertices[4].y = d->height + 0.5f;
    d->borderVertices[5].x = 0.5f;
    d->borderVertices[5].y = d->height - d->lineEdit->height() - 0.5f;
    d->borderVertices[6].x = d->width - 0.5f;
    d->borderVertices[6].y = d->height - d->lineEdit->height() - 0.5f;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}
}

