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
#include "NinePatch.h"

namespace Rangers
{

Button::Button(Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
}

Button::Button(boost::shared_ptr<Texture> texture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
    if (texture)
    {
        m_style.normal.region = TextureRegion(texture);
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        m_width = texture->width();
        m_height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
    if (texture)
    {
        m_style.normal.region = TextureRegion(texture);
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        m_style.hovered.region = TextureRegion(hoverTexture);
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        m_width = texture->width();
        m_height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
    if (texture)
    {
        m_style.normal.region = TextureRegion(texture);
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        m_style.hovered.region = TextureRegion(hoverTexture);
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressTexture)
    {
        m_style.pressed.region = TextureRegion(pressTexture);
        m_style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        m_width = texture->width();
        m_height = texture->height();
    }
}

Button::Button(const std::wstring& texture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    if (main)
    {
        m_style.normal.region = TextureRegion(main);
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        m_width = main->width();
        m_height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    if (main)
    {
        m_style.normal.region = TextureRegion(main);
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        m_style.hovered.region = TextureRegion(hover);
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        m_width = main->width();
        m_height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    boost::shared_ptr<Texture> pressed = ResourceManager::instance()->loadTexture(pressTexture);
    if (main)
    {
        m_style.normal.region = TextureRegion(main);
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        m_style.hovered.region = TextureRegion(hover);
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressed)
    {
        m_style.pressed.region = TextureRegion(pressed);
        m_style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        m_width = main->width();
        m_height = main->height();
    }
}

Button::Button(const ButtonStyle& style, Widget* parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_style(style)
{
    init();
    if (m_sprite)
    {
        m_width = m_sprite->width();
        m_height = m_sprite->height();
    }
}

void Button::init()
{
    if (m_style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        m_normalSprite = new NinePatch(m_style.normal.ninepatch, this);
    }
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
    {
        m_normalSprite = new Sprite(m_style.normal.region.texture, this);
    }
    if (m_style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        m_hoverSprite = new NinePatch(m_style.hovered.ninepatch, this);
    }
    else if (m_style.hovered.type == ResourceDescriptor::SPRITE)
    {
        m_hoverSprite = new Sprite(m_style.hovered.region.texture, this);
    }
    if (m_style.pressed.type == ResourceDescriptor::NINEPATCH)
    {
        m_pressedSprite = new NinePatch(m_style.pressed.ninepatch, this);
    }
    else if (m_style.pressed.type == ResourceDescriptor::SPRITE)
    {
        m_pressedSprite = new Sprite(m_style.pressed.region.texture, this);
    }
    m_sprite = m_normalSprite;
}

Button::~Button()
{
    if (m_normalSprite)
        delete m_normalSprite;
    if (m_hoverSprite);
    delete m_hoverSprite;
    if (m_pressedSprite)
        delete m_pressedSprite;
}

void Button::draw() const
{
    if (!prepareDraw())
        return;
    if (m_sprite)
        m_sprite->draw();
    endDraw();
}

void Button::processMain()
{
    lock();
    if (m_normalSprite)
        m_normalSprite->setGeometry(m_width, m_height);
    if (m_hoverSprite)
        m_hoverSprite->setGeometry(m_width, m_height);
    if (m_pressedSprite)
        m_pressedSprite->setGeometry(m_width, m_height);
    unlock();
    Widget::processMain();
}

void Button::mouseEnter()
{
    lock();
    if (m_hoverSprite)
        m_sprite = m_hoverSprite;
    unlock();
    Widget::mouseEnter();
}

void Button::mouseLeave()
{
    lock();
    if (m_normalSprite)
        m_sprite = m_normalSprite;
    unlock();
    Widget::mouseLeave();
}
void Button::mouseDown(uint8_t key, int x, int y)
{
    lock();
    if (m_pressedSprite)
        m_sprite = m_pressedSprite;
    Widget::mouseDown(key, x, y);
    unlock();
}

void Button::mouseUp(uint8_t key, int x, int y)
{
    lock();
    if (m_hoverSprite)
        m_sprite = m_hoverSprite;
    else if (m_normalSprite)
        m_sprite = m_normalSprite;
    Widget::mouseUp(key, x, y);
    unlock();
}

void Button::mouseClick(int x, int y)
{
    action(Action(this, Action::BUTTON_CLICKED, Action::Argument()));
}

Button::Button(const Button& other)
{
    if (m_normalSprite)
        delete m_normalSprite;
    if (m_hoverSprite);
    delete m_hoverSprite;
    if (m_pressedSprite)
        delete m_pressedSprite;
    Button(other.m_style);
}

Button& Button::operator=(const Button& other)
{
    if (this == &other)
        return *this;
    Widget::operator=(other);
    if (m_normalSprite)
        delete m_normalSprite;
    if (m_hoverSprite);
    delete m_hoverSprite;
    if (m_pressedSprite)
        delete m_pressedSprite;
    m_style = other.m_style;
    m_normalSprite = 0;
    m_hoverSprite = 0;
    m_pressedSprite = 0;
    init();
    return *this;
}

}

