/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include <cstring>
#include <fstream>

using namespace Rangers;
using namespace std;

void drawA6ToARGB(uint8_t* dest, int destwidth, int x, int y, const uint8_t *data)
{
    int size, cnt, line = y;

    size = *(uint32_t *)data;
    data += 16;

    unsigned char *row = dest + (line * destwidth + x) * 4 + 3;

    while (size > 0)
    {
        uint8_t byte = *data;
        data++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = dest + (line * destwidth + x) * 4 + 3;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            do
            {
                *row = 4 * (63 - *data);
                row += 4;
                data++;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * 4;
        }

        size--;
    }
}

void drawR5G6B5ToR5G6B5(uint8_t* dest, int destwidth, int x, int y, const uint8_t *data)
{
    int size, cnt, line = y;

    size = *(uint32_t *)data;
    data += 16;

    unsigned char *row = dest + (line * destwidth + x) * 2;

    while (size > 0)
    {
        uint8_t byte = *data;
        data++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = dest + (line * destwidth + x) * 2;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt * 2;

            do
            {
                *((uint16_t *)row) = *((uint16_t *)data);
                row += 2;
                data += 2;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * 2;
        }

        size--;
    }
}

void drawR5G6B5ToARGB(uint8_t* dest, int destwidth, int x, int y, const uint8_t *data)
{
    int size, cnt, line = y;

    size = *(uint32_t *)data;
    data += 16;

    unsigned char *row = dest + (line * destwidth + x) * 4;

    while (size > 0)
    {
        uint8_t byte = *data;
        data++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = dest + (line * destwidth + x) * 4;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt * 2;

            do
            {
                uint16_t color = *((uint16_t *)data);
                *((uint32_t *)row) = (0xff000000) | (((color >> 11) & 0x1f) << 19) | (((color >> 5) & 0x3f) << 10) | (((color) & 0x1f) << 3);
                row += 4;
                data += 2;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * 4;
        }

        size--;
    }
}

