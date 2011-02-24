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

#ifndef RANGERS_LINEEDITWIDGET_H
#define RANGERS_LINEEDITWIDGET_H

#include "Label.h"
#include "Widget.h"

namespace Rangers
{
class LineEditWidget: public Widget
{
public:
    LineEditWidget(float w, float h = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>(), Object* parent = 0);
    LineEditWidget(Object* parent = 0);
    LineEditWidget(const LineEditWidget& other);

    void draw() const;
    void processMain();
    void processLogic(int dt);

    void keyPressed(SDL_keysym key);

    std::wstring text() const;
    void setText(const std::wstring &s);
    
    LineEditWidget& operator=(const LineEditWidget& other);

private:
    Label label;
    std::wstring editText;
    Vertex *border;
    GLuint borderBuffer;
    int position;
    bool cursorVisible;
    int cursorTime;
};
};

#endif
