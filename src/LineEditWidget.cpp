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

using namespace Rangers;

void LineEditWidget::draw() const
{
	lock();
    prepareDraw();
    label.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, borderBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glLineWidth(1);

    if (cursorVisible)
        glDrawArrays(GL_LINES, 0, 10);
    else
        glDrawArrays(GL_LINES, 0, 8);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
    unlock();
}

LineEditWidget::LineEditWidget(float w, float h, boost::shared_ptr< Font > font, Object* parent):
        Widget(parent)
{
    if (!font)
        font = Engine::instance()->defaultFont();
    label = Label(L"", this, font, POSITION_X_LEFT, POSITION_Y_TOP);
    widgetHeight = h > font->size() + 4 ? h : font->size() + 4;
    widgetWidth = w;
    borderBuffer = 0;
    label.setPosition(5, 2);
    position = 0;
    cursorTime = 0;
    cursorVisible = true;
    markToUpdate();
}

LineEditWidget::LineEditWidget(Object* parent): Widget(parent)
{
    border = 0;
    borderBuffer = 0;
    position = 0;
    cursorVisible = false;
    cursorTime = 0;
}

LineEditWidget::LineEditWidget(const Rangers::LineEditWidget& other): Widget(other)
{
    label = other.label;
    editText = other.editText;
    border = 0;
    borderBuffer = 0;
    position = 0;
    cursorVisible = false;
    cursorTime = 0;
}

LineEditWidget& LineEditWidget::operator=(const Rangers::LineEditWidget& other)
{
    if(this == &other)
        return *this;
    
    label = other.label;
    editText = other.editText;
    border = 0;
    borderBuffer = 0;
    position = 0;
    cursorVisible = false;
    cursorTime = 0;
    
    ::Widget::operator=(other);
    return *this;
}



void LineEditWidget::processMain()
{
    lock();
    int cursorPosition = label.font()->getWidth(editText.begin(), editText.begin() + position) + 6;
    if (!borderBuffer)
    {
        border = new Vertex[10];
        glGenBuffers(1, &borderBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, borderBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 10, border, GL_DYNAMIC_DRAW);
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
    border[3].y = widgetHeight - 1;
    border[4].x = widgetWidth - 1;
    border[4].y = widgetHeight - 1;
    border[5].x = 1;
    border[5].y = widgetHeight - 1;
    border[6].x = 1;
    border[6].y = widgetHeight - 1;
    border[7].x = 1;
    border[7].y = 1;
    border[8].x = cursorPosition;
    border[8].y = widgetHeight - 2;
    border[9].x = cursorPosition;
    border[9].y = 2;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    Widget::processMain();
    unlock();
}

void LineEditWidget::processLogic(int dt)
{
    cursorTime += dt;
    if (cursorTime > 500)
    {
        cursorVisible = !cursorVisible;
        cursorTime = 0;
    }
}


void LineEditWidget::keyPressed(SDL_keysym key)
{
    lock();

    if (key.sym == SDLK_RETURN);
    else if (key.sym == SDLK_LEFT)
        position--;
    else if (key.sym == SDLK_RIGHT)
        position++;
    else if (key.sym == SDLK_ESCAPE)
        editText.clear();
    else if (key.sym == SDLK_BACKSPACE)
    {
        if (editText.size())
            if (position > 0)
                editText.erase(position - 1, 1);
        position--;
    }
    else if (key.sym == SDLK_DELETE)
    {
        if (editText.size())
            if (position < editText.length())
                editText.erase(position, 1);
    }
    else if (key.unicode && !(key.mod & (KMOD_ALT | KMOD_META | KMOD_CTRL)))
    {
        if (key.unicode == '\t')
            editText.insert(position, 1, ' ');
        else
            editText.insert(position, 1, key.unicode);
        position++;
    }
    if (position < 0) position = 0;
    if (position > editText.length()) position = editText.length();
    label.setText(editText);
    markToUpdate();

    unlock();
}

void LineEditWidget::setText(const std::wstring& s)
{
    editText = s;
    position = s.length();
    label.setText(s);
    markToUpdate();
}

std::wstring LineEditWidget::text() const
{
    return editText;
}
