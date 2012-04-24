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
#include "Label.h"
#include "Font.h"
#include "Engine.h"
#include "SoundManager.h"
#include "Sound.h"

namespace Rangers
{

Button::Button(Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
}

Button::Button(boost::shared_ptr<Texture> texture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
    if (texture)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
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
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
    if (texture)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
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
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
    if (texture)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressTexture)
    {
        m_style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressTexture));
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
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    if (main)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
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
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    if (main)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
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
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0),
    m_autoResize(false)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    boost::shared_ptr<Texture> pressed = ResourceManager::instance()->loadTexture(pressTexture);
    if (main)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressed)
    {
        m_style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressed));
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
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_style(style),
    m_autoResize(false)
{
    init();
    if (m_sprite)
    {
        m_width = m_sprite->width();
        m_height = m_sprite->height();
    }
}

bool Button::autoResize() const
{
    return m_autoResize;
}

void Button::setAutoResize(bool autoResize)
{
    lock();
    m_autoResize = autoResize;
    calcAutoRresize();
    unlock();
}

void Button::setText(const std::wstring& text)
{
    lock();
    m_text = text;
    m_label.setText(text);
    calcAutoRresize();
    markToUpdate();
    unlock();
}

void Button::setColor(int color)
{
    lock();
    m_label.setColor(color);
    unlock();
}

void Button::setFont(boost::shared_ptr<Font> font)
{
    lock();
    m_label.setFont(font);
    calcAutoRresize();
    markToUpdate();
    unlock();
}

int Button::color() const
{
    return m_label.color();
}

std::wstring Button::text() const
{
    return m_text;
}

void Button::calcAutoRresize()
{
    if (!m_autoResize)
        return;
    if (!m_normalSprite)
        return;

    lock();
    float labelWidth = 0.0f;
    float labelHeight = 0.0f;
    if (m_label.font())
    {
        labelWidth = m_label.font()->calculateStringWidth(m_text.begin(), m_text.end());
        labelHeight = m_label.font()->size();
    }
    if (!m_style.contentRect.valid())
    {
        setGeometry(std::max(labelWidth, m_normalSprite->normalWidth()), std::max(labelHeight, m_normalSprite->normalHeight()));
    }
    else
    {
        float width = m_normalSprite->normalWidth() + labelWidth - m_style.contentRect.width;
        float height = m_normalSprite->normalHeight() + labelHeight - m_style.contentRect.height;
        setGeometry(std::max(width, m_normalSprite->normalWidth()), std::max(height, m_normalSprite->normalHeight()));
    }
    unlock();
}

int Button::minHeight() const
{
    if (m_normalSprite)
        return std::max(m_label.height(), m_normalSprite->normalHeight());
    return Widget::minHeight();
}

int Button::minWidth() const
{
    if (m_normalSprite)
        return std::max(m_label.width(), m_normalSprite->normalWidth());
    return Widget::minWidth();
}

int Button::preferredHeight() const
{
    if (!m_normalSprite)
        return Widget::preferredHeight();

    if (m_style.contentRect.valid())
        return m_normalSprite->normalHeight() + m_label.height() - m_style.contentRect.height;

    return minHeight();
}

int Button::preferredWidth() const
{
    if (!m_normalSprite)
        return Widget::preferredWidth();

    if (m_style.contentRect.valid())
        return m_normalSprite->normalWidth() + m_label.width() - m_style.contentRect.width;

    return minWidth();
}

boost::shared_ptr<Font> Button::font() const
{
    return m_label.font();
}

void Button::init()
{
    if (m_style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        m_normalSprite = new NinePatch(boost::get<NinePatchDescriptor>(m_style.normal.resource), this);
    }
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
    {
        m_normalSprite = new Sprite(boost::get<TextureRegion>(m_style.normal.resource), this);
    }
    if (m_style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        m_hoverSprite = new NinePatch(boost::get<NinePatchDescriptor>(m_style.hovered.resource), this);
    }
    else if (m_style.hovered.type == ResourceDescriptor::SPRITE)
    {
        m_hoverSprite = new Sprite(boost::get<TextureRegion>(m_style.hovered.resource), this);
    }
    if (m_style.pressed.type == ResourceDescriptor::NINEPATCH)
    {
        m_pressedSprite = new NinePatch(boost::get<NinePatchDescriptor>(m_style.pressed.resource), this);
    }
    else if (m_style.pressed.type == ResourceDescriptor::SPRITE)
    {
        m_pressedSprite = new Sprite(boost::get<TextureRegion>(m_style.pressed.resource), this);
    }
    if ((m_style.font.path != L"") && (m_style.font.size > 0))
    {
        m_label = Label(m_text, this, ResourceManager::instance()->loadFont(m_style.font.path, m_style.font.size));
    }
    m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    setColor(m_style.color);
    m_sprite = m_normalSprite;
    m_hoverSound = SoundManager::instance()->loadSound(L"Sound/ButtonEnter.wav");
    markToUpdate();
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
    m_label.draw();
    endDraw();
}

void Button::processMain()
{
    lock();
    if (m_label.needUpdate())
        m_label.processMain();
    if (m_normalSprite)
        m_normalSprite->setGeometry(m_width, m_height);
    if (m_hoverSprite)
        m_hoverSprite->setGeometry(m_width, m_height);
    if (m_pressedSprite)
        m_pressedSprite->setGeometry(m_width, m_height);
    if (!m_style.contentRect.valid() || (!m_normalSprite))
    {
        m_label.setPosition(int((m_width - m_label.width()) / 2), int((m_height - m_label.height()) / 2));
    }
    else
    {
        float x, y;
        if (m_label.width() < m_style.contentRect.width)
            x = int(m_style.contentRect.width - m_label.width()) / 2 + m_style.contentRect.x;
        else
            x = m_style.contentRect.x;
        if (m_label.height() < m_style.contentRect.height)
            y = int(m_style.contentRect.height - m_label.height()) / 2 + m_style.contentRect.y;
        else
            y = m_style.contentRect.y;
        m_label.setPosition(x, y);
    }
    unlock();
    Widget::processMain();
}

void Button::mouseEnter()
{
    lock();
    if (m_hoverSprite)
        m_sprite = m_hoverSprite;
    m_hoverSound->play();
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

void Button::mouseDown(uint8_t key, const Vector &p)
{
    lock();
    Widget::mouseDown(key, p);
    if (m_leftMouseButtonPressed)
    {
        if (m_pressedSprite)
            m_sprite = m_pressedSprite;
    }
    unlock();
}

void Button::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    if (m_leftMouseButtonPressed)
    {
        if (m_hoverSprite)
            m_sprite = m_hoverSprite;
        else if (m_normalSprite)
            m_sprite = m_normalSprite;
    }
    Widget::mouseUp(key, p);
    unlock();
}

void Button::mouseClick(const Vector &p)
{
    action(Action(this, Action::BUTTON_CLICKED, Action::Argument()));
}


//FIXME: Copy constructor and operator=
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

