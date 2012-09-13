/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LINE_EDIT_WIDGET_P_H
#define RANGERS_LINE_EDIT_WIDGET_P_H

#include "Widget_p.h"

#include "Label.h"
#include "Styles.h"

namespace Rangers
{
class LineEditWidgetPrivate: public WidgetPrivate
{
    RANGERS_DECLARE_PUBLIC(LineEditWidget)
public:    
    Label m_label;
    std::wstring m_text;
    Vertex *m_cursorVertices;
    GLuint m_cursorBuffer;
    int m_position;
    bool m_cursorVisible;
    int m_cursorTime;
    int m_stringOffset;

    Sprite *m_background;
    LineEditStyle m_style;
};
}

#endif