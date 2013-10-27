/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_FONT_H
#define RANGERS_FONT_H

#include "OpenSR/config.h"

#include <string>
#include <boost/shared_array.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Rangers
{
class Texture;
class RANGERS_ENGINE_API Font
{
public:
    Font(const char *data, size_t dataSize, int size, bool antialiased = true);
    Font(const std::wstring& file, int size, bool antialiased = true);
    ~Font();

    boost::shared_ptr<Texture> renderText(const std::string& text, int width = 0) const;
    boost::shared_ptr<Texture> renderText(const std::wstring& text, int width = 0) const;
    boost::shared_ptr<Texture> renderColoredText(const std::string& text, int defaultTextColor = 0xFFFFFF, int width = 0) const;
    boost::shared_ptr<Texture> renderColoredText(const std::wstring& text, int defaultTextColor = 0xFFFFFF, int width = 0) const;

    int calculateStringWidth(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last) const;
    int maxChars(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last, int width) const;

    int size() const;
    bool antialiased() const;

private:
    static void drawGlyph(unsigned char *dest, int destwidth, int destheight,
                          int x, int y, int w, int h, int pitch, unsigned char *data, bool antialiased);
    static void drawGlyph(unsigned char *dest, int destwidth, int destheight,
                          int x, int y, int w, int h, int pitch, unsigned char *data, unsigned int color, bool antialiased);
    FT_Face m_fontFace;
    int m_fontSize;
    boost::shared_array<char> m_fontData;
    bool m_antialiased;
};
}

#endif // FONT_H
