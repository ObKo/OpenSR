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

#ifndef RANGERS_FONT_H
#define RANGERS_FONT_H
#include <string>
#include <ft2build.h>
#include <boost/shared_ptr.hpp>
#include FT_FREETYPE_H

namespace Rangers
{
class Texture;
class Font
{
public:
    Font(const std::wstring& path, int size);
    Font(const char *data, size_t dataSize, int size);
    ~Font();

    boost::shared_ptr<Texture> renderText(const std::string& text, int width = 0) const;
    boost::shared_ptr<Texture> renderText(const std::wstring& text, int width = 0) const;
    boost::shared_ptr<Texture> renderColoredText(const std::string& text, int defaultTextColor = 0xFFFFFF, int width = 0) const;
    boost::shared_ptr<Texture> renderColoredText(const std::wstring& text, int defaultTextColor = 0xFFFFFF, int width = 0) const;

    int calculateStringWidth(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last) const;

    static void drawGlyph(unsigned char *dest, int destwidth, int destheight,
                          int x, int y, int w, int h, unsigned char *data);
    static void drawGlyph(unsigned char *dest, int destwidth, int destheight,
                          int x, int y, int w, int h, unsigned char *data, unsigned int color);

    int size() const;

private:
    FT_Face m_fontFace;
    int m_fontSize;
    char *m_fontData;
};
}

#endif // FONT_H
