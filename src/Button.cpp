/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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
#include "Action.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "NinePatch.h"
#include "Label.h"
#include "Font.h"
#include "Sound.h"
#include "SoundManager.h"

#include "private/Button_p.h"

namespace Rangers
{
ButtonPrivate::ButtonPrivate(): WidgetPrivate()
{
    m_hoverSprite = 0;
    m_autoResize = false;
    m_pressedSprite = 0;
    m_sprite = 0;
    m_normalSprite = 0;
}

Button::Button(Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
}

Button::Button(boost::shared_ptr<Texture> texture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    if (texture)
    {
        d->m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        d->m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        d->m_width = texture->width();
        d->m_height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    if (texture)
    {
        d->m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        d->m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        d->m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        d->m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        d->m_width = texture->width();
        d->m_height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    if (texture)
    {
        d->m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        d->m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        d->m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        d->m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressTexture)
    {
        d->m_style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressTexture));
        d->m_style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        d->m_width = texture->width();
        d->m_height = texture->height();
    }
}

Button::Button(const std::wstring& texture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    if (main)
    {
        d->m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        d->m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        d->m_width = main->width();
        d->m_height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    if (main)
    {
        d->m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        d->m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        d->m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        d->m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        d->m_width = main->width();
        d->m_height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    boost::shared_ptr<Texture> pressed = ResourceManager::instance()->loadTexture(pressTexture);
    if (main)
    {
        d->m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        d->m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        d->m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        d->m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressed)
    {
        d->m_style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressed));
        d->m_style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        d->m_width = main->width();
        d->m_height = main->height();
    }
}

Button::Button(const ButtonStyle& style, Widget* parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    init();
    if (d->m_sprite)
    {
        d->m_width = d->m_sprite->width();
        d->m_height = d->m_sprite->height();
    }
}


Button::Button(ButtonPrivate &p, Widget *parent): Widget(p, parent)
{
}

Button::Button(ButtonPrivate &p, const Button& other): Widget(p, other)
{
    Button(other.d_func()->m_style);
}

bool Button::autoResize() const
{
    RANGERS_D(const Button);
    return d->m_autoResize;
}

void Button::setAutoResize(bool autoResize)
{
    lock();
    RANGERS_D(Button);
    d->m_autoResize = autoResize;
    calcAutoRresize();
    unlock();
}

void Button::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(Button);
    d->m_text = text;
    d->m_label.setText(text);
    calcAutoRresize();
    markToUpdate();
    unlock();
}

void Button::setColor(int color)
{
    lock();
    RANGERS_D(Button);
    d->m_label.setColor(color);
    unlock();
}

void Button::setFont(boost::shared_ptr<Font> font)
{
    lock();
    RANGERS_D(Button);
    d->m_label.setFont(font);
    calcAutoRresize();
    markToUpdate();
    unlock();
}

int Button::color() const
{
    RANGERS_D(const Button);
    return d->m_label.color();
}

std::wstring Button::text() const
{
    RANGERS_D(const Button);
    return d->m_text;
}

void Button::calcAutoRresize()
{
    RANGERS_D(Button);
    if (!d->m_autoResize)
        return;
    if (!d->m_normalSprite)
        return;

    lock();
    float labelWidth = 0.0f;
    float labelHeight = 0.0f;
    if (d->m_label.font())
    {
        labelWidth = d->m_label.font()->calculateStringWidth(d->m_text.begin(), d->m_text.end());
        labelHeight = d->m_label.font()->size();
    }
    if (!d->m_style.contentRect.valid())
    {
        setGeometry(std::max(labelWidth, d->m_normalSprite->normalWidth()), std::max(labelHeight, d->m_normalSprite->normalHeight()));
    }
    else
    {
        float width = d->m_normalSprite->normalWidth() + labelWidth - d->m_style.contentRect.width;
        float height = d->m_normalSprite->normalHeight() + labelHeight - d->m_style.contentRect.height;
        setGeometry(std::max(width, d->m_normalSprite->normalWidth()), std::max(height, d->m_normalSprite->normalHeight()));
    }
    unlock();
}

