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

#ifndef RANGERS_WIDGET_H
#define RANGERS_WIDGET_H

#include "Object.h"

namespace Rangers
{
class ActionListener;
class Action;
class Widget: public Object
{
public:
    Widget(Widget *parent = 0);
    Widget(float w, float h, Widget *parent = 0);
    Widget(const Widget& other);

    virtual ~Widget();

    virtual Rect getBoundingRect() const;

    virtual void mouseEnter();
    virtual void mouseLeave();

    void mouseMove(float x, float y);
    void mouseDown(uint8_t key, float x, float y);
    void mouseUp(uint8_t key, float x, float y);
    void mouseClick(float x, float y);

    virtual void mouseMove(const Vector &p);
    virtual void mouseDown(uint8_t key, const Vector &p);
    virtual void mouseUp(uint8_t key, const Vector &p);
    virtual void mouseClick(const Vector &p);

    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual int preferredWidth() const;
    virtual int preferredHeight() const;
    virtual int maxWidth() const;
    virtual int maxHeight() const;

    int width() const;
    int height() const;

    void setWidth(int width);
    void setHeight(int height);
    void setGeometry(int width, int height);

    Widget& operator=(const Widget& other);

    void addWidget(Widget *w);
    void removeWidget(Widget *w);

    void addListener(ActionListener* listener);
    void removeListener(ActionListener* listener);

    void action(const Action& action);

protected:
    std::list<Widget *> m_childWidgets;
    std::list<ActionListener*> m_listeners;
    Widget *m_currentChild;
    int m_width, m_height;
    bool m_leftMouseButtonPressed;
};
}

#endif // WIDGET_H
