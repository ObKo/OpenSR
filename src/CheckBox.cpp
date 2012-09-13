/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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
#include "Log.h"
#include "Action.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Engine.h"
#include "NinePatch.h"

#include "private/CheckBox_p.h"

namespace Rangers
{

CheckBox::CheckBox(const CheckBoxStyle& style, const std::wstring &text, Widget *parent): Widget(*(new CheckBoxPrivate()), parent)
{
    RANGERS_D(CheckBox);
    d->m_checked = false;
    d->m_normal = 0;
    d->m_checkedNormal = 0;
    d->m_hovered = 0;
    d->m_checkedHovered = 0;
    d->m_style = style;
    if (d->m_style.normal.type == ResourceDescriptor::NINEPATCH)
        d->m_normal = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.normal.resource), this);
    else if (d->m_style.normal.type == ResourceDescriptor::SPRITE)
        d->m_normal = new Sprite(boost::get<TextureRegion>(d->m_style.normal.resource), this);

    if (d->m_style.checkedNormal.type == ResourceDescriptor::NINEPATCH)
        d->m_checkedNormal = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.checkedNormal.resource), this);
    else if (d->m_style.normal.type == ResourceDescriptor::SPRITE)
        d->m_checkedNormal = new Sprite(boost::get<TextureRegion>(d->m_style.checkedNormal.resource), this);

    if (d->m_style.hovered.type == ResourceDescriptor::NINEPATCH)
        d->m_hovered = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.hovered.resource), this);
    else if (d->m_style.normal.type == ResourceDescriptor::SPRITE)
        d->m_hovered = new Sprite(boost::get<TextureRegion>(d->m_style.hovered.resource), this);

    if (d->m_style.checkedHovered.type == ResourceDescriptor::NINEPATCH)
        d->m_checkedHovered = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.checkedHovered.resource), this);
    else if (d->m_style.normal.type == ResourceDescriptor::SPRITE)
        d->m_checkedHovered = new Sprite(boost::get<TextureRegion>(d->m_style.checkedHovered.resource), this);

    if ((d->m_style.font.path != L"") && (d->m_style.font.size > 0))
    {
        d->m_label = Label(text, this, ResourceManager::instance()->loadFont(d->m_style.font.path, d->m_style.font.size));
        d->m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    }
    setColor(d->m_style.color);
    d->m_sprite = d->m_normal;
    markToUpdate();
}

CheckBox::~CheckBox()
{
    RANGERS_D(CheckBox);
    if (d->m_normal)
        delete d->m_normal;
    if (d->m_checkedNormal)
        delete d->m_checkedNormal;
    if (d->m_hovered)
        delete d->m_hovered;
    if (d->m_checkedHovered)
        delete d->m_checkedHovered;
}

void CheckBox::setColor(int color)
{
    lock();
    RANGERS_D(CheckBox);
    d->m_label.setColor(color);
    unlock();
}

int CheckBox::color() const
{
    RANGERS_D(const CheckBox);
    return d->m_label.color();
}

void CheckBox::setChecked(bool checked)
{
    lock();
    RANGERS_D(CheckBox);
    d->m_checked = checked;
    if ((d->m_sprite == d->m_hovered) || (d->m_sprite == d->m_checkedHovered))
    {
        if (d->m_checked)
            d->m_sprite = d->m_checkedHovered;
        else
            d->m_sprite = d->m_hovered;
    }
    else
    {
        if (d->m_checked)
            d->m_sprite = d->m_checkedNormal;
        else
            d->m_sprite = d->m_normal;
    }
    unlock();
    markToUpdate();
}

void CheckBox::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(CheckBox);
    d->m_label.setText(text);
    markToUpdate();
    unlock();
}

bool CheckBox::checked() const
{
    RANGERS_D(const CheckBox);
    return d->m_checked;
}

std::wstring CheckBox::text() const
{
    RANGERS_D(const CheckBox);
    return d->m_label.text();
}

void CheckBox::draw() const
{
    RANGERS_D(const CheckBox);
    if (!prepareDraw())
        return;
    if (d->m_sprite)
        d->m_sprite->draw();
    d->m_label.draw();
    endDraw();
}

void CheckBox::processMain()
{
    lock();
    RANGERS_D(CheckBox);
    if (!d->m_sprite)
        return;

    if (d->m_sprite->needUpdate())
        d->m_sprite->processMain();
    if (d->m_label.needUpdate())
        d->m_label.processMain();

    d->m_width = d->m_sprite->width() + d->m_label.width() + 5;
    d->m_height = d->m_sprite->height();
    d->m_label.setPosition(d->m_sprite->width() + 5, int(d->m_sprite->height() - d->m_label.height()) / 2);
    unlock();
    Widget::processMain();
}

void CheckBox::mouseEnter()
{
    lock();
    RANGERS_D(CheckBox);
    if (d->m_hovered && d->m_checkedHovered)
    {
        if (d->m_checked)
            d->m_sprite = d->m_checkedHovered;
        else
            d->m_sprite = d->m_hovered;
    }
    unlock();
    Widget::mouseEnter();
}

void CheckBox::mouseLeave()
{
    lock();
    RANGERS_D(CheckBox);
    if (d->m_checked)
        d->m_sprite = d->m_checkedNormal;
    else
        d->m_sprite = d->m_normal;
    unlock();
    Widget::mouseLeave();
}

void CheckBox::mouseClick(const Vector &p)
{
    lock();
    RANGERS_D(CheckBox);
    setChecked(!d->m_checked);
    action(Action(this, Action::CHECKBOX_TOGGLED, Action::Argument(d->m_checked)));
    unlock();
}
}