void drawAIToARGB(uint8_t* dest, int destwidth, int x, int y, const uint8_t *data)
{
    int size, cnt, palsize, i, line = y;
    uint32_t pal[256];

    size = *(uint32_t *)data;
    palsize = *(data + 12);

    if (palsize == 0)
        palsize = 256;

    data += 16;

    for (i = 0; i < palsize; i++)
    {
        uint16_t color = *((uint16_t*)data);
        uint8_t alpha = 4 * (63 - * (data + 2));

        if (alpha > 0)
        {
            uint16_t r = ((((color >> 11) & 0x1f) << 3) * 0xff) / alpha;
            uint16_t g = ((((color >> 5) & 0x3f) << 2) * 0xff) / alpha;
            uint16_t b = ((((color) & 0x1f) << 3) * 0xff) / alpha;
            pal[i] = (alpha << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
        }
        else
        {
            pal[i] = 0;
        }

        data += 4;
    }

    unsigned char *row = dest + (line * destwidth + x) * 4;

    while (size > 0)
    {
        uint8_t byte = *data;
        data++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = dest + (line * destwidth + x) * 4;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            do
            {
                uint8_t color = *data;
                *((uint32_t *)row) = pal[color];
                row += 4;
                data++;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * 4;
        }

        size--;
    }
}

void drawRGBIToARGB(uint8_t* dest, int destwidth, int x, int y, const uint8_t *data)
{
    int size, cnt, palsize, i, line = y;
    uint16_t pal[256];

    size = *(uint32_t *)data;
    palsize = *(data + 12);

    if (palsize == 0)
        palsize = 256;

    data += 16;

    for (i = 0; i < palsize; i++)
    {
        pal[i] = *((uint16_t *)data);
        data += 2;
    }

    unsigned char *row = dest + (line * destwidth + x) * 4;

    while (size > 0)
    {
        uint8_t byte = *data;
        data++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = dest + (line * destwidth + x) * 4;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            do
            {
                uint8_t color = *data;
                *((uint32_t *)row) = (0xff000000) | (((pal[color] >> 11) & 0x1f) << 19) | (((pal[color] >> 5) & 0x3f) << 10) | (((pal[color]) & 0x1f) << 3);
                row += 4;
                data++;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * 4;
        }

        size--;
    }
}

void Rangers::decodeGAIDeltaFrame(uint8_t *bufdes, int prevWidth, int x, int y, const uint8_t* graphbuf)
{
    int i, cnt, cnt2, shlc;
    int shlca[4];
    unsigned char byte;
    int line = y;

    byte = graphbuf[12];
    shlc = shlca[0] = 8 - (byte & 15);
    shlca[1] = 8 - (byte >> 4);
    byte = graphbuf[13];
    shlca[2] = 8 - (byte & 15);
    shlca[3] = 8 - (byte >> 4);

    graphbuf += 16 + (((uint32_t *)graphbuf)[2] << 2);

    unsigned char * bufdesrow = bufdes + (prevWidth * line + x) * 4;

    uint32_t channel = 0;
    while (true)
    {
        byte = *graphbuf;
        graphbuf++;

        if ((byte & 0x80) != 0)
        {
            cnt = (byte & 0xf) + 1;
            byte = (byte >> 4) & 7;
            if (byte == 0)
            {
                do
                {
                    cnt2 = (cnt > 8) ? 8 : cnt;
                    byte = *graphbuf;
                    graphbuf++;
                    for (i = 0; i < cnt2; i++)
                    {
                        *bufdesrow = *bufdesrow + (((byte & 1) + 1) << shlc);
                        byte >>= 1;
                        bufdesrow += 4;
                    }
                    cnt -= cnt2;
                }
                while (cnt > 0);
            }
            else if (byte == 1)
            {
                do
                {
                    cnt2 = (cnt > 4) ? 4 : cnt;
                    byte = *graphbuf;
                    graphbuf++;
                    for (i = 0; i < cnt2; i++)
                    {
                        *bufdesrow = *bufdesrow + (((byte & 3) + 1) << shlc);
                        byte >>= 2;
                        bufdesrow += 4;
                    }
                    cnt -= cnt2;
                }
                while (cnt > 0);
            }
            else if (byte == 2)
            {
                do
                {
                    cnt2 = (cnt > 2) ? 2 : cnt;
                    byte = *graphbuf;
                    graphbuf++;
                    for (i = 0; i < cnt2; i++)
                    {
                        *bufdesrow = *bufdesrow + (((byte & 15) + 1) << shlc);
                        byte >>= 4;
                        bufdesrow += 4;
                    }
                    cnt -= cnt2;
                }
                while (cnt > 0);
            }
            else if (byte == 3)
            {
                for (i = 0; i < cnt; i++)
                {
                    byte = *graphbuf;
                    graphbuf++;
                    *bufdesrow = *bufdesrow + ((byte + 1) << shlc);
                    bufdesrow += 4;
                }
            }
            else if (byte == 4)
            {
                do
                {
                    cnt2 = (cnt > 8) ? 8 : cnt;
                    byte = *graphbuf;
                    graphbuf++;
                    for (i = 0; i < cnt2; i++)
                    {
                        *bufdesrow = *bufdesrow - (((byte & 1) + 1) << shlc);
                        byte >>= 1;
                        bufdesrow += 4;
                    }
                    cnt -= cnt2;
                }
                while (cnt > 0);
            }
            else if (byte == 5)
            {
                do
                {
                    cnt2 = (cnt > 4) ? 4 : cnt;
                    byte = *graphbuf;
                    graphbuf++;
                    for (i = 0; i < cnt2; i++)
                    {
                        *bufdesrow = *bufdesrow - (((byte & 3) + 1) << shlc);
                        byte >>= 2;
                        bufdesrow += 4;
                    }
                    cnt -= cnt2;
                }
                while (cnt > 0);
            }
            else if (byte == 6)
            {
                do
                {
                    cnt2 = (cnt > 2) ? 2 : cnt;
                    byte = *graphbuf;
                    graphbuf++;
                    for (i = 0; i < cnt2; i++)
                    {
                        *bufdesrow = *bufdesrow - (((byte & 15) + 1) << shlc);
                        byte >>= 4;
                        bufdesrow += 4;
                    }
                    cnt -= cnt2;
                }
                while (cnt > 0);
            }
            else if (byte == 7)
            {
                for (i = 0; i < cnt; i++)
                {
                    byte = *graphbuf;
                    graphbuf++;
                    *bufdesrow = *bufdesrow - ((byte + 1) << shlc);
                    bufdesrow += 4;
                }
            }
        }
        else if (byte == 0)
        {
            channel++;
            if (channel < 4)
            {
                bufdesrow = bufdes + (prevWidth * line + x) * 4 + channel;
            }
            else
            {
                channel = 0;
                line++;
                bufdesrow = bufdes + (prevWidth * line + x) * 4;
            }
            shlc = shlca[channel];
        }
        else if (byte == 0x03f)
        {
            bufdesrow += uint32_t(*((uint16_t *)graphbuf)) << 2;
            graphbuf += 2;
        }
        else if ((byte & 0xc0) == 0)
        {
            bufdesrow += uint32_t(byte) << 2;
        }
        else if (byte == 0x40)
        {
            break;
        }
    }
}

void blitARGB(uint8_t *dest, int destwidth, int x, int y, int w, int h, const uint8_t *graphbuf)
{
    for (int i = y; i < y + h; i++)
        memcpy(dest + (i * destwidth + x) * 4, graphbuf + (i - y) * w * 4, w * 4);
}

void blitR5G6B5(uint8_t *dest, int destwidth, int x, int y, int w, int h, const uint8_t *graphbuf)
{
    for (int i = y; i < y + h; i++)
        memcpy(dest + (i * destwidth + x) * 2, graphbuf + (i - y) * w * 2, w * 2);
}

void blitABGRI(uint8_t *dest, int destwidth, int x, int y, int w, int h, const uint8_t *graphbuf, const uint8_t *pal)
{
    for (int i = y; i < y + h; i++)
    {
        uint8_t *row = dest + (i * destwidth + x) * 4;

        for (int j = 0; j < w; j++)
        {
            uint32_t abgr = ((uint32_t *)pal)[graphbuf[(i - y) * w + j]];
            row[j] = (abgr & 0xFF000000) | ((abgr >> 16) & 0xFF) | (((abgr >> 8) & 0xFF) << 8) | ((abgr & 0xFF) << 16);
        }
    }
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadGIImageData(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, GIFrame *background, uint32_t offset)
{
    GIFrame resultFrame;

    switch (image.type)
    {
    case 0:
        resultFrame = loadFrameType0(image, layers, stream, offset);
        break;

    case 1:
        resultFrame = loadFrameType1(image, layers, stream, offset);
        break;

    case 2:
        resultFrame = loadFrameType2(image, layers, stream, offset);
        break;

    case 3:
        resultFrame = loadFrameType3(image, layers, stream, offset);
        break;

    case 4:
        resultFrame = loadFrameType4(image, layers, stream, offset);
        break;
    case 5:
        resultFrame = loadFrameType5(image, layers, stream, background, offset);
        break;

    default:
        cout << "Unknown GI frame type: " << image.type << endl;
        resultFrame = GIFrame();
        break;
    }

    return resultFrame;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType2(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset)
{
    GIFrame result;

    if (image.type != 2 || image.layerCount < 3 || !layers)
        return result;

    int width = image.finishX;
    int height = image.finishY;

    result.width = width;
    result.height = height;

    result.format = GIFrame::Format_ARGB32;
    result.data = new unsigned char[width * height * 4];
    memset(result.data, 0, width * height * 4);


    if (layers[0].size)
    {
        uint8_t *layerData = new uint8_t[layers[0].size];
        stream.seekg(offset + layers[0].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[0].size);

        drawR5G6B5ToARGB(result.data, width, layers[0].startX, layers[0].startY, layerData);

        delete[] layerData;
    }

    if (layers[1].size)
    {
        uint8_t *layerData = new uint8_t[layers[1].size];
        stream.seekg(offset + layers[1].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[1].size);

        drawR5G6B5ToARGB(result.data, width, layers[1].startX, layers[1].startY, layerData);

        delete[] layerData;
    }

    if (layers[2].size)
    {
        uint8_t *layerData = new uint8_t[layers[2].size];
        stream.seekg(offset + layers[2].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[2].size);

        drawA6ToARGB(result.data, width, layers[2].startX, layers[2].startY, layerData);

        delete[] layerData;
    }

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType5(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, GIFrame *background, uint32_t offset)
{
    GIFrame result;

    if (image.type != 5 || !image.layerCount || !layers)
        return result;

    int width = image.finishX;
    int height = image.finishY;

    result.width = width;
    result.height = height;

    result.format = GIFrame::Format_ARGB32;
    result.data = new unsigned char[width * height * 4];

    if (background)
        memcpy(result.data, background->data, width * height * 4);
    else
        memset(result.data, 0x0, width * height * 4);


    if (layers[0].size)
    {
        uint8_t *layerData = new uint8_t[layers[0].size];
        stream.seekg(offset + layers[0].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[0].size);

        decodeGAIDeltaFrame(result.data, width, layers[0].startX, layers[0].startY, layerData);

        delete[] layerData;
    }

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType1(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset)
{
    GIFrame result;

    if (image.type != 1 || image.layerCount < 1 || !layers)
        return result;

    int width = image.finishX;
    int height = image.finishY;

    result.width = width;
    result.height = height;

    result.format = GIFrame::Format_RGB16;
    result.data = new unsigned char[width * height * 2];
    memset(result.data, 0, width * height * 2);

    if (layers[0].size)
    {
        uint8_t *layerData = new uint8_t[layers[0].size];
        stream.seekg(offset + layers[0].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[0].size);

        drawR5G6B5ToR5G6B5(result.data, width, layers[0].startX, layers[0].startY, layerData);

        delete[] layerData;
    }

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType4(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset)
{
    GIFrame result;

    if (image.type != 4 || image.layerCount < 2 || !layers)
        return result;

    int width = image.finishX;
    int height = image.finishY;

    result.width = width;
    result.height = height;

    result.format = GIFrame::Format_ARGB32;
    result.data = new unsigned char[width * height * 2];
    memset(result.data, 0, width * height * 2);

    if (layers[0].size && layers[1].size)
    {
        uint8_t *layerData = new uint8_t[layers[0].size];
        uint8_t *palData = new uint8_t[layers[1].size];

        stream.seekg(offset + layers[0].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[0].size);
        stream.seekg(offset + layers[1].seek, std::ios_base::beg);
        stream.read((char*)palData, layers[1].size);

        blitABGRI(result.data, width, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), layerData, palData);

        delete[] layerData;
        delete[] palData;
    }

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType3(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset)
{
    GIFrame result;

    if (image.type != 3 || image.layerCount < 2 || !layers)
        return result;

    int width = image.finishX;
    int height = image.finishY;

    result.width = width;
    result.height = height;

    result.format = GIFrame::Format_ARGB32;
    result.data = new unsigned char[width * height * 4];
    memset(result.data, 0, width * height * 4);


    if (layers[0].size)
    {
        uint8_t *layerData = new uint8_t[layers[0].size];
        stream.seekg(offset + layers[0].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[0].size);

        drawRGBIToARGB(result.data, width, layers[0].startX, layers[0].startY, layerData);

        delete[] layerData;
    }

    if (layers[1].size)
    {
        uint8_t *layerData = new uint8_t[layers[1].size];
        stream.seekg(offset + layers[1].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[1].size);

        drawAIToARGB(result.data, width, layers[1].startX, layers[1].startY, layerData);

        delete[] layerData;
    }

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType0(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset)
{
    GIFrame result;

    if (image.type != 0 || !image.layerCount || !layers)
        return result;

    int width = image.finishX;
    int height = image.finishY;

    result.width = width;
    result.height = height;

    uint8_t *layerData = 0;
    if (layers[0].size)
    {
        layerData = new uint8_t[layers[0].size];
        stream.seekg(layers[0].seek, std::ios_base::beg);
        stream.read((char*)layerData, layers[0].size);
    }

    if ((image.aBitmask == 0xFF000000) && (image.rBitmask == 0xFF0000) && (image.gBitmask == 0xFF00) && (image.bBitmask == 0xFF))
    {
        result.format = GIFrame::Format_ARGB32;
        result.data = new unsigned char[width * height * 4];
        memset(result.data, 0, width * height * 4);

        if (layers[0].size)
            blitARGB(result.data, width, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), layerData);
    }
    else if ((image.rBitmask == 0xF800) && (image.gBitmask == 0x7E0) && (image.bBitmask == 0x1F))
    {
        result.format = GIFrame::Format_RGB16;
        result.data = new unsigned char[width * height * 2];
        memset(result.data, 0, width * height * 2);

        if (layers[0].size)
            blitR5G6B5(result.data, width, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), layerData);
    }

    delete[] layerData;

    return result;
}

/*!
 * \param stream input stream
 * \param offset current file offset
 * \param finishX max. image width 0 - doesn't matter
 * \param finishY max. image height 0 - doesn't matter
 * \param background background image
 * \return loaded frame.
 */
GIFrame Rangers::loadGIFrame(std::istream& stream, bool animation, GIFrame *background, uint32_t offset, int startX, int startY, int finishX, int finishY)
{
    GIFrameHeader image;
    GILayerHeader *layers;

    stream.read((char*)&image, 64);

    if (image.signature != 0x00006967)
        return GIFrame();

    layers = new GILayerHeader[image.layerCount];

    //size_t delta = 64;

    for (int i = 0; i < image.layerCount; i++)
    {
        stream.seekg(offset + 64 + i * 32, ios_base::beg);
        stream.read((char*)&layers[i], 32);

        if (animation)
        {
            layers[i].startX -= startX;
            layers[i].startY -= startY;
            layers[i].finishX -= startX;
            layers[i].finishY -= startY;
        }
        else
        {
            layers[i].startX -= image.startX;
            layers[i].startY -= image.startY;
            layers[i].finishX -= image.startX;
            layers[i].finishY -= image.startY;
        }
    }

    if (animation)
    {
        if (finishX)
            image.finishX = finishX;

        if (finishY)
            image.finishY = finishY;

        image.startX -= startX;
        image.startY -= startY;
        image.finishX -= startX;
        image.finishY -= startY;
    }
    else
    {
        image.finishX -= image.startX;
        image.finishY -= image.startY;
        image.startX = 0;
        image.startY = 0;
    }

    GIFrame resultFrame = loadGIImageData(image, layers, stream, background, offset);

    delete[] layers;

    return resultFrame;
}
