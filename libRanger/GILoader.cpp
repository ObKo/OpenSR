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

#define LIBGI_LIBRARY
#include "libRanger.h"
#include <cstring>
#include <fstream>

using namespace Rangers;
using namespace std;

#define B_565(word) ((word & 0x1f) * 8)
#define G_565(word) ((word>>3) & 0xfc)
#define R_565(word) ((word>>8) & 0xf8)

uint32_t R5G6B5ToR8G8B8(uint16_t color)
{
    uint32_t result = 0;
    result |= (color & 0x1F) << 11;
    result |= ((color >> 5) & 0x3F) << 18;
    result |= ((color >> 11) & 0x1F) << 27;
    return result;
}

uint16_t R5G6B5(char b, char g, char r)
{
    uint16_t result = 0;
    result |= ((r >> 3) & 0x1F) << 11;
    result |= ((g >> 2) & 0x3F) << 5;
    result |= ((b >> 3) & 0x1F);
    return result;
}

void DrawA6ToRGBA(unsigned char * bufdes, int bufdesll, unsigned char * graphbuf)
{
    int size, cnt;
    unsigned char byte;

    size = *(uint32_t *)graphbuf;
    graphbuf += 16;

    unsigned char * bufdesrow = bufdes;

    while (size > 0)
    {
        byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            bufdes += bufdesll;
            bufdesrow = bufdes;
        }
        else
            if (byte > 0x80)
            {
                //pixels found
                cnt = byte & 0x7f;
                size -= cnt;

                do
                {
                    *(bufdesrow) = 4 * (63 - *graphbuf);
                    bufdesrow += 4;
                    graphbuf++;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
                if (byte < 0x80)
                {
                    //shift to right
                    bufdesrow += byte * 4;
                }

        size--;
    }
}

void DrawR5G6B5ToRGBA(unsigned char * bufdes, int bufdesll, unsigned char * graphbuf)
{
    int size, cnt;
    uint16_t j;
    unsigned char byte;

    size = *(uint32_t *)graphbuf;
    graphbuf += 16;

    unsigned char * bufdesrow = bufdes;

    while (size > 0)
    {
        byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            bufdes += bufdesll;
            bufdesrow = bufdes;
        }
        else
            if (byte > 0x80)
            {
                //pixels found
                cnt = byte & 0x7f;
                size -= cnt * 2;

                do
                {
                    j = *(uint16_t *)graphbuf;
                    *(bufdesrow + 0) = 0xff;
                    *(bufdesrow + 1) = B_565(j);
                    *(bufdesrow + 2) = G_565(j);
                    *(bufdesrow + 3) = R_565(j);
                    bufdesrow += 4;
                    graphbuf += 2;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
                if (byte < 0x80)
                {
                    //shift to right
                    bufdesrow += byte * 4;
                }

        size--;
    }
}

void DrawAIToRGBA(unsigned char * bufdes, int bufdesll, unsigned char * graphbuf)
{
    int size, cnt, palsize, i;
    uint8_t byte, r, g, b, alpha;
    uint32_t Pal[256];
    uint16_t j;

    size = *(uint32_t *)graphbuf;
    palsize = *(graphbuf + 12);

    if (palsize == 0)
        palsize = 256;

    graphbuf += 16;

    for (i = 0; i < palsize; i++)
    {
        j = *(uint16_t *)graphbuf;
        alpha = 4 * (63 - *(graphbuf + 2));

        if (alpha > 0)
        {
            b = B_565(j) * 0xff / alpha;
            g = G_565(j) * 0xff / alpha;
            r = R_565(j) * 0xff / alpha;
            Pal[i] = b << 8 | g << 16 | r << 24 | alpha;
        }
        else
        {
            Pal[i] = 0;
        }

        graphbuf += 4;
    }

    unsigned char * bufdesrow = bufdes;

    while (size > 0)
    {
        byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            bufdes += bufdesll;
            bufdesrow = bufdes;
        }
        else
            if (byte > 0x80)
            {
                //pixels found
                cnt = byte & 0x7f;
                size -= cnt;

                do
                {
                    i = *graphbuf; //index of color in palette
                    *(uint32_t *)bufdesrow = Pal[i];
                    bufdesrow += 4;
                    graphbuf++;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
                if (byte < 0x80)
                {
                    //shift to right
                    bufdesrow += byte * 4;
                }

        size--;
    }
}

void DrawRGBIToRGBA(unsigned char * bufdes, int bufdesll, unsigned char * graphbuf)
{
    int size, cnt, palsize, i;
    uint32_t Pal[256];
    uint16_t j;
    uint8_t byte;

    size = *(uint32_t *)graphbuf;
    palsize = *(graphbuf + 12);

    if (palsize == 0)
        palsize = 256;

    graphbuf += 16;

    for (i = 0; i < palsize; i++)
    {
        j = *(uint16_t *)graphbuf;
        Pal[i] = R5G6B5ToR8G8B8(j) | 0xff;
        graphbuf += 2;
    }

    unsigned char * bufdesrow = bufdes;

    while (size > 0)
    {
        byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            bufdes += bufdesll;
            bufdesrow = bufdes;
        }
        else
            if (byte > 0x80)
            {
                //pixels found
                cnt = byte & 0x7f;
                size -= cnt;

                do
                {
                    i = *graphbuf; //index of color in palette
                    *(uint32_t *)bufdesrow = Pal[i];
                    bufdesrow += 4;
                    graphbuf++;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
                if (byte < 0x80)
                {
                    //shift to right
                    bufdesrow += byte * 4;
                }

        size--;
    }
}

void FillARGBToRGBA(unsigned char *bufdes, int destwidth, int x, int y, int w, int h, unsigned char *graphbuf)
{
    uint32_t j;
    unsigned char *row = bufdes + (destwidth * y + x) * 4;

    for (int i = 0; i < w * h; i++)
    {
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4) = *((graphbuf) + i * 4 + 3);
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4 + 1) = *((graphbuf) + i * 4);
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4 + 2) = *((graphbuf) + i * 4 + 1);
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4 + 3) = *((graphbuf) + i * 4 + 2);
    }
}

