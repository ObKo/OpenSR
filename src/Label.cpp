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

#include "Label.h"
#include <string>
#include "Engine.h"
#include "Object.h"
#include "Font.h"
#include "Texture.h"

#include "private/Label_p.h"

namespace Rangers
{
Label::Label(Object *parent): Sprite(*(new LabelPrivate()), parent)
{
    RANGERS_D(Label);
    d->m_vertexCount = 0;
    d->m_font = boost::shared_ptr<Font>();
    d->m_vertices = 0;
    d->m_wordWrap = false;
    d->m_fixedSize = false;
    d->m_scaling = TEXTURE_NO;
}

Label::Label(const std::string& text, Object *parent, boost::shared_ptr<Font> font, SpriteXOrigin xpos, SpriteYOrigin ypos): Sprite(*(new LabelPrivate()), parent)
{
    RANGERS_D(Label);
    if (!font)
        d->m_font = Engine::instance()->coreFont();
    else
        d->m_font = font;

    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    setText(text);
    d->m_wordWrap = false;
    d->m_fixedSize = false;
    d->m_scaling = TEXTURE_NO;
}

Label::Label(const std::wstring& text, Object *parent, boost::shared_ptr<Font> font, SpriteXOrigin xpos, SpriteYOrigin ypos): Sprite(*(new LabelPrivate()), parent)
{
    RANGERS_D(Label);
    if (!font)
        d->m_font = Engine::instance()->coreFont();
    else
        d->m_font = font;

    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    setText(text);
    d->m_wordWrap = false;
    d->m_fixedSize = false;
    d->m_scaling = TEXTURE_NO;
}

Label::Label(const Rangers::Label& other): Sprite(*(new LabelPrivate()), other)
{
    RANGERS_D(Label);
    d->m_font = other.d_func()->m_font;
    d->m_text = other.d_func()->m_text;
    d->m_wordWrap = other.d_func()->m_wordWrap;
    d->m_fixedSize = other.d_func()->m_fixedSize;
}

Label& Label::operator=(const Rangers::Label& other)
{
    RANGERS_D(Label);
    if (this == &other)
        return *this;

    d->m_font = other.d_func()->m_font;
    d->m_text = other.d_func()->m_text;
    d->m_wordWrap = other.d_func()->m_wordWrap;
    d->m_fixedSize = other.d_func()->m_fixedSize;

    Sprite::operator=(other);
    return *this;
}


Label::Label(LabelPrivate &p, Object *parent): Sprite(p, parent) 
{
    RANGERS_D(Label);
    d->m_vertexCount = 0;
    d->m_font = boost::shared_ptr<Font>();
    d->m_vertices = 0;
    d->m_wordWrap = false;
    d->m_fixedSize = false;
    d->m_scaling = TEXTURE_NO;
}

Label::Label(LabelPrivate &p, const Label& other): Sprite(p, other)
{
    RANGERS_D(Label);
    d->m_font = other.d_func()->m_font;
    d->m_text = other.d_func()->m_text;
    d->m_wordWrap = other.d_func()->m_wordWrap;
    d->m_fixedSize = other.d_func()->m_fixedSize;
}


boost::shared_ptr< Font > Label::font() const
{
    RANGERS_D(const Label);
    return d->m_font;
}


void Label::setText(const std::string& text)
{
    lock();
    RANGERS_D(Label);
    d->m_text.assign(text.length(), '\0');
    std::copy(text.begin(), text.end(), d->m_text.begin());
    markToUpdate();
    unlock();
}

void Label::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(Label);
    d->m_text = text;
    markToUpdate();
    unlock();
}

void Label::setFont(boost::shared_ptr<Font> font)
{
    lock();
    RANGERS_D(Label);
    d->m_font = font;
    markToUpdate();
    unlock();
}

void Label::processMain()
{
    RANGERS_D(Label);
    if (!d->m_font)
        return;
    lock();

    if (!d->m_wordWrap)
        d->m_region = TextureRegion(d->m_font->renderText(d->m_text));
    else
        d->m_region = TextureRegion(d->m_font->renderText(d->m_text, d->m_width));

    if (!d->m_fixedSize)
    {
        d->m_width = d->m_region.texture->width();
        d->m_height = d->m_region.texture->height();
    }

    unlock();
    Sprite::processMain();
}

std::wstring Label::text() const
{
    RANGERS_D(const Label);
    return d->m_text;
}

void Label::setWordWrap(bool wordWrap)
{
    lock();
    RANGERS_D(Label);
    d->m_wordWrap = wordWrap;
    markToUpdate();
    unlock();
}

void Label::setFixedSize(float width, float height)
{
    lock();
    RANGERS_D(Label);
    if ((width > 0) && (height > 0))
    {
        d->m_width = width;
        d->m_height = height;
        d->m_fixedSize = true;
    }
    else
        d->m_fixedSize = false;
    markToUpdate();
    unlock();
}
}
