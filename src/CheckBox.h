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

#ifndef RANGERS_CHECKBOX_H
#define RANGERS_CHECKBOX_H

#include "Widget.h"
#include "Label.h"
#include "Styles.h"

namespace Rangers
{
class RANGERS_ENGINE_API CheckBox: public Widget
{
public:
    CheckBox(const CheckBoxStyle& style, const std::wstring &text, Widget *parent = 0);
    virtual ~CheckBox();

    virtual void setColor(int color);
    void setChecked(bool checked);
    void setText(const std::wstring& text);

    virtual int color() const;
    bool checked() const;
    std::wstring text() const;

    virtual void draw() const;
    virtual void processMain();

    virtual void mouseEnter();
    virtual void mouseLeave();

    virtual void mouseClick(const Vector &p);



private:
    bool m_checked;
    Sprite *m_normal;
    Sprite *m_checkedNormal;
    Sprite *m_hovered;
    Sprite *m_checkedHovered;
    Sprite *m_sprite;
    Label m_label;
    CheckBoxStyle m_style;
};
}

#endif