int Button::minHeight() const
{
    RANGERS_D(const Button);
    if (d->m_normalSprite)
        return std::max(d->m_label.height(), d->m_normalSprite->normalHeight());
    return Widget::minHeight();
}

int Button::minWidth() const
{
    RANGERS_D(const Button);
    if (d->m_normalSprite)
        return std::max(d->m_label.width(), d->m_normalSprite->normalWidth());
    return Widget::minWidth();
}

int Button::preferredHeight() const
{
    RANGERS_D(const Button);
    if (!d->m_normalSprite)
        return Widget::preferredHeight();

    if (d->m_style.contentRect.valid())
        return d->m_normalSprite->normalHeight() + d->m_label.height() - d->m_style.contentRect.height;

    return minHeight();
}

int Button::preferredWidth() const
{
    RANGERS_D(const Button);
    if (!d->m_normalSprite)
        return Widget::preferredWidth();

    if (d->m_style.contentRect.valid())
        return d->m_normalSprite->normalWidth() + d->m_label.width() - d->m_style.contentRect.width;

    return minWidth();
}

boost::shared_ptr<Font> Button::font() const
{
    RANGERS_D(const Button);
    return d->m_label.font();
}

void Button::init()
{
    RANGERS_D(Button);
    if (d->m_style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        d->m_normalSprite = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.normal.resource), this);
    }
    else if (d->m_style.normal.type == ResourceDescriptor::SPRITE)
    {
        d->m_normalSprite = new Sprite(boost::get<TextureRegion>(d->m_style.normal.resource), this);
    }
    if (d->m_style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        d->m_hoverSprite = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.hovered.resource), this);
    }
    else if (d->m_style.hovered.type == ResourceDescriptor::SPRITE)
    {
        d->m_hoverSprite = new Sprite(boost::get<TextureRegion>(d->m_style.hovered.resource), this);
    }
    if (d->m_style.pressed.type == ResourceDescriptor::NINEPATCH)
    {
        d->m_pressedSprite = new NinePatch(boost::get<NinePatchDescriptor>(d->m_style.pressed.resource), this);
    }
    else if (d->m_style.pressed.type == ResourceDescriptor::SPRITE)
    {
        d->m_pressedSprite = new Sprite(boost::get<TextureRegion>(d->m_style.pressed.resource), this);
    }
    if ((d->m_style.font.path != L"") && (d->m_style.font.size > 0))
    {
        d->m_label = Label(d->m_text, this, ResourceManager::instance()->loadFont(d->m_style.font.path, d->m_style.font.size));
    }
    d->m_label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    setColor(d->m_style.color);
    d->m_sprite = d->m_normalSprite;

    if (d->m_style.enterSound != L"")
        d->m_enterSound = SoundManager::instance()->loadSound(d->m_style.enterSound);
    if (d->m_style.leaveSound != L"")
        d->m_leaveSound = SoundManager::instance()->loadSound(d->m_style.leaveSound);
    if (d->m_style.clickSound != L"")
        d->m_clickSound = SoundManager::instance()->loadSound(d->m_style.clickSound);

    markToUpdate();
}

Button::~Button()
{
    RANGERS_D(const Button);
    if (d->m_normalSprite)
        delete d->m_normalSprite;
    if (d->m_hoverSprite)
        delete d->m_hoverSprite;
    if (d->m_pressedSprite)
        delete d->m_pressedSprite;
}

void Button::draw() const
{
    RANGERS_D(const Button);
    if (!prepareDraw())
        return;
    if (d->m_sprite)
        d->m_sprite->draw();
    d->m_label.draw();
    endDraw();
}

void Button::setSounds(boost::shared_ptr<Sound> clickSound, boost::shared_ptr<Sound> leaveSound, boost::shared_ptr<Sound> enterSound)
{
    lock();
    RANGERS_D(Button);
    d->m_clickSound = clickSound;
    d->m_enterSound = enterSound;
    d->m_leaveSound = leaveSound;
    unlock();
}

