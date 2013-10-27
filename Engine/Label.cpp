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

#include "OpenSR/Label.h"
#include "OpenSR/Engine.h"
#include "OpenSR/Font.h"
#include "OpenSR/Texture.h"

#include "OpenSR/private/Label_p.h"

namespace Rangers
{
LabelPrivate::LabelPrivate(): SpritePrivate()
{
    font = boost::shared_ptr<Font>();
    wordWrap = false;
    fixedSize = false;
    scaling = TEXTURE_NO;
}

Label::Label(): Sprite(*(new LabelPrivate()))
{
}

Label::Label(const std::string& text, boost::shared_ptr<Font> font): Sprite(*(new LabelPrivate()))
{
    RANGERS_D(Label);
    if (!font)
        d->font = Engine::instance().coreFont();
    else
        d->font = font;
    setText(text);
}

Label::Label(const std::wstring& text, boost::shared_ptr<Font> font): Sprite(*(new LabelPrivate()))
{
    RANGERS_D(Label);
    if (!font)
        d->font = Engine::instance().coreFont();
    else
        d->font = font;

    setText(text);
}

Label::Label(LabelPrivate &p): Sprite(p)
{
}

boost::shared_ptr< Font > Label::font() const
{
    RANGERS_D(const Label);
    return d->font;
}


void Label::setText(const std::string& text)
{
    lock();
    RANGERS_D(Label);
    d->text.assign(text.length(), '\0');
    std::copy(text.begin(), text.end(), d->text.begin());
    markToUpdate();
    unlock();
}

void Label::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(Label);
    d->text = text;
    markToUpdate();
    unlock();
}

void Label::setFont(boost::shared_ptr<Font> font)
{
    lock();
    RANGERS_D(Label);
    d->font = font;
    markToUpdate();
    unlock();
}

void Label::processMain()
{
    RANGERS_D(Label);
    if (!d->font)
        return;
    lock();

    if (!d->wordWrap)
        d->region = TextureRegion(d->font->renderText(d->text));
    else
        d->region = TextureRegion(d->font->renderText(d->text, d->width));

    if (!d->fixedSize)
    {
        d->width = d->region.texture->width();
        d->height = d->region.texture->height();
    }

    unlock();
    Sprite::processMain();
}

std::wstring Label::text() const
{
    RANGERS_D(const Label);
    return d->text;
}

bool Label::wordWrap() const
{
    RANGERS_D(const Label);
    return d->wordWrap;
}

void Label::setWordWrap(bool wordWrap)
{
    lock();
    RANGERS_D(Label);
    d->wordWrap = wordWrap;
    markToUpdate();
    unlock();
}

void Label::setFixedSize(float width, float height)
{
    lock();
    RANGERS_D(Label);
    if ((width > 0) && (height > 0))
    {
        d->width = width;
        d->height = height;
        d->fixedSize = true;
    }
    else
        d->fixedSize = false;
    markToUpdate();
    unlock();
}
}
