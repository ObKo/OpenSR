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

#ifndef RANGERS_RADIOBUTTON_P_H
#define RANGERS_RADIOBUTTON_P_H

#include "Button_p.h"

#include "OpenSR/ActionListener.h"
#include "OpenSR/Styles.h"

namespace Rangers
{
class RadioButtonPrivate: public ButtonPrivate
{
    RANGERS_DECLARE_PUBLIC(RadioButton)
public:
    RadioButtonPrivate();

    bool selected;
    boost::shared_ptr<Sprite> selectedNormal;
    boost::shared_ptr<Sprite> selectedHovered;
    boost::shared_ptr<Sprite> deselectedNormal;
    boost::shared_ptr<Sprite> deselectedHovered;
    boost::shared_ptr<RadioButtonStyle> radioButtonStyle;
};
}

#endif