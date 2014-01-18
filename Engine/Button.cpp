/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Button.h"
#include "OpenSR/Sprite.h"
#include "OpenSR/Action.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"
#include "OpenSR/NinePatch.h"
#include "OpenSR/Label.h"
#include "OpenSR/Font.h"
#include "OpenSR/Sound.h"
#include "OpenSR/SoundManager.h"

#include "OpenSR/private/Button_p.h"

namespace Rangers
{
void ButtonPrivate::ButtonListener::actionPerformed(const Action &action)
{
    boost::shared_ptr<Button> q = boost::static_pointer_cast<Button>(action.source());
    ButtonPrivate *d = q->d_func();
    switch (action.type())
    {
    case Action::MOUSE_ENTER:
        if (d->hoverSprite)
            d->sprite = d->hoverSprite;
        if (d->enterSound)
            d->enterSound->play();
        break;

    case Action::MOUSE_LEAVE:
        if (d->normalSprite)
            d->sprite = d->normalSprite;
        if (d->leaveSound)
            d->leaveSound->play();
        break;

    case Action::MOUSE_UP:
        if (!d->leftMouseButtonPressed)
        {
            if (d->hoverSprite)
                d->sprite = d->hoverSprite;
            else if (d->normalSprite)
                d->sprite = d->normalSprite;
        }
        break;

    case Action::MOUSE_DOWN:
        if (d->leftMouseButtonPressed)
        {
            if (d->pressedSprite)
                d->sprite = d->pressedSprite;
        }
        break;

    case Action::MOUSE_CLICK:
        if (d->clickSound)
            d->clickSound->play();
        q->action(Action(q, Action::BUTTON_CLICKED));
        break;
    }
}


boost::shared_ptr<Sprite> ButtonPrivate::spriteFromStyleObject(boost::shared_ptr<ResourceObject> o)
{
    boost::shared_ptr<TextureRegionDescriptor> texture = boost::dynamic_pointer_cast<TextureRegionDescriptor>(o);
    boost::shared_ptr<NinePatchDescriptor> ninepatch = boost::dynamic_pointer_cast<NinePatchDescriptor>(o);
    if (ninepatch)
        return boost::shared_ptr<Sprite>(new NinePatch(*ninepatch));
    else if (texture)
        return boost::shared_ptr<Sprite>(new Sprite(*texture));

    return boost::shared_ptr<Sprite>();
}

ButtonPrivate::ButtonPrivate(): WidgetPrivate()
{
    autoResize = false;
    buttonListener = boost::shared_ptr<ButtonListener>(new ButtonListener());
}

Button::Button():
    Widget(*(new ButtonPrivate()))
{
    RANGERS_D(Button);
    d->initFromStyle();
}

Button::Button(boost::shared_ptr<Texture> texture):
    Widget(*(new ButtonPrivate()))
{
    RANGERS_D(Button);
    d->initFromStyle();

    if (texture)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(texture));
        addChild(d->normalSprite);
    }

    d->sprite = d->normalSprite;

    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture):
    Widget(*(new ButtonPrivate()))
{
    RANGERS_D(Button);
    d->initFromStyle();

    if (texture)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(texture));
        addChild(d->normalSprite);
    }
    if (hoverTexture)
    {
        d->hoverSprite = boost::shared_ptr<Sprite>(new Sprite(hoverTexture));
        addChild(d->hoverSprite);
    }
    if (pressTexture)
    {
        d->pressedSprite = boost::shared_ptr<Sprite>(new Sprite(pressTexture));
        addChild(d->pressedSprite);
    }

    d->sprite = d->normalSprite;

    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(const std::wstring& texture):
    Widget(*(new ButtonPrivate()))
{
    RANGERS_D(Button);
    d->initFromStyle();

    boost::shared_ptr<Texture> main = ResourceManager::instance().loadTexture(texture);

    if (main)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(main));
        addChild(d->normalSprite);
    }

    d->sprite = d->normalSprite;

    if (main)
    {
        d->width = main->width();
        d->height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture):
    Widget(*(new ButtonPrivate()))
{
    RANGERS_D(Button);
    d->initFromStyle();

    boost::shared_ptr<Texture> main = ResourceManager::instance().loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance().loadTexture(hoverTexture);
    boost::shared_ptr<Texture> pressed = ResourceManager::instance().loadTexture(pressTexture);

    if (main)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(main));
        addChild(d->normalSprite);
    }
    if (hover)
    {
        d->hoverSprite = boost::shared_ptr<Sprite>(new Sprite(hover));
        addChild(d->hoverSprite);
    }

    if (pressed)
    {
        d->pressedSprite = boost::shared_ptr<Sprite>(new Sprite(pressed));
        addChild(d->pressedSprite);
    }

    d->sprite = d->normalSprite;

    if (main)
    {
        d->width = main->width();
        d->height = main->height();
    }
}

Button::Button(const ButtonStyle& style):
    Widget(*(new ButtonPrivate()))
{
    RANGERS_D(Button);
    d->style = style;
    d->initFromStyle();
    if (d->sprite)
    {
        d->width = d->sprite->width();
        d->height = d->sprite->height();
    }
}


Button::Button(ButtonPrivate &p): Widget(p)
{
    RANGERS_D(Button);
    d->initFromStyle();
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
    d->calcAutoResize();
    unlock();
}

void Button::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(Button);
    d->text = text;
    d->label->setText(text);
    d->calcAutoResize();
    markToUpdate();
    unlock();
}

void Button::setFont(boost::shared_ptr<Font> font)
{
    lock();
    RANGERS_D(Button);
    d->label->setFont(font);
    d->calcAutoResize();
    markToUpdate();
    unlock();
}

