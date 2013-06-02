/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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
#include "ActionListener.h"

namespace Rangers
{
class LineEditWidgetPrivate: public WidgetPrivate
{
    RANGERS_DECLARE_PUBLIC(LineEditWidget)
public:
    LineEditWidgetPrivate();

    class LineEditWidgetListener: public ActionListener
    {
    public:
        virtual void actionPerformed(const Action &action);
    };

    boost::shared_ptr<Label> label;
    std::wstring text;
    Vertex *cursorVertices;
    GLuint cursorBuffer;
    int position;
    bool cursorVisible;
    int cursorTime;
    int stringOffset;

    boost::shared_ptr<Sprite> background;
    LineEditStyle style;

    Vector mousePosition;

    void init();
    void updateText();
    void keyPressed(const SDL_keysym& key);

    boost::shared_ptr<LineEditWidgetListener> lineEditListener;
};
}

#endif