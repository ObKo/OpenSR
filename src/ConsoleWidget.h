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

#ifndef RANGERS_CONSOLEWIDGET_H
#define RANGERS_CONSOLEWIDGET_H

#include "LineEditWidget.h"
#include "ColorLabel.h"
#include <vector>

namespace Rangers
{
//! Console widget
class ConsoleWidget: public Widget
{
public:
    ConsoleWidget(float w, float h, Widget* parent = 0);
    ConsoleWidget(const ConsoleWidget& other);
    ConsoleWidget(Widget *parent = 0);

    virtual void draw() const;
    virtual void processMain();
    virtual void processLogic(int dt);

    ConsoleWidget& operator=(const ConsoleWidget& other);

private:
    class ConsoleWidgetListener;
    LineEditWidget m_lineEdit;
    ColorLabel m_logLabel;
    Vertex *m_borderVertices;
    GLuint m_borderBuffer;
    int m_consoleLines;

    int m_historyPosition;
    std::vector<std::wstring> m_commandHistory;
};
}

#endif // CONSOLEWIDGET_H
