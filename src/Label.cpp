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

namespace Rangers
{
Label::Label(Object *parent): Sprite(parent)
{
    m_vertexCount = 0;
    m_font = boost::shared_ptr<Font>();
    m_vertices = 0;
    m_wordWrap = false;
    m_fixedSize = false;
    m_scaling = TEXTURE_NO;
}

Label::Label(const std::string& text, Object *parent, boost::shared_ptr<Font> font, SpriteXOrigin xpos, SpriteYOrigin ypos): Sprite(parent)
{
    if (!font)
        m_font = Engine::instance()->coreFont();
    else
        m_font = font;

    m_xOrigin = xpos;
    m_yOrigin = ypos;
    setText(text);
    m_wordWrap = false;
    m_fixedSize = false;
    m_scaling = TEXTURE_NO;
}

Label::Label(const std::wstring& text, Object *parent, boost::shared_ptr<Font> font, SpriteXOrigin xpos, SpriteYOrigin ypos): Sprite(parent)
{
    if (!font)
        m_font = Engine::instance()->coreFont();
    else
        m_font = font;

    m_xOrigin = xpos;
    m_yOrigin = ypos;
    setText(text);
    m_wordWrap = false;
    m_fixedSize = false;
    m_scaling = TEXTURE_NO;
}

Label::Label(const Rangers::Label& other): Sprite(other)
{
    m_font = other.m_font;
    m_text = other.m_text;
    m_wordWrap = other.m_wordWrap;
    m_fixedSize = other.m_fixedSize;
}

Label& Label::operator=(const Rangers::Label& other)
{
    if (this == &other)
        return *this;

    m_font = other.m_font;
    m_text = other.m_text;
    m_wordWrap = other.m_wordWrap;
    m_fixedSize = other.m_fixedSize;

    Sprite::operator=(other);
    return *this;
}


boost::shared_ptr< Font > Label::font() const
{
    return m_font;
}


void Label::setText(const std::string& text)
{
    lock();
    m_text.assign(text.length(), '\0');
    std::copy(text.begin(), text.end(), m_text.begin());
    markToUpdate();
    unlock();
}

void Label::setText(const std::wstring& text)
{
    lock();
    m_text = text;
    markToUpdate();
    unlock();
}

void Label::processMain()
{
    if (!m_font)
        return;
    lock();

    if (!m_wordWrap)
        m_texture = m_font->renderText(m_text);
    else
        m_texture = m_font->renderText(m_text, m_width);

    if (!m_fixedSize)
    {
        m_width = m_texture->width();
        m_height = m_texture->height();
    }

    unlock();
    Sprite::processMain();
}

float Label::height() const
{
    return m_height;
}

float Label::width() const
{
    return m_width;
}

void Label::setWordWrap(bool wordWrap)
{
    m_wordWrap = wordWrap;
    markToUpdate();
}

void Label::setFixedSize(float width, float height)
{
    if ((width > 0) && (height > 0))
    {
        m_width = width;
        m_height = height;
        m_fixedSize = true;
    }
    else
        m_fixedSize = false;
    markToUpdate();
}
}