void FillR5G6B5ToRGBA(unsigned char *bufdes, int destwidth, int x, int y, int w, int h, unsigned char *graphbuf)
{
    uint32_t j;
    uint32_t *dest = (uint32_t *)bufdes;
    uint16_t *src = (uint16_t *)graphbuf;
    unsigned char *row = bufdes + (destwidth * y + x) * 4;

    for (int i = 0; i < w * h; i++)
    {
        dest[destwidth * (i/w + y) + x + i % w] = R5G6B5ToR8G8B8(src[i]);
        dest[destwidth * (i/w + y) + x + i % w] |= 0xff;
    }
}

void FillARGBItoRGBA(unsigned char *bufdes, unsigned char *bufsrc, unsigned char *pal, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        int p = (height - y - 1) * (width);
        unsigned char *bits = (unsigned char *) & bufsrc[p];

        for (int x = 0; x < width; x++)
        {
            bufdes[0] = pal[bits[0] * 4 + 3];
            bufdes[1] = pal[bits[0] * 4 + 2];
            bufdes[2] = pal[bits[0] * 4 + 1];
            bufdes[3] = pal[bits[0] * 4 + 0];
            bufdes += 4;
            bits += 1;
        }
    }
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadGIImageData(const GIFrameHeader& image)
{
    GIFrame resultFrame;

    switch (image.type)
    {

    case 0:
        resultFrame = loadFrameType0(image);
        break;

    case 1:
        resultFrame = loadFrameType1(image);
        break;

    case 2:
        resultFrame = loadFrameType2(image);
        break;

    case 3:
        resultFrame = loadFrameType3(image);
        break;

    case 4:
        resultFrame = loadFrameType4(image);
        break;
        //case 5:
        //resultFrame = loadFrameType5(image);
        //    break;

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
GIFrame Rangers::loadFrameType2(const GIFrameHeader& image)
{
    GIFrame result;

    if (image.type != 2)
        return result;

    int width = image.finishX;

    int height = image.finishY;

    result.width = width;

    result.height = height;

    unsigned char *rgba = new unsigned char[width * height * 4];

    memset(rgba, 0, width * height * 4);


    if (image.layers[0].size)
        DrawR5G6B5ToRGBA(rgba + (image.layers[0].startY * width + image.layers[0].startX)*4, width * 4, (unsigned char *)image.layers[0].data);

    if (image.layers[1].size)
        DrawR5G6B5ToRGBA(rgba + (image.layers[1].startY * width + image.layers[1].startX)*4,  width * 4, (unsigned char *)image.layers[1].data);

    if (image.layers[2].size)
        DrawA6ToRGBA(rgba + (image.layers[2].startY * width + image.layers[2].startX)*4,  width * 4, (unsigned char *)image.layers[2].data);

    result.data = rgba;

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType1(const GIFrameHeader& image)
{
    GIFrame result;

    if (image.type != 1)
        return result;

    int width = image.finishX;

    int height = image.finishY;

    result.width = width;

    result.height = height;

    unsigned char *rgba = new unsigned char[width * height * 4];

    memset(rgba, 0, width * height * 4);

    if (image.layers[0].size)
        DrawR5G6B5ToRGBA(rgba + (image.layers[0].startY * width + image.layers[0].startX)*4, width * 4, (unsigned char *)image.layers[0].data);

    result.data = rgba;

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType4(const GIFrameHeader& image)
{
    GIFrame result;

    if (image.type != 4)
        return result;

    int width = image.finishX;

    int height = image.finishY;

    result.width = width;

    result.height = height;

    unsigned char *rgba = new unsigned char[width * height * 4];

    memset(rgba, 0, width * height * 4);

    if (image.layers[0].size)
        FillARGBItoRGBA(rgba + (image.layers[0].startY * width + image.layers[0].startX)*4, (unsigned char *)image.layers[0].data, (unsigned char *)image.layers[1].data, width, height);

    result.data = rgba;

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType3(const GIFrameHeader& image)
{
    GIFrame result;

    if (image.type != 3)
        return result;

    int width = image.finishX;

    int height = image.finishY;

    result.width = width;

    result.height = height;

    unsigned char *rgba = new unsigned char[width * height * 4];

    memset(rgba, 0, width * height * 4);

    if (image.layers[0].size)
        DrawRGBIToRGBA(rgba + (image.layers[0].startY * width + image.layers[0].startX)*4, width * 4, (unsigned char *)image.layers[0].data);

    if (image.layers[1].size)
        DrawAIToRGBA(rgba + (image.layers[1].startY * width + image.layers[1].startX)*4, width * 4, (unsigned char *)image.layers[1].data);

    result.data = rgba;

    return result;
}

/*!
 * \param image input image data from GI file.
 * \return loaded frame.
 */
GIFrame Rangers::loadFrameType0(const GIFrameHeader& image)
{
    GIFrame result;

    if (image.type != 0)
        return result;

    int width = image.finishX;

    int height = image.finishY;

    result.width = width;

    result.height = height;

    if ((image.aBitmask == 0xFF000000) && (image.rBitmask == 0xFF0000) && (image.gBitmask == 0xFF00) && (image.bBitmask == 0xFF))
    {
        unsigned char *rgba = new unsigned char[width * height * 4];
        memset(rgba, 0, width * height * 4);

        if (image.layers[0].size)
            FillARGBToRGBA(rgba, width, image.layers[0].startX, image.layers[0].startY, (image.layers[0].finishX - image.layers[0].startX), (image.layers[0].finishY - image.layers[0].startY), (unsigned char *)image.layers[0].data);

        result.data = rgba;
    }
    else
        if ((image.rBitmask == 0xF800) && (image.gBitmask == 0x7E0) && (image.bBitmask == 0x1F))
        {
            unsigned char *rgba = new unsigned char[width * height * 4];
            memset(rgba, 0, width * height * 4);

            if (image.layers[0].size)
                FillR5G6B5ToRGBA(rgba, width, image.layers[0].startX, image.layers[0].startY, (image.layers[0].finishX - image.layers[0].startX), (image.layers[0].finishY - image.layers[0].startY), (unsigned char *)image.layers[0].data);

            result.data = rgba;
        }

    return result;
}

/*!
 * \param data GI data in memory
 * \param offset current file offset
 * \param finishX max. image width 0 - doesn't matter
 * \param finishY max. image height 0 - doesn't matter
 * \return loaded frame.
 */
GIFrame Rangers::loadGIFrame(const char *data, size_t &offset, int finishX, int finishY)
{
    const char *buffer = data + offset;
    GIFrameHeader image;

    memcpy((char*)&image, buffer, 64);
    image.layers = new GILayerHeader[image.layerCount];

    size_t delta = 64;

    if (finishX)
        image.finishX = finishX;

    if (finishY)
        image.finishY = finishY;

    for (int i = 0; i < image.layerCount; i++)
    {
        buffer = data + offset + 64 + i * 32;
        memcpy((char*)&image.layers[i], buffer, 32);
        image.layers[i].data = new unsigned char[image.layers[i].size];
        buffer = data + offset + image.layers[i].seek;
        memcpy((char*)image.layers[i].data, buffer, image.layers[i].size);
        delta += image.layers[i].size + 32;

        //image.layers[i].startX -= image.startX;
        //image.layers[i].startY -= image.startY;
        //image.layers[i].finishX -= image.startX;
        //image.layers[i].finishY -= image.startY;
    }

    offset += delta;

    GIFrame resultFrame = loadGIImageData(image);

    for (int i = 0; i < image.layerCount; i++)
    {
        delete image.layers[i].data;
    }

    delete[] image.layers;

    return resultFrame;
}

/*!
 * \param stream input stream
 * \param offset current file offset
 * \param finishX max. image width 0 - doesn't matter
 * \param finishY max. image height 0 - doesn't matter
 * \return loaded frame.
 */
GIFrame Rangers::loadGIFrame(std::istream& stream, size_t &offset, int finishX, int finishY)
{
    GIFrameHeader image;

    stream.read((char*)&image, 64);
    image.layers = new GILayerHeader[image.layerCount];

    size_t delta = 64;

    if (finishX)
        image.finishX = finishX;

    if (finishY)
        image.finishY = finishY;


    for (int i = 0; i < image.layerCount; i++)
    {
        stream.seekg(offset + 64 + i * 32, ios_base::beg);
        stream.read((char*)&image.layers[i], 32);
        image.layers[i].data = new unsigned char[image.layers[i].size];
        stream.seekg(offset + image.layers[i].seek, ios_base::beg);
        stream.read((char*)image.layers[i].data, image.layers[i].size);
        delta += image.layers[i].size + 32;

        //image.layers[i].startX -= image.startX;
        //image.layers[i].startY -= image.startY;
        //image.layers[i].finishX -= image.startX;
        //image.layers[i].finishY -= image.startY;
    }

    offset += delta;

    GIFrame resultFrame = loadGIImageData(image);

    for (int i = 0; i < image.layerCount; i++)
    {
        delete image.layers[i].data;
    }

    delete[] image.layers;

    return resultFrame;
}

/*!
 * \param stream input stream
 * \return loaded frame.
 */
GIFrame Rangers::loadGIFile(std::istream& stream)
{
    GIFrameHeader image;

    stream.read((char*)&image, 64);
    image.layers = new GILayerHeader[image.layerCount];

    for (int i = 0; i < image.layerCount; i++)
    {
        stream.seekg(64 + i * 32, ios_base::beg);
        stream.read((char*)&image.layers[i], 32);
        image.layers[i].data = new unsigned char[image.layers[i].size];
        stream.seekg(image.layers[i].seek, ios_base::beg);
        stream.read((char*)image.layers[i].data, image.layers[i].size);

        image.layers[i].startX -= image.startX;
        image.layers[i].startY -= image.startY;
        image.layers[i].finishX -= image.startX;
        image.layers[i].finishY -= image.startY;
    }

    image.finishY = image.finishY - image.startY;

    image.finishX = image.finishX - image.startX;
    image.startY = 0;
    image.startY = 0;

    GIFrame resultFrame = loadGIImageData(image);

    for (int i = 0; i < image.layerCount; i++)
    {
        delete image.layers[i].data;
    }

    delete[] image.layers;

    return resultFrame;
}

/*!
 * \param data input data
 * \return loaded frame.
 */
GIFrame Rangers::loadGIFile(const char *data)
{
    GIFrameHeader image;
    image = *((GIFrameHeader *)data);
    //if(image.signature !=)
    image.layers = new GILayerHeader[image.layerCount];
    const char *ptr;

    for (int i = 0; i < image.layerCount; i++)
    {
        ptr = data + 64 + i * 32;
        image.layers[i] = *((GILayerHeader *)ptr);
        image.layers[i].data = new unsigned char[image.layers[i].size];
        ptr = data + image.layers[i].seek;
        memcpy(image.layers[i].data, ptr, image.layers[i].size);

        image.layers[i].startX -= image.startX;
        image.layers[i].startY -= image.startY;
        image.layers[i].finishX -= image.startX;
        image.layers[i].finishY -= image.startY;
    }

    image.finishY = image.finishY - image.startY;

    image.finishX = image.finishX - image.startX;
    image.startY = 0;
    image.startY = 0;

    GIFrame resultFrame = loadGIImageData(image);

    for (int i = 0; i < image.layerCount; i++)
        delete image.layers[i].data;

    delete[] image.layers;

    return resultFrame;
}