std::wstring Button::text() const
{
    RANGERS_D(const Button);
    return d->text;
}

void ButtonPrivate::calcAutoResize()
{
    RANGERS_Q(Button);
    if (!autoResize)
        return;
    if (!normalSprite)
        return;

    q->lock();
    int labelWidth = 0;
    int labelHeight = 0;
    if (label->font())
    {
        labelWidth = label->font()->calculateStringWidth(text.begin(), text.end());
        labelHeight = label->font()->size();
    }
    if (!style.contentRect.valid())
    {
        q->setGeometry(std::max(labelWidth, q->minWidth()), std::max(labelHeight, q->minHeight()));
    }
    else
    {
        float width = normalSprite->normalWidth() - style.contentRect.width + labelWidth;
        float height = normalSprite->normalHeight() - style.contentRect.height + labelHeight;
        q->setGeometry(std::max(width, normalSprite->normalWidth()), std::max(height, normalSprite->normalHeight()));
    }
    q->unlock();
}

int Button::minHeight() const
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        return std::max(d->label->height(), d->normalSprite->normalHeight());
    return Widget::minHeight();
}

int Button::minWidth() const
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        return std::max(d->label->width(), d->normalSprite->normalWidth());
    return Widget::minWidth();
}

int Button::preferredHeight() const
{
    RANGERS_D(const Button);
    if (!d->normalSprite)
        return Widget::preferredHeight();

    if (d->style.contentRect.valid())
        return d->normalSprite->normalHeight() + d->label->height() - d->style.contentRect.height;

    return minHeight();
}

int Button::preferredWidth() const
{
    RANGERS_D(const Button);
    if (!d->normalSprite)
        return Widget::preferredWidth();

    if (d->style.contentRect.valid())
        return d->normalSprite->normalWidth() + d->label->width() - d->style.contentRect.width;

    return minWidth();
}

boost::shared_ptr<Font> Button::font() const
{
    RANGERS_D(const Button);
    return d->label->font();
}

void ButtonPrivate::initFromStyle()
{
    RANGERS_Q(Button);

    normalSprite = spriteFromStyleObject(style.normal);
    if (normalSprite)
        q->addChild(normalSprite);

    hoverSprite = spriteFromStyleObject(style.hovered);
    if (hoverSprite)
        q->addChild(hoverSprite);

    pressedSprite = spriteFromStyleObject(style.pressed);
    if (pressedSprite)
        q->addChild(pressedSprite);

    if ((style.font) && (style.font->path != L"") && (style.font->size > 0))
    {
        label = boost::shared_ptr<Label>(new Label(text, ResourceManager::instance().loadFont(style.font->path, style.font->size)));
    }
    else
    {
        label = boost::shared_ptr<Label>(new Label());
    }
    label->setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    q->addChild(label);
    q->setColor(style.color);
    sprite = normalSprite;

    if (style.enterSound != L"")
        enterSound = SoundManager::instance().loadSound(style.enterSound);
    if (style.leaveSound != L"")
        leaveSound = SoundManager::instance().loadSound(style.leaveSound);
    if (style.clickSound != L"")
        clickSound = SoundManager::instance().loadSound(style.clickSound);

    q->addListener(buttonListener);
    q->markToUpdate();
}

Button::~Button()
{
}

void Button::draw() const
{
    RANGERS_D(const Button);
    if (!prepareDraw())
        return;

    glColor3f(1.0f, 1.0f, 1.0f);

    if (d->sprite)
        d->sprite->draw();

    d->label->setColor(d->color);
    d->label->draw();
    endDraw();
}

void Button::setSounds(boost::shared_ptr<Sound> clickSound, boost::shared_ptr<Sound> enterSound, boost::shared_ptr<Sound> leaveSound)
{
    RANGERS_D(Button);
    d->clickSound = clickSound;
    d->enterSound = enterSound;
    d->leaveSound = leaveSound;
}

void Button::setSounds(const std::wstring& clickSound, const std::wstring& enterSound, const std::wstring& leaveSound)
{
    RANGERS_D(Button);
    SoundManager &m = SoundManager::instance();
    setSounds(m.loadSound(clickSound), m.loadSound(enterSound), m.loadSound(leaveSound));
}

boost::shared_ptr<Sound> Button::clickSound() const
{
    RANGERS_D(const Button);
    return d->clickSound;
}

boost::shared_ptr<Sound> Button::enterSound() const
{
    RANGERS_D(const Button);
    return d->enterSound;
}

boost::shared_ptr<Sound> Button::leaveSound() const
{
    RANGERS_D(const Button);
    return d->leaveSound;
}

void Button::processMain()
{
    lock();
    RANGERS_D(Button);
    d->calcAutoResize();
    //if (d->label->needUpdate())
    //    d->label->processMain();
    if (d->normalSprite)
        d->normalSprite->setGeometry(d->width, d->height);
    if (d->hoverSprite)
        d->hoverSprite->setGeometry(d->width, d->height);
    if (d->pressedSprite)
        d->pressedSprite->setGeometry(d->width, d->height);
    if (!d->style.contentRect.valid() || (!d->normalSprite))
    {
        d->label->setPosition(int((d->width - d->label->width()) / 2), int((d->height - d->label->height()) / 2));
    }
    else
    {
        float x, y;
        if (d->label->width() < d->style.contentRect.width)
            x = int(d->style.contentRect.width - d->label->width()) / 2 + d->style.contentRect.x;
        else
            x = d->style.contentRect.x;
        if (d->label->height() < d->style.contentRect.height)
            y = int(d->style.contentRect.height - d->label->height()) / 2 + d->style.contentRect.y;
        else
            y = d->style.contentRect.y;
        d->label->setPosition(x, y);
    }
    unlock();
    Widget::processMain();
}

}

