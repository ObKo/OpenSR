/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/CheckBox.h"
#include "OpenSR/Sprite.h"
#include "OpenSR/Action.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"
#include "OpenSR/NinePatch.h"
#include "OpenSR/Label.h"

#include "OpenSR/private/CheckBox_p.h"

namespace Rangers
{
CheckBoxPrivate::CheckBoxPrivate()
{
    checked = false;
    checkBoxListener = boost::shared_ptr<CheckBoxListener>(new CheckBoxListener());
}

void CheckBoxPrivate::CheckBoxListener::actionPerformed(const Action& action)
{
    boost::shared_ptr<CheckBox> q = boost::static_pointer_cast<CheckBox>(action.source());
    CheckBoxPrivate *d = q->d_func();
    switch (action.type())
    {
    case Action::BUTTON_CLICKED:
        q->setChecked(!d->checked);
        q->action(Action(q, Action::CHECKBOX_TOGGLED, Action::Argument(d->checked)));
        break;
    }
}

CheckBox::CheckBox(boost::shared_ptr<CheckBoxStyle> style, const std::string &text): Button(*(new CheckBoxPrivate()))
{
    RANGERS_D(CheckBox);
    d->checkBoxStyle = style;

    if (style)
    {
        d->uncheckedNormal = d->spriteFromStyleObject(style->normal);
        if (d->uncheckedNormal)
            addChild(d->uncheckedNormal);

        d->checkedNormal = d->spriteFromStyleObject(style->checkedNormal);
        if (d->checkedNormal)
            addChild(d->checkedNormal);

        d->uncheckedHovered = d->spriteFromStyleObject(style->hovered);
        if (d->uncheckedHovered)
            addChild(d->uncheckedHovered);

        d->checkedHovered = d->spriteFromStyleObject(style->checkedHovered);
        if (d->checkedHovered)
            addChild(d->checkedHovered);

        if ((d->checkBoxStyle->font) && (d->checkBoxStyle->font->path != "") && (d->checkBoxStyle->font->size > 0))
        {
            d->label->setFont(ResourceManager::instance().loadFont(d->checkBoxStyle->font->path, d->checkBoxStyle->font->size));
        }
        setColor(d->checkBoxStyle->color);
    }

    d->normalSprite = d->uncheckedNormal;
    d->hoverSprite = d->uncheckedHovered;
    d->sprite = d->normalSprite;

    setText(text);

    d->width = minWidth();
    d->height = minHeight();

    addListener(d->checkBoxListener);
    markToUpdate();
}

void CheckBox::setChecked(bool checked)
{
    lock();
    RANGERS_D(CheckBox);
    d->checked = checked;

    if (checked)
    {
        boost::shared_ptr<Sprite> oldSprite = d->normalSprite;

        d->normalSprite = d->checkedNormal;
        d->hoverSprite = d->checkedHovered;

        if (d->sprite == oldSprite)
            d->sprite = d->normalSprite;
        else
            d->sprite = d->hoverSprite;
    }
    else
    {
        boost::shared_ptr<Sprite> oldSprite = d->normalSprite;

        d->normalSprite = d->uncheckedNormal;
        d->hoverSprite = d->uncheckedHovered;

        if (d->sprite == oldSprite)
            d->sprite = d->normalSprite;
        else
            d->sprite = d->hoverSprite;
    }

    unlock();
    markToUpdate();
}

bool CheckBox::checked() const
{
    RANGERS_D(const CheckBox);
    return d->checked;
}

int CheckBox::minWidth() const
{
    RANGERS_D(const CheckBox);
    return d->sprite->width() + d->label->width() + 5;
}

int CheckBox::minHeight() const
{
    RANGERS_D(const CheckBox);
    return std::max(d->sprite->height(), d->label->height());
}

void CheckBox::processMain()
{
    lock();
    RANGERS_D(CheckBox);
    if (!d->sprite)
        return;

    d->label->setPosition(d->sprite->width() + 5, int(d->sprite->height() - d->label->height()) / 2);
    d->width = minWidth();
    d->height = minHeight();
    unlock();
}
}

