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

namespace Rangers
{

CheckBox::CheckBox(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> checkTexture, const std::wstring &text, Widget *parent): Widget(parent)
{
    m_mainTexture = texture;
    m_checkTexture = checkTexture;
    m_checked = false;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width() + m_label.width();
    m_height = m_sprite.height();
    m_label = Label(text, this, Engine::instance()->coreFont());
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    m_label.setPosition(m_sprite.width() + 5, int(m_sprite.height() - m_label.height()) / 2);
    setText(text);
}

CheckBox::CheckBox(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> checkTexture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> hoverCheckTexture, const std::wstring &text, Widget *parent): Widget(parent)
{
    m_mainTexture = texture;
    m_checkTexture = checkTexture;
    m_hoverTexture = hoverTexture;
    m_hoverCheckTexture = hoverCheckTexture;
    m_checked = false;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width() + m_label.width() + 5;
    m_height = m_sprite.height();
    m_label = Label(text, this, Engine::instance()->coreFont());
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    m_label.setPosition(m_sprite.width() + 5, int(m_sprite.height() - m_label.height()) / 2);
    setText(text);
}

CheckBox::CheckBox(const std::wstring& texture, const std::wstring& checkTexture, const std::wstring &text, Widget *parent): Widget(parent)
{
    m_mainTexture = ResourceManager::instance()->loadTexture(texture);
    m_checkTexture = ResourceManager::instance()->loadTexture(checkTexture);
    m_checked = false;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width() + m_label.width() + 5;
    m_height = m_sprite.height();
    m_label = Label(text, this, Engine::instance()->coreFont());
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    m_label.setPosition(m_sprite.width() + 5, int(m_sprite.height() - m_label.height()) / 2);
    setText(text);
}

CheckBox::CheckBox(const std::wstring& texture, const std::wstring& checkTexture, const std::wstring& hoverTexture, const std::wstring& hoverCheckTexture, const std::wstring &text, Widget *parent): Widget(parent)
{
    m_mainTexture = ResourceManager::instance()->loadTexture(texture);
    m_checkTexture = ResourceManager::instance()->loadTexture(checkTexture);
    m_hoverTexture = ResourceManager::instance()->loadTexture(hoverTexture);
    m_hoverCheckTexture = ResourceManager::instance()->loadTexture(hoverCheckTexture);
    m_checked = false;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width() + m_label.width() + 5;
    m_height = m_sprite.height();
    m_label = Label(text, this, Engine::instance()->coreFont());
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    m_label.setPosition(m_sprite.width() + 5, int(m_sprite.height() - m_label.height()) / 2);
    setText(text);
}


void CheckBox::setChecked(bool checked)
{
    m_checked = checked;
    if (m_hoverTexture)
    {
        if (m_checked)
            m_sprite.setTexture(m_hoverCheckTexture);
        else
            m_sprite.setTexture(m_hoverTexture);
    }
    else
    {
        if (m_checked)
            m_sprite.setTexture(m_checkTexture);
        else
            m_sprite.setTexture(m_mainTexture);
    }
    markToUpdate();
}

void CheckBox::setColor(float r, float g, float b, float a)
{
    m_label.setColor(r, g, b, a);
    Object::setColor(r, g, b, a);
}

void CheckBox::setText(const std::wstring& text)
{
    m_label.setText(text);
    markToUpdate();
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
    m_sprite.draw();
    m_label.draw();
    endDraw();
}

void CheckBox::processMain()
{
    m_sprite.processMain();
    m_label.processMain();
    m_width = m_sprite.width() + m_label.width() + 5;
    m_height = m_sprite.height();
    m_label.setPosition(m_sprite.width() + 5, int(m_sprite.height() - m_label.height()) / 2);
    Widget::processMain();
}

void CheckBox::mouseEnter()
{
    if (m_hoverTexture)
    {
        if (m_checked)
            m_sprite.setTexture(m_hoverCheckTexture);
        else
            m_sprite.setTexture(m_hoverTexture);
    }
    Widget::mouseEnter();
}

void CheckBox::mouseLeave()
{
    if (m_mainTexture)
    {
        if (m_checked)
            m_sprite.setTexture(m_checkTexture);
        else
            m_sprite.setTexture(m_mainTexture);
    }
    Widget::mouseLeave();
}

void CheckBox::mouseClick(int x, int y)
{
    setChecked(!m_checked);
    action(Action(this, Action::CHECKBOX_TOGGLED, Action::Argument(m_checked)));
}
}

