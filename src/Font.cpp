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

#include "Font.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <libRanger.h>
#include "Texture.h"

//TODO: Move this

namespace Rangers
{
extern FT_Library trueTypeLibrary;

Font::Font(const char* data, size_t dataSize, int size, bool antialiased):
    m_antialiased(antialiased), m_fontSize(size), m_fontData(0)
{
    m_fontData = new char[dataSize];
    memcpy(m_fontData, data, dataSize);
    int error = FT_New_Memory_Face(trueTypeLibrary, (FT_Byte *)m_fontData, dataSize, 0, &m_fontFace);
    FT_Set_Pixel_Sizes(m_fontFace, 0, size);
}

Font::~Font()
{
    FT_Done_Face(m_fontFace);
    if (m_fontData)
        delete m_fontData;
}

void Font::drawGlyph(unsigned char *dest, int destwidth, int destheight, int x, int y, int w, int h, int pitch, unsigned char *data, bool antialiased)
{
    if (antialiased)
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            dest[(y + i / w) * destwidth + x + i % w] |= data[i];
        }
    }
    else
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            //t = (i / w) * ((w - 1) / 8 + 1) + i % w
            dest[(y + i / w) * destwidth + x + i % w] |= data[(i / w * pitch) + (i % w) / 8] & (1 << (7 - i % w)) ? 0xff : 0;
        }
    }
}

void Font::drawGlyph(unsigned char *dest, int destwidth, int destheight, int x, int y, int w, int h, int pitch, unsigned char *data, unsigned int color, bool antialiased)
{
    if (antialiased)
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            dest[((y + i / w) * destwidth + x + i % w) * 4 + 0] = (color & 0xff0000) >> 16;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 1] = (color & 0xff00) >> 8;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 2] = color & 0xff;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 3] = data[i] & 0xff;
        }
    }
    else
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            dest[((y + i / w) * destwidth + x + i % w) * 4 + 0] = (color & 0xff0000) >> 16;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 1] = (color & 0xff00) >> 8;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 2] = color & 0xff;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 3] = data[(i / w * pitch) + (i % w) / 8] & (1 << (7 - i % w)) ? 0xff : 0;
        }
    }

}

boost::shared_ptr<Texture> Font::renderText(const std::string& t, int width) const
{
    std::wstring ws(t.length(), L'\0');
    std::copy(t.begin(), t.end(), ws.begin());
    return renderText(ws, width);
}

boost::shared_ptr<Texture> Font::renderColoredText(const std::string& t, int defaultTextColor, int width) const
{
    std::wstring ws(t.length(), L'\0');
    std::copy(t.begin(), t.end(), ws.begin());
    return renderColoredText(ws, defaultTextColor, width);
}

int Font::calculateStringWidth(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last) const
{
    int width = 0;
    int x = 0;
    for (std::wstring::const_iterator i = first; i != last; ++i)
    {
        if (*i == L'\n')
        {
            width = x > width ? x : width;
            x = 0;
            continue;
        }
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, *i);
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;
    }
    width = x > width ? x : width;
    return width;
}

int Font::maxChars(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last, int width) const
{
    int x = 0;
    for (std::wstring::const_iterator i = first; i < last; ++i)
    {
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, *i);
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;
        if (x > width)
            return i - first - 1;
    }
    return last - first;
}

boost::shared_ptr<Texture> Font::renderText(const std::wstring& t, int wrapWidth) const
{
    int x = 0;
    int y = 0;
    int lines = 1;
    int bearingY = 0;
    int width = 0;
    int i;

    std::wstring text = t;

    if ((wrapWidth) && (wrapWidth < (m_fontFace->max_advance_width >> 6)))
        return boost::shared_ptr<Texture>();


    int pos = 0;
    while ((pos = text.find('\t', pos)) >= 0)
    {
        text.replace(pos, 1, L"    ");
        pos += 4;
    }

    for (i = 0; i < text.length(); i++)
    {
        if (text.at(i) == '\n')
        {
            width = x > width ? x : width;
            x = 0;
            lines++;
            continue;
        }
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;

        if ((wrapWidth) && (x > wrapWidth))
        {
            int pos = 0;
            int lastnl = 0;
            while (1)
            {
                pos = text.find(L"\n", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastnl = pos;
                pos++;
            }


            int lastsp = lastnl;
            pos = lastnl;
            while (1)
            {
                pos = text.find(L" ", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastsp = pos;
                pos++;
            }

            if (lastsp == lastnl)
                text.insert(i - 1, L"-\n");
            else
                text.replace(lastsp, 1, L"\n");

            x = 0;
            lines = 1;
            width = 0;
            bearingY = 0;
            i = 0;
        }
    }
    width = x > width ? x : width;

    //FIXME: Baseline workaround.
    FT_UInt glyph_index;
    glyph_index = FT_Get_Char_Index(m_fontFace, 'f');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'W');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'A');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    //bearingY = (fontFace->size->metrics.ascending >> 6) - fontSize/10 -1;


    int fullHeight = lines * m_fontSize;
    int lineHeight = m_fontSize;
    int fullWidth;

    if (wrapWidth)
        fullWidth = wrapWidth;
    else
        fullWidth = width;

    x = 0;
    y = 0;
    int line = 0;

    unsigned char *textureData = new unsigned char[fullWidth * fullHeight];
    memset(textureData, 0, fullWidth * fullHeight);

    for (i = 0; i < text.length(); i++)
    {
        if (text.at(i) == '\n')
        {
            x = 0;
            line++;
        }
        else
        {
            FT_UInt glyph_index;
            glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
            FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
            if (m_antialiased)
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_NORMAL);
            else
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_MONO);

            drawGlyph(textureData, fullWidth, fullHeight,
                      x + m_fontFace->glyph->bitmap_left, bearingY - m_fontFace->glyph->bitmap_top + line * m_fontSize,
                      m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows, m_fontFace->glyph->bitmap.pitch,
                      m_fontFace->glyph->bitmap.buffer, m_antialiased);

            x += m_fontFace->glyph->advance.x >> 6;
        }
    }
    Texture *texture = new Texture(fullWidth, fullHeight, TEXTURE_A8, textureData);
    delete[] textureData;
    return boost::shared_ptr<Texture>(texture);
}

