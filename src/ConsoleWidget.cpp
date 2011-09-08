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

using namespace Rangers;

ConsoleWidget::ConsoleWidget(float w, float h, Object* parent): Widget(w, h, parent)
{
    int editSize = Engine::instance()->serviceFont()->size() + 4;
    lineEdit = LineEditWidget(w, editSize, Engine::instance()->serviceFont(), this);
    logLabel = ColorLabel("", this, Engine::instance()->serviceFont(), POSITION_X_LEFT, POSITION_Y_TOP);
    logLabel.setPosition(4, 4);
    logLabel.setFixedSize(w - 8, h - editSize - 8);
    lineEdit.setPosition(0, h - editSize);
    lines = (h - editSize - 8) / Engine::instance()->serviceFont()->size();
    border = 0;
    borderBuffer = 0;
    markToUpdate();
    currentHistoryPosition = -1;
}

ConsoleWidget::ConsoleWidget(const Rangers::ConsoleWidget& other): Widget(other)
{
    lineEdit = other.lineEdit;
    logLabel = other.logLabel;
    border = 0;
    borderBuffer = 0;
    lines = other.lines;

    currentHistoryPosition = other.currentHistoryPosition;
    history = other.history;
    markToUpdate();
}

ConsoleWidget::ConsoleWidget(Object* parent): Widget(parent)
{
    border = 0;
    borderBuffer = 0;
    currentHistoryPosition = -1;
}


ConsoleWidget& ConsoleWidget::operator=(const Rangers::ConsoleWidget& other)
{
    if(this == &other)
        return *this;
    
    lineEdit = other.lineEdit;
    logLabel = other.logLabel;
    border = 0;
    borderBuffer = 0;
    lines = other.lines;

    currentHistoryPosition = other.currentHistoryPosition;
    history = other.history;
    markToUpdate();
    
    ::Widget::operator=(other);    
    return *this;
}



void ConsoleWidget::draw()
{
    if(!prepareDraw())
        return;
    
    lineEdit.draw();
    logLabel.draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, borderBuffer);

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
        if (lineEdit.text() != L"")
        {
            history.push_back(lineEdit.text());
            currentHistoryPosition = -1;
            Engine::instance()->execCommand(lineEdit.text());
            lineEdit.setText(L"");
        }
    }
    else
        if (key.sym == SDLK_UP)
        {
            if (currentHistoryPosition == -1 && lineEdit.text() != L"")
            {
                history.push_back(lineEdit.text());
                currentHistoryPosition = history.size() - 1;
            }

            if ((currentHistoryPosition == -1) && (history.size() > 0))
                currentHistoryPosition = history.size();

            if (currentHistoryPosition > 0)
            {
                currentHistoryPosition--;
                lineEdit.setText(history[currentHistoryPosition]);
            }

            //markToUpdate();
        }
        else
            if (key.sym == SDLK_DOWN)
            {
                if (currentHistoryPosition < history.size() - 1)
                {
                    currentHistoryPosition++;
                    lineEdit.setText(history[currentHistoryPosition]);
                }
                else
                {
                    currentHistoryPosition = -1;
                    lineEdit.setText(L"");
                }

                //markToUpdate();
            }
            else
                lineEdit.keyPressed(key);
}

void ConsoleWidget::processLogic(int dt)
{
    lineEdit.processLogic(dt);

    if (logger().checkForUpdate())
    {
        std::list<LogEntry> l = logger().lines(lines);
        std::wstring text;

        for (std::list<LogEntry>::const_iterator i = l.begin(); i != l.end(); i++)
        {
            const LogEntry& e = *i;

            switch (e.level)
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

            text += e.text + L"\\cR\n";
        }

        logLabel.setText(text);
    }

    Widget::processLogic(dt);
}

void ConsoleWidget::processMain()
{
    lock();

    if (!borderBuffer)
    {
        border = new Vertex[6];
        glGenBuffers(1, &borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, border, GL_DYNAMIC_DRAW);
        delete border;
    }

    glBindBuffer(GL_ARRAY_BUFFER, borderBuffer);

    border = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    border[0].x = 1;
    border[0].y = 1;
    border[1].x = widgetWidth - 1;
    border[1].y = 1;
    border[2].x = widgetWidth - 1;
    border[2].y = 1;
    border[3].x = widgetWidth - 1;
    border[3].y = widgetHeight - lineEdit.height();
    border[4].x = 1;
    border[4].y = 1;
    border[5].x = 1;
    border[5].y = widgetHeight - lineEdit.height();

    glUnmapBuffer(GL_ARRAY_BUFFER);

    lineEdit.processMain();
    logLabel.processMain();
    unlock();
    Widget::processMain();
}

