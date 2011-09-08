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

using namespace Rangers;

Label::Label(Object *parent): Sprite(parent)
{
    vertexCount = 0;
    labelFont = boost::shared_ptr<Font>();
    vertex = 0;
    textWordWrap = false;
    fixedSize = false;
    textureScaling = TEXTURE_NO;
}

Label::Label(const std::string& text, Object *parent, boost::shared_ptr<Font> font, SpriteXPosition xpos, SpriteYPosition ypos): Sprite(parent)
{
    if (!font)
        labelFont = Engine::instance()->defaultFont();
    else
        labelFont = font;

    xPos = xpos;
    yPos = ypos;
    setText(text);
    textWordWrap = false;
    fixedSize = false;
    textureScaling = TEXTURE_NO;
}

Label::Label(const std::wstring& text, Object *parent, boost::shared_ptr<Font> font, SpriteXPosition xpos, SpriteYPosition ypos): Sprite(parent)
{
    if (!font)
        labelFont = Engine::instance()->defaultFont();
    else
        labelFont = font;

    xPos = xpos;
    yPos = ypos;
    setText(text);
    textWordWrap = false;
    fixedSize = false;
    textureScaling = TEXTURE_NO;
}

Label::Label(const Rangers::Label& other): Sprite(other)
{
    labelFont = other.labelFont;
    labelText = other.labelText;
    textWordWrap = other.textWordWrap;
    fixedSize = other.fixedSize;
}

Label& Label::operator=(const Rangers::Label& other)
{
    if(this == &other)
	return *this;
    
    labelFont = other.labelFont;
    labelText = other.labelText;
    textWordWrap = other.textWordWrap;
    fixedSize = other.fixedSize;
    
    ::Sprite::operator=(other);    
    return *this;
}


boost::shared_ptr< Font > Label::font() const
{
    return labelFont;
}


void Label::setText(const std::string& text)
{
    lock();
    labelText.assign(text.length(), '\0');
    std::copy(text.begin(), text.end(), labelText.begin());
    markToUpdate();
    unlock();
}

void Label::setText(const std::wstring& text)
{
    lock();
    labelText = text;
    markToUpdate();
    unlock();
}

void Label::processMain()
{
    if(!labelFont)
        return;
    lock();

    if (!textWordWrap)
        spriteTexture = labelFont->renderText(labelText);
    else
        spriteTexture = labelFont->renderText(labelText, spriteWidth);

    if (!fixedSize)
    {
        spriteWidth = spriteTexture->width();
        spriteHeight = spriteTexture->height();
    }
    
    unlock();
    ::Sprite::processMain();
}

float Label::height() const
{
    return spriteHeight;
}

float Label::width() const
{
    return spriteWidth;
}

void Label::setWordWrap(bool wordWrap)
{
    textWordWrap = wordWrap;
    markToUpdate();
}

void Label::setFixedSize(float width, float height)
{
    if ((width > 0) && (height > 0))
    {
        spriteWidth = width;
        spriteHeight = height;
        fixedSize = true;
    }
    else
        fixedSize = false;
    markToUpdate();
}
