/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/RadioButtonGroup.h"
#include "OpenSR/private/RadioButtonGroup_p.h"

#include "OpenSR/RadioButton.h"
#include "OpenSR/Action.h"

namespace Rangers
{
RadioButtonGroupPrivate::RadioButtonGroupPrivate()
{
    groupListener = boost::shared_ptr<RadioButtonGroupListener>(new RadioButtonGroupListener());
    groupListener->d = this;
}

void RadioButtonGroupPrivate::RadioButtonGroupListener::actionPerformed(const Action& action)
{
    if(action.type() != Action::BUTTON_CLICKED)
        return;
    
    boost::shared_ptr<RadioButton> button = boost::dynamic_pointer_cast<RadioButton>(action.source());
    if (!button)
        return;
    
    //FIXME: Ugly
    //auto index = std::find(d->radioButtons.begin(), d->radioButtons.end(), button);
    //if(index == d->radioButtons.end())
    //    return;
    
    if(button->isSelected())
        return;

    d->q_func()->selectRadioButton(button);
    button->action(Action(button, Action::RADIOBUTTON_SWITCHED));
}

RadioButtonGroup::RadioButtonGroup(): WidgetNode(*(new RadioButtonGroupPrivate()))
{
}

RadioButtonGroup::RadioButtonGroup(RadioButtonGroupPrivate &p): WidgetNode(p)
{
}

void RadioButtonGroup::addRadioButton(boost::shared_ptr<RadioButton> button)
{
    RANGERS_D(RadioButtonGroup);

    if (!button)
        return;

    d->radioButtons.push_back(button);
    button->addListener(d->groupListener);
    addWidget(button);

    if (d->radioButtons.size() == 1)
        button->select();
}

void RadioButtonGroup::removeRadioButton(boost::shared_ptr<RadioButton> button)
{
    RANGERS_D(RadioButtonGroup);

    if (!button)
        return;

    d->radioButtons.remove(button);
    button->removeListener(d->groupListener);
    removeWidget(button);
}

void RadioButtonGroup::selectRadioButton(boost::shared_ptr<RadioButton> button)
{
    RANGERS_D(RadioButtonGroup);

    if (!button)
        return;

    bool founded = false;
    std::list<boost::shared_ptr<RadioButton> >::const_iterator end = d->radioButtons.end();
    for (std::list<boost::shared_ptr<RadioButton> >::const_iterator i = d->radioButtons.begin(); i != end; ++i)
    {
        if (*i == button)
            founded = true;
    }
    if (!founded)
        return;

    for (std::list<boost::shared_ptr<RadioButton> >::const_iterator i = d->radioButtons.begin(); i != end; ++i)
    {
        (*i)->deselect();
    }
    button->select();
}

}