boost::shared_ptr<Texture> Font::renderColoredText(const std::wstring& t, int defaultTextColor, int wrapWidth) const
{
    int x = 0;
    int y = 0;
    int lines = 1;
    int bearingY = 0;
    int width = 0;
    int i;

    std::map<int, unsigned int> colorSelect;
    unsigned int currentColor = defaultTextColor;

    std::wstring text = t;

    if ((wrapWidth) && (wrapWidth < (m_fontFace->max_advance_width >> 6)))
        return boost::shared_ptr<Texture>();


    int pos = 0;
    while ((pos = text.find('\t', pos)) >= 0)
    {
        text.replace(pos, 1, L"    ");
        pos += 4;
    }
    pos = 0;
    //TODO: hex check
    while ((pos = text.find(L"\\c", pos)) >= 0)
    {
        if (text.length() < pos + 3)
            break;

        if (text[pos + 2] == L'R')
        {
            colorSelect[pos] = defaultTextColor;
            text.erase(pos, 3);
            continue;
        }
        if (text.length() < pos + 8)
            break;

        std::wstring colorstring = text.substr(pos + 2, 6);
        text.erase(pos, 8);
        std::wistringstream ss(colorstring);
        int color;
        ss >> std::hex >> color;
        colorSelect[pos] = color;
    }

    for (i = 0; i < text.length(); i++)
    {
        if (text.at(i) == '\n')
        {
            width = x > width ? x : width;
            x = 0;
            lines++;
            continue;
        }
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;

        if ((wrapWidth) && (x > wrapWidth))
        {
            int pos = 0;
            int lastnl = 0;
            while (1)
            {
                pos = text.find(L"\n", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastnl = pos;
                pos++;
            }


            int lastsp = lastnl;
            pos = lastnl;
            while (1)
            {
                pos = text.find(L" ", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastsp = pos;
                pos++;
            }

            if (lastsp == lastnl)
                text.insert(i - 1, L"-\n");
            else
                text.replace(lastsp, 1, L"\n");

            x = 0;
            lines = 1;
            width = 0;
            bearingY = 0;
            i = 0;
        }
    }
    width = x > width ? x : width;

    //FIXME: Baseline workaround.
    FT_UInt glyph_index;
    glyph_index = FT_Get_Char_Index(m_fontFace, 'f');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'W');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'A');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    //bearingY = (fontFace->size->metrics.ascending >> 6) - fontSize/10 -1;


    int fullHeight = lines * m_fontSize;
    int lineHeight = m_fontSize;
    int fullWidth;

    if (wrapWidth)
        fullWidth = wrapWidth + 1;
    else
        fullWidth = width + 1;

    x = 0;
    y = 0;
    int line = 0;

    unsigned char *textureData = new unsigned char[fullWidth * fullHeight * 4];
    memset(textureData, 0, fullWidth * fullHeight * 4);

    for (i = 0; i < text.length(); i++)
    {
        std::map<int, unsigned int>::const_iterator it;
        if ((it = colorSelect.find(i)) != colorSelect.end())
            currentColor = it->second;
        if (text.at(i) == '\n')
        {
            x = 0;
            line++;
        }
        else
        {
            FT_UInt glyph_index;
            glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
            FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
            if (m_antialiased)
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_NORMAL);
            else
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_MONO);

            drawGlyph(textureData, fullWidth, fullHeight,
                      x + m_fontFace->glyph->bitmap_left, bearingY - m_fontFace->glyph->bitmap_top + line * m_fontSize,
                      m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows, m_fontFace->glyph->bitmap.pitch,
                      m_fontFace->glyph->bitmap.buffer, currentColor, m_antialiased);

            x += m_fontFace->glyph->advance.x >> 6;
        }
    }
    Texture *texture = new Texture(fullWidth, fullHeight, TEXTURE_R8G8B8A8, textureData);
    delete[] textureData;
    return boost::shared_ptr<Texture>(texture);
}

int Font::size() const
{
    return m_fontSize;
}
}
