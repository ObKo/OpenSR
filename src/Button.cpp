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
    hoverSprite = 0;
    autoResize = false;
    pressedSprite = 0;
    sprite = 0;
    normalSprite = 0;
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
        d->style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        d->style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    if (texture)
    {
        d->style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        d->style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        d->style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        d->style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    if (texture)
    {
        d->style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        d->style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        d->style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        d->style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressTexture)
    {
        d->style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressTexture));
        d->style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(const std::wstring& texture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    if (main)
    {
        d->style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        d->style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        d->width = main->width();
        d->height = main->height();
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
        d->style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        d->style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        d->style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        d->style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        d->width = main->width();
        d->height = main->height();
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
        d->style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        d->style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        d->style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        d->style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressed)
    {
        d->style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressed));
        d->style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        d->width = main->width();
        d->height = main->height();
    }
}

Button::Button(const ButtonStyle& style, Widget* parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);

    init();
    if (d->sprite)
    {
        d->width = d->sprite->width();
        d->height = d->sprite->height();
    }
}


Button::Button(ButtonPrivate &p, Widget *parent): Widget(p, parent)
{
}

Button::Button(ButtonPrivate &p, const Button& other): Widget(p, other)
{
    Button(other.d_func()->style);
}

bool Button::autoResize() const
{
    RANGERS_D(const Button);
    return d->autoResize;
}

void Button::setAutoResize(bool autoResize)
{
    lock();
    RANGERS_D(Button);
    d->autoResize = autoResize;
    calcAutoRresize();
    unlock();
}

void Button::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(Button);
    d->text = text;
    d->label.setText(text);
    calcAutoRresize();
    markToUpdate();
    unlock();
}

void Button::setColor(int color)
{
    lock();
    RANGERS_D(Button);
    d->label.setColor(color);
    unlock();
}

void Button::setFont(boost::shared_ptr<Font> font)
{
    lock();
    RANGERS_D(Button);
    d->label.setFont(font);
    calcAutoRresize();
    markToUpdate();
    unlock();
}

int Button::color() const
{
    RANGERS_D(const Button);
    return d->label.color();
}

std::wstring Button::text() const
{
    RANGERS_D(const Button);
    return d->text;
}

void Button::calcAutoRresize()
{
    RANGERS_D(Button);
    if (!d->autoResize)
        return;
    if (!d->normalSprite)
        return;

    lock();
    float labelWidth = 0.0f;
    float labelHeight = 0.0f;
    if (d->label.font())
    {
        labelWidth = d->label.font()->calculateStringWidth(d->text.begin(), d->text.end());
        labelHeight = d->label.font()->size();
    }
    if (!d->style.contentRect.valid())
    {
        setGeometry(std::max(labelWidth, d->normalSprite->normalWidth()), std::max(labelHeight, d->normalSprite->normalHeight()));
    }
    else
    {
        float width = d->normalSprite->normalWidth() + labelWidth - d->style.contentRect.width;
        float height = d->normalSprite->normalHeight() + labelHeight - d->style.contentRect.height;
        setGeometry(std::max(width, d->normalSprite->normalWidth()), std::max(height, d->normalSprite->normalHeight()));
    }
    unlock();
}

int Button::minHeight() const
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        return std::max(d->label.height(), d->normalSprite->normalHeight());
    return Widget::minHeight();
}

int Button::minWidth() const
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        return std::max(d->label.width(), d->normalSprite->normalWidth());
    return Widget::minWidth();
}

int Button::preferredHeight() const
{
    RANGERS_D(const Button);
    if (!d->normalSprite)
        return Widget::preferredHeight();

    if (d->style.contentRect.valid())
        return d->normalSprite->normalHeight() + d->label.height() - d->style.contentRect.height;

    return minHeight();
}

int Button::preferredWidth() const
{
    RANGERS_D(const Button);
    if (!d->normalSprite)
        return Widget::preferredWidth();

    if (d->style.contentRect.valid())
        return d->normalSprite->normalWidth() + d->label.width() - d->style.contentRect.width;

    return minWidth();
}

boost::shared_ptr<Font> Button::font() const
{
    RANGERS_D(const Button);
    return d->label.font();
}

void Button::init()
{
    RANGERS_D(Button);
    if (d->style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        d->normalSprite = new NinePatch(boost::get<NinePatchDescriptor>(d->style.normal.resource), this);
    }
    else if (d->style.normal.type == ResourceDescriptor::SPRITE)
    {
        d->normalSprite = new Sprite(boost::get<TextureRegion>(d->style.normal.resource), this);
    }
    if (d->style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        d->hoverSprite = new NinePatch(boost::get<NinePatchDescriptor>(d->style.hovered.resource), this);
    }
    else if (d->style.hovered.type == ResourceDescriptor::SPRITE)
    {
        d->hoverSprite = new Sprite(boost::get<TextureRegion>(d->style.hovered.resource), this);
    }
    if (d->style.pressed.type == ResourceDescriptor::NINEPATCH)
    {
        d->pressedSprite = new NinePatch(boost::get<NinePatchDescriptor>(d->style.pressed.resource), this);
    }
    else if (d->style.pressed.type == ResourceDescriptor::SPRITE)
    {
        d->pressedSprite = new Sprite(boost::get<TextureRegion>(d->style.pressed.resource), this);
    }
    if ((d->style.font.path != L"") && (d->style.font.size > 0))
    {
        d->label = Label(d->text, this, ResourceManager::instance()->loadFont(d->style.font.path, d->style.font.size));
    }
    d->label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    setColor(d->style.color);
    d->sprite = d->normalSprite;

    if (d->style.enterSound != L"")
        d->enterSound = SoundManager::instance()->loadSound(d->style.enterSound);
    if (d->style.leaveSound != L"")
        d->leaveSound = SoundManager::instance()->loadSound(d->style.leaveSound);
    if (d->style.clickSound != L"")
        d->clickSound = SoundManager::instance()->loadSound(d->style.clickSound);

    markToUpdate();
}

