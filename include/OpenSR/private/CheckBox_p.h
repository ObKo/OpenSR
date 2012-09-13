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

#ifndef RANGERS_CHECKBOX_P_H
#define RANGERS_CHECKBOX_P_H

#include "Widget_p.h"

#include "Label.h"

namespace Rangers
{
class CheckBoxPrivate: public WidgetPrivate
{
    RANGERS_DECLARE_PUBLIC(CheckBox)
public:
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