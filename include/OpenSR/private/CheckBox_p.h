/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 -2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/ActionListener.h"
#include "OpenSR/Label.h"
#include "OpenSR/Styles.h"

namespace Rangers
{
class CheckBoxPrivate: public WidgetPrivate
{
    RANGERS_DECLARE_PUBLIC(CheckBox)
public:
    class CheckBoxListener: public ActionListener
    {
    public:
        virtual void actionPerformed(const Action& action);
    };

    CheckBoxPrivate();

    bool checked;
    boost::shared_ptr<Sprite> normal;
    boost::shared_ptr<Sprite> checkedNormal;
    boost::shared_ptr<Sprite> hovered;
    boost::shared_ptr<Sprite> checkedHovered;
    boost::shared_ptr<Sprite> sprite;
    boost::shared_ptr<Label> label;
    CheckBoxStyle style;

    boost::shared_ptr<CheckBoxListener> checkBoxListener;
};
}

#endif