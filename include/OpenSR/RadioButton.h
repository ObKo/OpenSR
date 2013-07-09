/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_RADIOBUTTON_H
#define RANGERS_RADIOBUTTON_H

#include "OpenSR/Button.h"

namespace Rangers
{
class RadioButtonPrivate;
struct RadioButtonStyle;
class RANGERS_ENGINE_API RadioButton: public Button
{
    RANGERS_DECLARE_PRIVATE(RadioButton);
public:
    RadioButton(const RadioButtonStyle& style, const std::wstring &text);

    void select();
    void deselect();
    bool isSelected() const;

    virtual void processMain();

    virtual int minWidth() const;
    virtual int minHeight() const;

protected:
    RadioButton(RadioButtonPrivate &p);

    RANGERS_DISABLE_COPY(RadioButton)
};
}

#endif
