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

#ifndef RANGERS_RADIOBUTTON_GROUP_H
#define RANGERS_RADIOBUTTON_GROUP_H

#include "OpenSR/WidgetNode.h"

namespace Rangers
{
class RadioButton;
class RadioButtonGroupPrivate;
class RANGERS_ENGINE_API RadioButtonGroup: public WidgetNode
{
    RANGERS_DECLARE_PRIVATE(RadioButtonGroup)
public:
    RadioButtonGroup();

    void addRadioButton(boost::shared_ptr<RadioButton> button);
    void removeRadioButton(boost::shared_ptr<RadioButton> button);

    void selectRadioButton(boost::shared_ptr<RadioButton> button);

protected:
    RadioButtonGroup(RadioButtonGroupPrivate &p);

    RANGERS_DISABLE_COPY(RadioButtonGroup)
};
}

#endif
