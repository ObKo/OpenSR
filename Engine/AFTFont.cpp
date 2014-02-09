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

#include "OpenSR/AFTFont.h"

#include "OpenSR/Texture.h"

#include <string.h>
#include <sstream>

namespace Rangers
{
namespace
{
void drawGlyph(const AFTGlyph& glyph, uint32_t x, uint32_t y, uint8_t *target, uint32_t targetWidth, uint32_t targetHeight)
{
    uint32_t rx = x + glyph.x;
    uint32_t ry = y + glyph.y;
    for (int i = 0; i < glyph.width * glyph.height; i++)
    {
        if (((ry + (i / glyph.width)) * targetWidth + rx + i % glyph.width) < targetWidth * targetHeight)
            target[(ry + (i / glyph.width)) * targetWidth + rx + i % glyph.width] = glyph.data[i];
    }
}
void drawGlyph(const AFTGlyph& glyph, uint32_t x, uint32_t y, uint8_t *target, uint32_t targetWidth, uint32_t targetHeight, uint32_t color)
{
    uint32_t rx = x + glyph.x;
    uint32_t ry = y + glyph.y;
    for (int i = 0; i < glyph.width * glyph.height; i++)
    {
        if (((ry + (i / glyph.width)) * targetWidth + rx + i % glyph.width) < targetWidth * targetHeight)
            ((uint32_t*)target)[(ry + (i / glyph.width)) * targetWidth + rx + i % glyph.width] = (glyph.data[i] << 24) | (color & 0xFFFFFF);
    }
}
}
AFTFont::AFTFont(const AFT& font): Font()
{
    m_aft = font;
    m_fontSize = m_aft.size;

    for (int i = 0; i < m_aft.glyphCount; i++)
    {
        m_charMap[m_aft.glyphs[i].code] = &(m_aft.glyphs[i]);
    }
}

AFTFont::~AFTFont()
{
    for (int i = 0; i < m_aft.glyphCount; i++)
    {
        delete[] m_aft.glyphs[i].data;
    }
    delete[] m_aft.glyphs;
}

//TODO: Move common code to Font class
int AFTFont::calculateStringWidth(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last) const
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
        auto charGlyph = m_charMap.find(*i);
        if (charGlyph != m_charMap.end())
            x += (*charGlyph).second->stride;
    }
    width = x > width ? x : width;
    return width;
}

//TODO: Move common code to Font class
int AFTFont::maxChars(const std::wstring::const_iterator& first, const std::wstring::const_iterator& last, int width) const
{
    int x = 0;
    for (std::wstring::const_iterator i = first; i < last; ++i)
    {
        auto charGlyph = m_charMap.find(*i);
        if (charGlyph != m_charMap.end())
            x += (*charGlyph).second->stride;

        if (x > width)
            return i - first;
    }
    return last - first;
}

//TODO: Move common code to Font class
boost::shared_ptr<Texture> AFTFont::renderText(const std::wstring& t, int wrapWidth) const
{
    int x = 0;
    int lines = 1;
    int bearingY = 0;
    int width = 0;
    int i;

    std::wstring text = t;

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
        auto charGlyph = m_charMap.find(text.at(i));
        if (charGlyph != m_charMap.end())
            x += (*charGlyph).second->stride;

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

    int fullHeight = lines * m_aft.height;
    int fullWidth;

    if (wrapWidth)
        fullWidth = wrapWidth;
    else
        fullWidth = width;

    x = 0;
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
            auto charGlyph = m_charMap.find(text.at(i));
            if (charGlyph == m_charMap.end())
                continue;

            AFTGlyph *g = (*charGlyph).second;

            drawGlyph(*g, x, line * m_aft.height,
                      textureData, fullWidth, fullHeight);

            x += g->stride;
        }
    }
    Texture *texture = new Texture(fullWidth, fullHeight, TEXTURE_A8, textureData);
    delete[] textureData;
    return boost::shared_ptr<Texture>(texture);
}

//TODO: Move common code to Font class
boost::shared_ptr<Texture> AFTFont::renderColoredText(const std::wstring& t, int defaultTextColor, int selectionTextColor, int wrapWidth) const
{
    int x = 0;
    int lines = 1;
    int bearingY = 0;
    int width = 0;
    int i;

    std::map<int, unsigned int> colorSelect;
    unsigned int currentColor = defaultTextColor;

    std::wstring text = t;

    int pos = 0;
    while ((pos = text.find('\t', pos)) >= 0)
    {
        text.replace(pos, 1, L"    ");
        pos += 4;
    }

    pos = 0;
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
        if (text[pos + 2] == L'S')
        {
            colorSelect[pos] = selectionTextColor;
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
        auto charGlyph = m_charMap.find(text.at(i));
        if (charGlyph != m_charMap.end())
            x += (*charGlyph).second->stride;

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

    int fullHeight = lines * m_aft.height;
    int fullWidth;

    if (wrapWidth)
        fullWidth = wrapWidth;
    else
        fullWidth = width;

    x = 0;
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
            auto charGlyph = m_charMap.find(text.at(i));
            if (charGlyph == m_charMap.end())
                continue;

            AFTGlyph *g = (*charGlyph).second;

            drawGlyph(*g, x, line * m_aft.height, textureData, fullWidth, fullHeight, currentColor);

            x += g->stride;
        }
    }
    Texture *texture = new Texture(fullWidth, fullHeight, TEXTURE_B8G8R8A8, textureData);
    delete[] textureData;
    return boost::shared_ptr<Texture>(texture);
}


}