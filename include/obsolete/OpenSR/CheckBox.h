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

#include "OpenSR/Button.h"

namespace Rangers
{
class CheckBoxPrivate;
struct CheckBoxStyle;
class RANGERS_ENGINE_API CheckBox: public Button
{
    RANGERS_DECLARE_PRIVATE(CheckBox);
public:
    CheckBox(boost::shared_ptr<CheckBoxStyle> style, const std::string &text);

    void setChecked(bool checked);

    bool checked() const;

    virtual void processMain();

    virtual int minWidth() const;
    virtual int minHeight() const;

protected:
    CheckBox(CheckBoxPrivate &p);

    RANGERS_DISABLE_COPY(CheckBox)
};
}

#endif
