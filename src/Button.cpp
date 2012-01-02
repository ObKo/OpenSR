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

#include "Button.h"
#include "Sprite.h"
#include "Log.h"
#include "Action.h"
#include "ResourceManager.h"
#include "Texture.h"

namespace Rangers
{

Button::Button(Widget *parent): Widget(parent)
{
}

Button::Button(boost::shared_ptr<Texture> texture, Widget *parent): Widget(parent)
{
    m_mainTexture = texture;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width();
    m_height = m_sprite.height();
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent): Widget(parent)
{
    m_mainTexture = texture;
    m_hoverTexture = hoverTexture;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width();
    m_height = m_sprite.height();
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent): Widget(parent)
{
    m_mainTexture = texture;
    m_hoverTexture = hoverTexture;
    m_pressedTexture = pressTexture;
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width();
    m_height = m_sprite.height();
}

Button::Button(const std::wstring& texture, Widget *parent): Widget(parent)
{
    m_mainTexture = ResourceManager::instance()->loadTexture(texture);
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width();
    m_height = m_sprite.height();
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, Widget *parent): Widget(parent)
{
    m_mainTexture = ResourceManager::instance()->loadTexture(texture);
    m_hoverTexture = ResourceManager::instance()->loadTexture(hoverTexture);
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width();
    m_height = m_sprite.height();
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture, Widget *parent): Widget(parent)
{
    m_mainTexture = ResourceManager::instance()->loadTexture(texture);
    m_hoverTexture = ResourceManager::instance()->loadTexture(hoverTexture);
    m_pressedTexture = ResourceManager::instance()->loadTexture(pressTexture);
    m_sprite = Sprite(m_mainTexture, this);
    m_width = m_sprite.width();
    m_height = m_sprite.height();
}

void Button::draw() const
{
    if (!prepareDraw())
        return;
    m_sprite.draw();
    endDraw();
}

void Button::processMain()
{
    lock();
    m_sprite.processMain();
    unlock();
    Widget::processMain();
}

void Button::mouseEnter()
{
    lock();
    if (m_hoverTexture)
        m_sprite.setTexture(m_hoverTexture);
    unlock();
    Widget::mouseEnter();
}

void Button::mouseLeave()
{
    lock();
    if (m_mainTexture)
        m_sprite.setTexture(m_mainTexture);
    unlock();
    Widget::mouseLeave();
}
void Button::mouseDown(uint8_t key, int x, int y)
{
    lock();
    if (m_pressedTexture)
        m_sprite.setTexture(m_pressedTexture);
    Widget::mouseDown(key, x, y);
    unlock();
}

void Button::mouseUp(uint8_t key, int x, int y)
{
    lock();
    if (m_hoverTexture)
        m_sprite.setTexture(m_hoverTexture);
    else if (m_mainTexture)
        m_sprite.setTexture(m_mainTexture);
    Widget::mouseUp(key, x, y);
    unlock();
}

void Button::mouseClick(int x, int y)
{
    action(Action(this, Action::BUTTON_CLICKED, Action::Argument()));
}

Button::Button(const Button& other): Widget(other)
{
    m_mainTexture = other.m_mainTexture;
    m_hoverTexture = other.m_hoverTexture;
    m_pressedTexture = other.m_pressedTexture;
    m_sprite = other.m_sprite;
}

Button& Button::operator=(const Button& other)
{
    if (this == &other)
        return *this;
    Widget::operator=(other);
    m_mainTexture = other.m_mainTexture;
    m_hoverTexture = other.m_hoverTexture;
    m_pressedTexture = other.m_pressedTexture;
    m_sprite = other.m_sprite;
    return *this;
}

}