void Button::setSounds(const std::wstring& clickSound, const std::wstring& leaveSound, const std::wstring& enterSound)
{
    lock();
    RANGERS_D(Button);
    d->m_clickSound = SoundManager::instance()->loadSound(clickSound);
    d->m_enterSound = SoundManager::instance()->loadSound(enterSound);
    d->m_leaveSound = SoundManager::instance()->loadSound(leaveSound);
    unlock();
}

void Button::processMain()
{
    lock();
    RANGERS_D(Button);
    if (d->m_label.needUpdate())
        d->m_label.processMain();
    if (d->m_normalSprite)
        d->m_normalSprite->setGeometry(d->m_width, d->m_height);
    if (d->m_hoverSprite)
        d->m_hoverSprite->setGeometry(d->m_width, d->m_height);
    if (d->m_pressedSprite)
        d->m_pressedSprite->setGeometry(d->m_width, d->m_height);
    if (!d->m_style.contentRect.valid() || (!d->m_normalSprite))
    {
        d->m_label.setPosition(int((d->m_width - d->m_label.width()) / 2), int((d->m_height - d->m_label.height()) / 2));
    }
    else
    {
        float x, y;
        if (d->m_label.width() < d->m_style.contentRect.width)
            x = int(d->m_style.contentRect.width - d->m_label.width()) / 2 + d->m_style.contentRect.x;
        else
            x = d->m_style.contentRect.x;
        if (d->m_label.height() < d->m_style.contentRect.height)
            y = int(d->m_style.contentRect.height - d->m_label.height()) / 2 + d->m_style.contentRect.y;
        else
            y = d->m_style.contentRect.y;
        d->m_label.setPosition(x, y);
    }
    unlock();
    Widget::processMain();
}

void Button::mouseEnter()
{
    lock();
    RANGERS_D(Button);
    if (d->m_hoverSprite)
        d->m_sprite = d->m_hoverSprite;
    if (d->m_enterSound)
        d->m_enterSound->play();
    unlock();
    Widget::mouseEnter();
}

void Button::mouseLeave()
{
    lock();
    RANGERS_D(Button);
    if (d->m_normalSprite)
        d->m_sprite = d->m_normalSprite;
    if (d->m_leaveSound)
        d->m_leaveSound->play();
    unlock();
    Widget::mouseLeave();
}

void Button::mouseDown(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(Button);
    Widget::mouseDown(key, p);
    if (d->m_leftMouseButtonPressed)
    {
        if (d->m_pressedSprite)
            d->m_sprite = d->m_pressedSprite;
    }
    unlock();
}

void Button::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(Button);
    if (d->m_leftMouseButtonPressed)
    {
        if (d->m_hoverSprite)
            d->m_sprite = d->m_hoverSprite;
        else if (d->m_normalSprite)
            d->m_sprite = d->m_normalSprite;
    }
    Widget::mouseUp(key, p);
    unlock();
}

void Button::mouseClick(const Vector &p)
{
    RANGERS_D(Button);
    if (d->m_clickSound)
        d->m_clickSound->play();
    action(Action(this, Action::BUTTON_CLICKED, Action::Argument()));
}


//FIXME: Copy constructor and operator=
Button::Button(const Button& other): Widget(*(new ButtonPrivate()), other)
{
    Button(other.d_func()->m_style);
}

Button& Button::operator=(const Button& other)
{
    if (this == &other)
        return *this;
    RANGERS_D(Button);
    Widget::operator=(other);
    if (d->m_normalSprite)
        delete d->m_normalSprite;
    if (d->m_hoverSprite);
    delete d->m_hoverSprite;
    if (d->m_pressedSprite)
        delete d->m_pressedSprite;
    d->m_style = other.d_func()->m_style;
    d->m_normalSprite = 0;
    d->m_hoverSprite = 0;
    d->m_pressedSprite = 0;
    init();
    return *this;
}

}

