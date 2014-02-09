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

#include "OpenSR/libRanger.h"
#include <string.h>


namespace Rangers
{
namespace
{
//TODO: Antialiasing
void drawGlyph(AFTLayerHeader& intLayer, uint8_t *intLayerData, AFTLayerHeader& alphaLayer, uint8_t *alphaLayerData, AFTGlyph& glyph)
{
    uint32_t count;
    uint32_t w = 0;
    uint32_t h = 0;
    uint32_t cnt;
    int32_t x, y;

    if (intLayerData)
    {
        w = ((uint32_t*)intLayerData)[1];
        h = ((uint32_t*)intLayerData)[2];
    }

    if (alphaLayerData)
    {
        w = w < ((uint32_t*)alphaLayerData)[1] ? ((uint32_t*)alphaLayerData)[1] : w;
        h = h < ((uint32_t*)alphaLayerData)[2] ? ((uint32_t*)alphaLayerData)[2] : h;
    }

    if (alphaLayerData)
    {
        uint32_t dx = std::min(intLayer.x, alphaLayer.x);
        uint32_t dy = std::min(intLayer.y, alphaLayer.y);
        glyph.x += dx;
        glyph.y += dy;
        intLayer.x -= dx;
        intLayer.y -= dy;
        alphaLayer.x -= dx;
        alphaLayer.y -= dy;
    }
    else
    {
        glyph.x += intLayer.x;
        glyph.y += intLayer.y;
        intLayer.x = 0;
        intLayer.y = 0;
    }

    if (intLayer.width + intLayer.x > w)
        w = intLayer.width + intLayer.x;

    if (alphaLayer.width + alphaLayer.x > w)
        w = alphaLayer.width + alphaLayer.x;


    if (intLayer.height + intLayer.y > h)
        h = intLayer.height + intLayer.y;

    if (alphaLayer.height + alphaLayer.y > h)
        h = alphaLayer.height + alphaLayer.y;

    glyph.data = new uint8_t[w * h];
    memset(glyph.data, 0, w * h);

    glyph.width = w;
    glyph.height = h;

    if (intLayerData)
    {
        count = ((uint32_t*)intLayerData)[0];
        w = ((uint32_t*)intLayerData)[1];
        h = ((uint32_t*)intLayerData)[2];
        cnt = 0;
        x = intLayer.x;
        y = intLayer.y;

        for (uint32_t i = 0; (i < intLayer.size) && (cnt < count) && ((y - intLayer.y) < h); i++)
        {
            uint8_t b = *(intLayerData + 16 + i);

            if (b == 0)
                cnt++;
            else if (b == 0x80)
            {
                y++;
                cnt++;
            }
            else if (b & 0x80)
            {
                uint8_t n = b & 0x7F;
                for (int j = 0; (j < n) && (i < intLayer.size) && (cnt < count) && ((y - intLayer.y) < h); j++)
                {
                    if ((y * glyph.width + x) < glyph.height * glyph.width)
                        glyph.data[y * glyph.width + x] = 0xFF;

                    x++;
                    if (x - intLayer.x >= w)
                    {
                        y++;
                        x = intLayer.x;
                    }
                }
            }
            else
            {
                cnt++;
                x += b;
                if (x - intLayer.x >= w)
                {
                    y += (x - intLayer.x) / w;
                    x = (x - intLayer.x) % w + intLayer.x;
                }
            }
        }
    }
    if (alphaLayerData)
    {
        count = ((uint32_t*)alphaLayerData)[0];
        w = ((uint32_t*)alphaLayerData)[1];
        h = ((uint32_t*)alphaLayerData)[2];
        cnt = 0;
        x = alphaLayer.x;
        y = alphaLayer.y;

        for (uint32_t i = 0; (i < alphaLayer.size) && (cnt < count) && ((y - alphaLayer.y) < h); i++)
        {
            uint8_t b = *(alphaLayerData + 16 + i);

            if (b == 0)
                cnt++;
            else if (b == 0x80)
            {
                y++;
                cnt++;
            }
            else if (b & 0x80)
            {
                uint8_t n = b & 0x7F;
                cnt += n;
                for (int j = 0; (j < n) && (i < alphaLayer.size) && (cnt < count) && ((y - alphaLayer.y) < h); j++)
                {
                    if ((y * glyph.width + x) < glyph.height * glyph.width)
                        glyph.data[y * glyph.width + x] = *(alphaLayerData + 16 + i + 1);
                    i++;
                    x++;
                    if (x - alphaLayer.x >= w)
                    {
                        y++;
                        x = alphaLayer.x;
                    }
                }
            }
            else
            {
                cnt++;
                x += b;
                if (x - alphaLayer.x >= w)
                {
                    y += (x - alphaLayer.x) / w;
                    x = alphaLayer.x + (x - alphaLayer.x) % w;
                }
            }
        }
    }
}
}

AFT loadAFTFont(std::istream &stream)
{
    AFT result;
    result.glyphCount = 0;
    result.glyphs = 0;
    result.height = 0;
    result.size = 0;

    AFTHeader header;

    stream.read((char*)&header, sizeof(AFTHeader));

    if (header.sig != 0x00746661)
        return result;

    result.glyphCount = header.charCount;
    result.size = header.height;
    result.height = header.height + header.verticalSpace * 2;
    result.glyphs = new AFTGlyph[result.glyphCount];

    AFTLayerHeader *intLayers = new AFTLayerHeader[result.glyphCount];
    AFTLayerHeader *alphaLayers = new AFTLayerHeader[result.glyphCount];

    for (int i = 0; i < result.glyphCount; i++)
    {
        AFTGlyphHeader gHeader;
        stream.read((char*)&gHeader, sizeof(AFTGlyphHeader));
        result.glyphs[i].code = gHeader.code;
        result.glyphs[i].height = result.height;
        result.glyphs[i].stride = gHeader.charWidth + gHeader.leftSpace + gHeader.rightSpace;
        result.glyphs[i].width = gHeader.charWidth + gHeader.leftSpace + gHeader.rightSpace;
        result.glyphs[i].data = 0;

        stream.read((char*) & (intLayers[i]), sizeof(AFTLayerHeader));
        stream.read((char*) & (alphaLayers[i]), sizeof(AFTLayerHeader));

        intLayers[i].y = result.height + intLayers[i].y - header.verticalSpace;
        alphaLayers[i].y = result.height + alphaLayers[i].y - header.verticalSpace;
        if (intLayers[i].y < 0)
            intLayers[i].y = 0;
        if (alphaLayers[i].y < 0)
            alphaLayers[i].y = 0;

        result.glyphs[i].y = 0;
        result.glyphs[i].x = 0;
    }

    for (int i = 0; i < result.glyphCount; i++)
    {
        uint8_t *intLayerData = 0;
        uint8_t *alphaLayerData = 0;

        if (intLayers[i].offset && intLayers[i].size)
        {
            intLayerData = new uint8_t[intLayers[i].size];
            stream.seekg(intLayers[i].offset, std::ios_base::beg);
            stream.read((char*)intLayerData, intLayers[i].size);
        }
        if (alphaLayers[i].offset && alphaLayers[i].size)
        {
            alphaLayerData = new uint8_t[alphaLayers[i].size];
            stream.seekg(alphaLayers[i].offset, std::ios_base::beg);
            stream.read((char*)alphaLayerData, alphaLayers[i].size);
        }

        drawGlyph(intLayers[i], intLayerData, alphaLayers[i], alphaLayerData, result.glyphs[i]);

        delete[] intLayerData;
        delete[] alphaLayerData;
    }

    delete[] intLayers;
    delete[] alphaLayers;

    return result;
}
}
