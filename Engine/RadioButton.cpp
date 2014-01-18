/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/RadioButton.h"
#include "OpenSR/Sprite.h"
#include "OpenSR/Action.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"
#include "OpenSR/NinePatch.h"
#include "OpenSR/Label.h"

#include "OpenSR/private/RadioButton_p.h"

namespace Rangers
{
RadioButtonPrivate::RadioButtonPrivate()
{
    selected = false;
}

RadioButton::RadioButton(const RadioButtonStyle& style, const std::wstring &text): Button(*(new RadioButtonPrivate()))
{
    RANGERS_D(RadioButton);
    d->radioButtonStyle = style;

    d->deselectedNormal = d->spriteFromStyleObject(style.normal);
    if (d->deselectedNormal)
        addChild(d->deselectedNormal);

    d->selectedNormal = d->spriteFromStyleObject(style.selectedNormal);
    if (d->selectedNormal)
        addChild(d->selectedNormal);

    d->deselectedHovered = d->spriteFromStyleObject(style.hovered);
    if (d->deselectedHovered)
        addChild(d->deselectedHovered);

    d->selectedHovered = d->spriteFromStyleObject(style.selectedHovered);
    if (d->selectedHovered)
        addChild(d->selectedHovered);

    if ((d->radioButtonStyle.font) && (d->radioButtonStyle.font->path != L"") && (d->radioButtonStyle.font->size > 0))
    {
        d->label->setFont(ResourceManager::instance().loadFont(d->radioButtonStyle.font->path, d->radioButtonStyle.font->size));
    }
    setColor(d->radioButtonStyle.color);

    d->normalSprite = d->deselectedNormal;
    d->hoverSprite = d->deselectedHovered;
    d->sprite = d->normalSprite;

    setText(text);

    d->width = minWidth();
    d->height = minHeight();

    markToUpdate();
}

void RadioButton::select()
{
    lock();
    RANGERS_D(RadioButton);

    if (d->selected)
    {
        unlock();
        return;
    }

    d->selected = true;

    boost::shared_ptr<Sprite> oldSprite = d->normalSprite;

    d->normalSprite = d->selectedNormal;
    d->hoverSprite = d->selectedHovered;

    if (d->sprite == oldSprite)
        d->sprite = d->normalSprite;
    else
        d->sprite = d->hoverSprite;

    unlock();
    markToUpdate();
}

void RadioButton::deselect()
{
    lock();
    RANGERS_D(RadioButton);

    if (!d->selected)
    {
        unlock();
        return;
    }

    d->selected = false;

    boost::shared_ptr<Sprite> oldSprite = d->normalSprite;

    d->normalSprite = d->deselectedNormal;
    d->hoverSprite = d->deselectedHovered;

    if (d->sprite == oldSprite)
        d->sprite = d->normalSprite;
    else
        d->sprite = d->hoverSprite;

    unlock();
    markToUpdate();
}

bool RadioButton::isSelected() const
{
    RANGERS_D(const RadioButton);
    return d->selected;
}

int RadioButton::minWidth() const
{
    RANGERS_D(const RadioButton);
    return d->sprite->width() + d->label->width() + 5;
}

int RadioButton::minHeight() const
{
    RANGERS_D(const RadioButton);
    return std::max(d->sprite->height(), d->label->height());
}

void RadioButton::processMain()
{
    Button::processMain();

    lock();
    RANGERS_D(RadioButton);

    if (!d->sprite)
        return;

    d->label->setPosition(d->sprite->width() + 5, int(d->sprite->height() - d->label->height()) / 2);
    d->width = minWidth();
    d->height = minHeight();
    unlock();
}
}

