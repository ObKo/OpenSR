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

#include "CheckBox.h"
#include "Sprite.h"
#include "Action.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "NinePatch.h"

#include "private/CheckBox_p.h"

namespace Rangers
{
CheckBoxPrivate::CheckBoxPrivate()
{
    checked = false;
    normal = 0;
    checkedNormal = 0;
    hovered = 0;
    checkedHovered = 0;
    sprite = 0;
    label = 0;
}

void CheckBoxPrivate::actionPerformed(const Action& action)
{
    RANGERS_Q(CheckBox);
    switch (action.type())
    {
    case Action::MOUSE_ENTER:
        if (hovered && checkedHovered)
        {
            if (checked)
                sprite = checkedHovered;
            else
                sprite = hovered;
        }
        break;

    case Action::MOUSE_LEAVE:
        if (checked)
            sprite = checkedNormal;
        else
            sprite = normal;
        break;

    case Action::MOUSE_CLICK:
        q->setChecked(!checked);
        q->action(Action(q, Action::CHECKBOX_TOGGLED, Action::Argument(checked)));
        break;
    }
}

CheckBox::CheckBox(const CheckBoxStyle& style, const std::wstring &text, Widget *parent): Widget(*(new CheckBoxPrivate()), parent)
{
    RANGERS_D(CheckBox);
    d->style = style;
    if (d->style.normal.type == ResourceDescriptor::NINEPATCH)
        d->normal = new NinePatch(boost::get<NinePatchDescriptor>(d->style.normal.resource), this);
    else if (d->style.normal.type == ResourceDescriptor::SPRITE)
        d->normal = new Sprite(boost::get<TextureRegion>(d->style.normal.resource), this);

    if (d->style.checkedNormal.type == ResourceDescriptor::NINEPATCH)
        d->checkedNormal = new NinePatch(boost::get<NinePatchDescriptor>(d->style.checkedNormal.resource), this);
    else if (d->style.normal.type == ResourceDescriptor::SPRITE)
        d->checkedNormal = new Sprite(boost::get<TextureRegion>(d->style.checkedNormal.resource), this);

    if (d->style.hovered.type == ResourceDescriptor::NINEPATCH)
        d->hovered = new NinePatch(boost::get<NinePatchDescriptor>(d->style.hovered.resource), this);
    else if (d->style.normal.type == ResourceDescriptor::SPRITE)
        d->hovered = new Sprite(boost::get<TextureRegion>(d->style.hovered.resource), this);

    if (d->style.checkedHovered.type == ResourceDescriptor::NINEPATCH)
        d->checkedHovered = new NinePatch(boost::get<NinePatchDescriptor>(d->style.checkedHovered.resource), this);
    else if (d->style.normal.type == ResourceDescriptor::SPRITE)
        d->checkedHovered = new Sprite(boost::get<TextureRegion>(d->style.checkedHovered.resource), this);

    if ((d->style.font.path != L"") && (d->style.font.size > 0))
    {
        d->label = new Label(text, this, ResourceManager::instance().loadFont(d->style.font.path, d->style.font.size));
    }
    else
    {
        d->label = new Label(this);
    }
    d->label->setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    setColor(d->style.color);
    d->sprite = d->normal;

    addListener(d);
    markToUpdate();
}

CheckBox::~CheckBox()
{
    RANGERS_D(CheckBox);
    if (d->normal)
        delete d->normal;
    if (d->checkedNormal)
        delete d->checkedNormal;
    if (d->hovered)
        delete d->hovered;
    if (d->checkedHovered)
        delete d->checkedHovered;
    delete d->label;
}

void CheckBox::setColor(int color)
{
    lock();
    RANGERS_D(CheckBox);
    d->label->setColor(color);
    unlock();
}

int CheckBox::color() const
{
    RANGERS_D(const CheckBox);
    return d->label->color();
}

void CheckBox::setChecked(bool checked)
{
    lock();
    RANGERS_D(CheckBox);
    d->checked = checked;
    if ((d->sprite == d->hovered) || (d->sprite == d->checkedHovered))
    {
        if (d->checked)
            d->sprite = d->checkedHovered;
        else
            d->sprite = d->hovered;
    }
    else
    {
        if (d->checked)
            d->sprite = d->checkedNormal;
        else
            d->sprite = d->normal;
    }
    unlock();
    markToUpdate();
}

void CheckBox::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(CheckBox);
    d->label->setText(text);
    markToUpdate();
    unlock();
}

bool CheckBox::checked() const
{
    RANGERS_D(const CheckBox);
    return d->checked;
}

std::wstring CheckBox::text() const
{
    RANGERS_D(const CheckBox);
    return d->label->text();
}

void CheckBox::draw() const
{
    RANGERS_D(const CheckBox);
    if (!prepareDraw())
        return;
    if (d->sprite)
        d->sprite->draw();
    d->label->draw();
    endDraw();
}

void CheckBox::processMain()
{
    lock();
    RANGERS_D(CheckBox);
    if (!d->sprite)
        return;

    if (d->sprite->needUpdate())
        d->sprite->processMain();
    if (d->label->needUpdate())
        d->label->processMain();

    d->width = d->sprite->width() + d->label->width() + 5;
    d->height = d->sprite->height();
    d->label->setPosition(d->sprite->width() + 5, int(d->sprite->height() - d->label->height()) / 2);
    unlock();
    Widget::processMain();
}
}