Button::~Button()
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        delete d->normalSprite;
    if (d->hoverSprite)
        delete d->hoverSprite;
    if (d->pressedSprite)
        delete d->pressedSprite;
}

void Button::draw() const
{
    RANGERS_D(const Button);
    if (!prepareDraw())
        return;
    if (d->sprite)
        d->sprite->draw();
    d->label.draw();
    endDraw();
}

void Button::setSounds(boost::shared_ptr<Sound> clickSound, boost::shared_ptr<Sound> leaveSound, boost::shared_ptr<Sound> enterSound)
{
    lock();
    RANGERS_D(Button);
    d->clickSound = clickSound;
    d->enterSound = enterSound;
    d->leaveSound = leaveSound;
    unlock();
}

void Button::setSounds(const std::wstring& clickSound, const std::wstring& leaveSound, const std::wstring& enterSound)
{
    lock();
    RANGERS_D(Button);
    d->clickSound = SoundManager::instance()->loadSound(clickSound);
    d->enterSound = SoundManager::instance()->loadSound(enterSound);
    d->leaveSound = SoundManager::instance()->loadSound(leaveSound);
    unlock();
}

void Button::processMain()
{
    lock();
    RANGERS_D(Button);
    if (d->label.needUpdate())
        d->label.processMain();
    if (d->normalSprite)
        d->normalSprite->setGeometry(d->width, d->height);
    if (d->hoverSprite)
        d->hoverSprite->setGeometry(d->width, d->height);
    if (d->pressedSprite)
        d->pressedSprite->setGeometry(d->width, d->height);
    if (!d->style.contentRect.valid() || (!d->normalSprite))
    {
        d->label.setPosition(int((d->width - d->label.width()) / 2), int((d->height - d->label.height()) / 2));
    }
    else
    {
        float x, y;
        if (d->label.width() < d->style.contentRect.width)
            x = int(d->style.contentRect.width - d->label.width()) / 2 + d->style.contentRect.x;
        else
            x = d->style.contentRect.x;
        if (d->label.height() < d->style.contentRect.height)
            y = int(d->style.contentRect.height - d->label.height()) / 2 + d->style.contentRect.y;
        else
            y = d->style.contentRect.y;
        d->label.setPosition(x, y);
    }
    unlock();
    Widget::processMain();
}

void Button::mouseEnter()
{
    lock();
    RANGERS_D(Button);
    if (d->hoverSprite)
        d->sprite = d->hoverSprite;
    if (d->enterSound)
        d->enterSound->play();
    unlock();
    Widget::mouseEnter();
}

void Button::mouseLeave()
{
    lock();
    RANGERS_D(Button);
    if (d->normalSprite)
        d->sprite = d->normalSprite;
    if (d->leaveSound)
        d->leaveSound->play();
    unlock();
    Widget::mouseLeave();
}

void Button::mouseDown(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(Button);
    Widget::mouseDown(key, p);
    if (d->leftMouseButtonPressed)
    {
        if (d->pressedSprite)
            d->sprite = d->pressedSprite;
    }
    unlock();
}

void Button::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    RANGERS_D(Button);
    if (d->leftMouseButtonPressed)
    {
        if (d->hoverSprite)
            d->sprite = d->hoverSprite;
        else if (d->normalSprite)
            d->sprite = d->normalSprite;
    }
    Widget::mouseUp(key, p);
    unlock();
}

void Button::mouseClick(const Vector &p)
{
    RANGERS_D(Button);
    if (d->clickSound)
        d->clickSound->play();
    action(Action(this, Action::BUTTON_CLICKED, Action::Argument()));
}


//FIXME: Copy constructor and operator=
Button::Button(const Button& other): Widget(*(new ButtonPrivate()), other)
{
    Button(other.d_func()->style);
}

Button& Button::operator=(const Button& other)
{
    if (this == &other)
        return *this;
    RANGERS_D(Button);
    Widget::operator=(other);
    if (d->normalSprite)
        delete d->normalSprite;
    if (d->hoverSprite);
    delete d->hoverSprite;
    if (d->pressedSprite)
        delete d->pressedSprite;
    d->style = other.d_func()->style;
    d->normalSprite = 0;
    d->hoverSprite = 0;
    d->pressedSprite = 0;
    init();
    return *this;
}

}

