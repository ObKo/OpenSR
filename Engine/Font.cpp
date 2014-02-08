/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Font.h"
#include "OpenSR/Texture.h"

namespace Rangers
{
Font::Font()
{
    m_fontSize = 0;
}

Font::~Font()
{
}

boost::shared_ptr<Texture> Font::renderText(const std::string& t, int width) const
{
    return boost::shared_ptr<Texture>();
}

boost::shared_ptr<Texture> Font::renderColoredText(const std::string& t, int defaultTextColor, int selectionTextColor, int width) const
{
    return boost::shared_ptr<Texture>();
}

int Font::calculateStringWidth(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last) const
{
    return 0;
}

int Font::maxChars(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last, int width) const
{
    return 0;
}

boost::shared_ptr<Texture> Font::renderText(const std::wstring& t, int wrapWidth) const
{
    return boost::shared_ptr<Texture>();
}

boost::shared_ptr<Texture> Font::renderColoredText(const std::wstring& t, int defaultTextColor, int selectionTextColor, int wrapWidth) const
{
    return boost::shared_ptr<Texture>();
}

int Font::size() const
{
    return m_fontSize;
}
}
