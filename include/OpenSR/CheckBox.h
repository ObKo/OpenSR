/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Widget.h"

namespace Rangers
{
class CheckBoxPrivate;
struct CheckBoxStyle;
class RANGERS_ENGINE_API CheckBox: public Widget
{
    RANGERS_DECLARE_PRIVATE(CheckBox);
public:
    CheckBox(const CheckBoxStyle& style, const std::wstring &text, Widget *parent = 0);
    virtual ~CheckBox();

    virtual void setColor(uint32_t color);
    void setChecked(bool checked);
    void setText(const std::wstring& text);

    virtual uint32_t color() const;
    bool checked() const;
    std::wstring text() const;

    virtual void draw() const;
    virtual void processMain();

protected:
    CheckBox(CheckBoxPrivate &p, Widget *parent = 0);

    RANGERS_DISABLE_COPY(CheckBox)
};
}

#endif
