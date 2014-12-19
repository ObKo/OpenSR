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

#ifndef RANGERS_RADIOBUTTON_GROUP_P_H
#define RANGERS_RADIOBUTTON_GROUP_P_H

#include "WidgetNode_p.h"

#include "OpenSR/ActionListener.h"

#include <list>

namespace Rangers
{
class RadioButtonGroup;
class RadioButton;
class RadioButtonGroupPrivate: public WidgetNodePrivate
{
    RANGERS_DECLARE_PUBLIC(RadioButtonGroup)

    class RadioButtonGroupListener: public ActionListener
    {
    public:
        virtual void actionPerformed(const Action& action);
        RadioButtonGroupPrivate *d;
    };

    RadioButtonGroupPrivate();

    std::list<boost::shared_ptr<RadioButton> > radioButtons;
    boost::shared_ptr<RadioButtonGroupListener> groupListener;
};
}

#endif