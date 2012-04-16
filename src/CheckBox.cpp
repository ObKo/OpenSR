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

namespace Rangers
{

CheckBox::CheckBox(const CheckBoxStyle& style, const std::wstring &text, Widget *parent): Widget(parent),
    m_checked(false), m_normal(0), m_checkedNormal(0), m_hovered(0), m_checkedHovered(0), m_style(style)
{
    if (m_style.normal.type == ResourceDescriptor::NINEPATCH)
        m_normal = new NinePatch(boost::get<NinePatchDescriptor>(m_style.normal.resource), this);
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
        m_normal = new Sprite(boost::get<TextureRegion>(m_style.normal.resource), this);

    if (m_style.checkedNormal.type == ResourceDescriptor::NINEPATCH)
        m_checkedNormal = new NinePatch(boost::get<NinePatchDescriptor>(m_style.checkedNormal.resource), this);
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
        m_checkedNormal = new Sprite(boost::get<TextureRegion>(m_style.checkedNormal.resource), this);

    if (m_style.hovered.type == ResourceDescriptor::NINEPATCH)
        m_hovered = new NinePatch(boost::get<NinePatchDescriptor>(m_style.hovered.resource), this);
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
        m_hovered = new Sprite(boost::get<TextureRegion>(m_style.hovered.resource), this);

    if (m_style.checkedHovered.type == ResourceDescriptor::NINEPATCH)
        m_checkedHovered = new NinePatch(boost::get<NinePatchDescriptor>(m_style.checkedHovered.resource), this);
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
        m_checkedHovered = new Sprite(boost::get<TextureRegion>(m_style.checkedHovered.resource), this);

    if ((m_style.font.path != L"") && (m_style.font.size > 0))
    {
        m_label = Label(text, this, ResourceManager::instance()->loadFont(m_style.font.path, m_style.font.size));
        m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    }
    setColor(m_style.color);
    m_sprite = m_normal;
    markToUpdate();
}

CheckBox::~CheckBox()
{
    if (m_normal)
        delete m_normal;
    if (m_checkedNormal)
        delete m_checkedNormal;
    if (m_hovered)
        delete m_hovered;
    if (m_checkedHovered)
        delete m_checkedHovered;
}

void CheckBox::setColor(int color)
{
    lock();
    m_label.setColor(color);
    unlock();
}

int CheckBox::color() const
{
    return m_label.color();
}

void CheckBox::setChecked(bool checked)
{
    lock();
    m_checked = checked;
    if ((m_sprite == m_hovered) || (m_sprite == m_checkedHovered))
    {
        if (m_checked)
            m_sprite = m_checkedHovered;
        else
            m_sprite = m_hovered;
    }
    else
    {
        if (m_checked)
            m_sprite = m_checkedNormal;
        else
            m_sprite = m_normal;
    }
    unlock();
    markToUpdate();
}

void CheckBox::setText(const std::wstring& text)
{
    lock();
    m_label.setText(text);
    markToUpdate();
    unlock();
}

bool CheckBox::checked() const
{
    return m_checked;
}

std::wstring CheckBox::text() const
{
    return m_label.text();
}

void CheckBox::draw() const
{
    if (!prepareDraw())
        return;
    if (m_sprite)
        m_sprite->draw();
    m_label.draw();
    endDraw();
}

void CheckBox::processMain()
{
    lock();
    if (!m_sprite)
        return;

    if (m_sprite->needUpdate())
        m_sprite->processMain();
    if (m_label.needUpdate())
        m_label.processMain();

    m_width = m_sprite->width() + m_label.width() + 5;
    m_height = m_sprite->height();
    m_label.setPosition(m_sprite->width() + 5, int(m_sprite->height() - m_label.height()) / 2);
    unlock();
    Widget::processMain();
}

void CheckBox::mouseEnter()
{
    lock();
    if (m_hovered && m_checkedHovered)
    {
        if (m_checked)
            m_sprite = m_checkedHovered;
        else
            m_sprite = m_hovered;
    }
    unlock();
    Widget::mouseEnter();
}

void CheckBox::mouseLeave()
{
    lock();
    if (m_checked)
        m_sprite = m_checkedNormal;
    else
        m_sprite = m_normal;
    unlock();
    Widget::mouseLeave();
}

void CheckBox::mouseClick(const Vector &p)
{
    lock();
    setChecked(!m_checked);
    action(Action(this, Action::CHECKBOX_TOGGLED, Action::Argument(m_checked)));
    unlock();
}
}

