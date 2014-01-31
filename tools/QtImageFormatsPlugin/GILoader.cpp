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

#include "Types.h"
#include <QIODevice>
#include <QImage>
#include <QDebug>

namespace Rangers
{
void blitBGRA(QImage *image, int x, int y, int w, int h, char *graphbuf)
{
    for (int i = y; i < y + h; i++)
    {
        QRgb *line = (QRgb*)image->scanLine(i);
        for (int j = x; j < x + w; j++)
            line[j] = ((QRgb *)graphbuf)[(i - y) * w + (j - x)];
    }
}

void blitRGBAI(QImage *image, int x, int y, int w, int h, char *graphbuf, char *pal)
{
    for (int i = y; i < y + h; i++)
    {
        QRgb *line = (QRgb*)image->scanLine(i);
        for (int j = x; j < x + w; j++)
        {
            uint32_t rgba = ((uint32_t *)pal)[((uint8_t*)graphbuf)[(i - y) * w + (j - x)]];
            line[j] = (rgba & 0xFF000000) | ((rgba >> 16) & 0xFF) | (((rgba >> 8) & 0xFF) << 8) | ((rgba & 0xFF) << 16);
        }
    }
}

void blitB5G6R5(QImage *image, int x, int y, int w, int h, char *graphbuf)
{
    for (int i = y; i < y + h; i++)
    {
        uint16_t *line = (uint16_t *)image->scanLine(i);
        for (int j = x; j < x + w; j++)
            line[j] = ((uint16_t *)graphbuf)[(i - y) * w + (j - x)];
    }
}

void drawB5G6R5ToB5G6R5(QImage *image, int x, int y, char* graphbuf)
{
    int size, cnt;

    size = *(uint32_t *)graphbuf;
    graphbuf += 16;

    int lineNum = y;
    unsigned char *line = image->scanLine(lineNum) + x * 2;

    while (size > 0)
    {
        unsigned char byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            lineNum++;
            line = image->scanLine(lineNum) + x * 2;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt * 2;

            do
            {
                *((uint16_t *)line) = *((uint16_t *)graphbuf);
                line += 2;
                graphbuf += 2;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            line += byte * 2;
        }

        size--;
    }
}

void drawB5G6R5ToBGRA(QImage *image, int x, int y, char* graphbuf)
{
    int size, cnt;

    size = *(uint32_t *)graphbuf;
    graphbuf += 16;

    int lineNum = y;
    unsigned char *line = image->scanLine(lineNum) + x * 4;

    while (size > 0)
    {
        unsigned char byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            lineNum++;
            line = image->scanLine(lineNum) + x * 4;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt * 2;

            do
            {
                uint16_t color = *((uint16_t *)graphbuf);
                *((uint32_t *)line) = (0xff000000) | (((color >> 11) & 0x1f) << 19) | (((color >> 5) & 0x3f) << 10) | (((color) & 0x1f) << 3);
                line += 4;
                graphbuf += 2;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            line += byte * 4;
        }

        size--;
    }
}

void drawA6ToBGRA(QImage *image, int x, int y, char* graphbuf)
{
    int size, cnt;

    size = *(uint32_t *)graphbuf;
    graphbuf += 16;

    int lineNum = y;
    unsigned char *line = image->scanLine(lineNum) + x * 4 + 3;

    while (size > 0)
    {
        unsigned char byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            lineNum++;
            line = image->scanLine(lineNum) + x * 4 + 3;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            do
            {
                *(line) = 4 * (63 - * ((uint8_t*)graphbuf));
                line += 4;
                graphbuf += 1;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            line += byte * 4;
        }

        size--;
    }
}

void drawAIToBGRA(QImage *image, int x, int y, char* graphbuf)
{
    int size, cnt;
    uint32_t pal[256];

    size = *(uint32_t *)graphbuf;
    int palsize = *((uint8_t*)graphbuf + 12);

    if (palsize == 0)
        palsize = 256;

    graphbuf += 16;

    for (int i = 0; i < palsize; i++)
    {
        uint16_t color = *(uint16_t *)graphbuf;
        pal[i] = (((color >> 11) & 0x1f) << 19) | (((color >> 5) & 0x3f) << 10) | (((color) & 0x1f) << 3);
        uint8_t alpha = 4 * (63 - * (graphbuf + 2));
        pal[i] |= alpha << 24;
        graphbuf += 4;
    }

    int lineNum = y;
    unsigned char *line = image->scanLine(lineNum) + x * 4;

    while (size > 0)
    {
        unsigned char byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            lineNum++;
            line = image->scanLine(lineNum) + x * 4;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            do
            {
                *((uint32_t*)line) = pal[*((uint8_t*)graphbuf)];
                line += 4;
                graphbuf += 1;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            line += byte * 4;
        }

        size--;
    }
}

void drawBGRIToBGRA(QImage *image, int x, int y, char* graphbuf)
{
    int size, cnt;
    uint32_t pal[256];

    size = *(uint32_t *)graphbuf;
    int palsize = *((uint8_t*)graphbuf + 12);

    if (palsize == 0)
        palsize = 256;

    graphbuf += 16;

    for (int i = 0; i < palsize; i++)
    {
        uint16_t color = *(uint16_t *)graphbuf;
        pal[i] = (0xff000000) | (((color >> 11) & 0x1f) << 19) | (((color >> 5) & 0x3f) << 10) | (((color) & 0x1f) << 3);
        graphbuf += 2;
    }

    int lineNum = y;
    unsigned char *line = image->scanLine(lineNum) + x * 4;

    while (size > 0)
    {
        unsigned char byte = *graphbuf;
        graphbuf++;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            lineNum++;
            line = image->scanLine(lineNum) + x * 4;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            do
            {
                *((uint32_t*)line) = pal[*((uint8_t*)graphbuf)];
                line += 4;
                graphbuf += 1;
                cnt--;
            }
            while (cnt > 0);
        }
        else if (byte < 0x80)
        {
            //shift to right
            line += byte * 4;
        }

        size--;
    }
}

void drawF5ToBGRA(QImage *image, int x, int y, char* graphbuf)
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

    unsigned char * bufdesrow = image->bits() + (image->width() * line + x) * 4;

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
                bufdesrow = image->bits() + (image->width() * line + x) * 4 + channel;
            }
            else
            {
                channel = 0;
                line++;
                bufdesrow = image->bits() + (image->width() * line + x) * 4;
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

bool loadFrameType0(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset)
{
    if (header.type != 0)
        return false;

    char *layerData = 0;

    if (header.layerCount && layers[0].size)
    {
        layerData = new char[layers[0].size];
        device->seek(offset + layers[0].seek);
        device->read(layerData, layers[0].size);
    }

    if ((header.aBitmask == 0xFF000000) && (header.rBitmask == 0xFF0000) && (header.gBitmask == 0xFF00) && (header.bBitmask == 0xFF))
    {
        if (layerData)
            blitBGRA(image, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), layerData);
    }
    else if ((header.rBitmask == 0xF800) && (header.gBitmask == 0x7E0) && (header.bBitmask == 0x1F))
    {
        if (layerData)
        {
            blitB5G6R5(image, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), layerData);
        }
    }

    delete[] layerData;

    return true;
}

bool loadFrameType1(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset)
{
    if (header.type != 1)
        return false;

    char *layerData = 0;

    if (header.layerCount && layers[0].size)
    {
        layerData = new char[layers[0].size];
        device->seek(offset + layers[0].seek);
        device->read(layerData, layers[0].size);
    }

    if (layerData)
        drawB5G6R5ToB5G6R5(image, layers[0].startX, layers[0].startY, layerData);

    delete[] layerData;

    return true;
}

bool loadFrameType2(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset)
{
    if (header.type != 2)
        return false;

    char *layerData0 = 0;
    char *layerData1 = 0;
    char *layerData2 = 0;

    if (header.layerCount && layers[0].size)
    {
        layerData0 = new char[layers[0].size];
        device->seek(offset + layers[0].seek);
        device->read(layerData0, layers[0].size);
    }
    if ((header.layerCount > 1) && layers[1].size)
    {
        layerData1 = new char[layers[1].size];
        device->seek(offset + layers[1].seek);
        device->read(layerData1, layers[1].size);
    }
    if ((header.layerCount > 2) && layers[2].size)
    {
        layerData2 = new char[layers[2].size];
        device->seek(offset + layers[2].seek);
        device->read(layerData2, layers[2].size);
    }

    if (layerData0)
        drawB5G6R5ToBGRA(image, layers[0].startX, layers[0].startY, layerData0);

    if (layerData1)
        drawB5G6R5ToBGRA(image, layers[1].startX, layers[1].startY, layerData1);

    if (layerData2)
        drawA6ToBGRA(image, layers[2].startX, layers[2].startY, layerData2);

    delete[] layerData0;
    delete[] layerData1;
    delete[] layerData2;

    return true;
}

bool loadFrameType3(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset)
{
    if (header.type != 3)
        return false;

    char *layerData0 = 0;
    char *layerData1 = 0;

    if (header.layerCount && layers[0].size)
    {
        layerData0 = new char[layers[0].size];
        device->seek(offset + layers[0].seek);
        device->read(layerData0, layers[0].size);
    }
    if ((header.layerCount > 1) && layers[1].size)
    {
        layerData1 = new char[layers[1].size];
        device->seek(offset + layers[1].seek);
        device->read(layerData1, layers[1].size);
    }

    if (layerData0)
        drawBGRIToBGRA(image, layers[0].startX, layers[0].startY, layerData0);

    if (layerData1)
        drawAIToBGRA(image, layers[1].startX, layers[1].startY, layerData1);

    delete[] layerData0;
    delete[] layerData1;

    return true;
}

bool loadFrameType4(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset)
{
    if (header.type != 4)
        return false;

    char *layerData = 0;
    char *palData = 0;

    if (header.layerCount && layers[0].size)
    {
        layerData = new char[layers[0].size];
        device->seek(offset + layers[0].seek);
        device->read(layerData, layers[0].size);
    }
    if ((header.layerCount > 1) && layers[1].size)
    {
        palData = new char[layers[1].size];
        device->seek(offset + layers[1].seek);
        device->read(palData, layers[1].size);
    }

    if (layerData && palData)
        blitRGBAI(image, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), layerData, palData);

    delete[] layerData;
    delete[] palData;

    return true;
}

bool loadFrameType5(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset, QImage *background)
{
    if (header.type != 5)
        return false;

    char *layerData = 0;

    if (header.layerCount && layers[0].size)
    {
        layerData = new char[layers[0].size];
        device->seek(offset + layers[0].seek);
        device->read(layerData, layers[0].size);
    }

    if (background && (image != background) && (!background->isNull()))
    {
        *image = *background;
    }

    if (layerData)
        drawF5ToBGRA(image, layers[0].startX, layers[0].startY, layerData);

    delete[] layerData;

    return true;
}

bool loadGIImageData(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset, QImage *background)
{
    switch (header.type)
    {
    case 0:
        return loadFrameType0(device, image, header, layers, offset);
        break;

    case 1:
        return loadFrameType1(device, image, header, layers, offset);
        break;

    case 2:
        return loadFrameType2(device, image, header, layers, offset);
        break;

    case 3:
        return loadFrameType3(device, image, header, layers, offset);
        break;

    case 4:
        return loadFrameType4(device, image, header, layers, offset);
        break;
    case 5:
        return loadFrameType5(device, image, header, layers, offset, background);
        break;

    default:
        qCritical() << "Unknown GI frame type: " << header.type << endl;
        return false;
        break;
    }
}

bool loadGIFrame(QIODevice *device, QImage *image, uint32_t offset, QImage *background, const QPoint& start)
{
    GIFrameHeader header;

    device->seek(offset);
    device->read((char*)&header, sizeof(GIFrameHeader));

    if (header.signature != 0x00006967)
        return false;

    GILayerHeader *layers = new GILayerHeader[header.layerCount];

    for (int i = 0; i < header.layerCount; i++)
    {
        device->read((char*)&layers[i], sizeof(GILayerHeader));

        layers[i].startX -= start.x();
        layers[i].startY -= start.y();
        layers[i].finishX -= start.x();
        layers[i].finishY -= start.y();
    }

    QImage::Format format;

    switch (header.type)
    {
    case 1:
        format = QImage::Format_RGB16;
        break;
    case 0:
        if ((header.aBitmask == 0xFF000000) && (header.rBitmask == 0xFF0000) && (header.gBitmask == 0xFF00) && (header.bBitmask == 0xFF))
            format = QImage::Format_ARGB32;
        else if ((header.rBitmask == 0xF800) && (header.gBitmask == 0x7E0) && (header.bBitmask == 0x1F))
            format = QImage::Format_RGB16;
        else
            format = QImage::Format_Invalid;
        break;
    case 3:
        format = QImage::Format_ARGB32_Premultiplied;
        break;
    case 2:
    case 4:
    case 5:
        format = QImage::Format_ARGB32;
        break;
    default:
        format = QImage::Format_Invalid;
        break;
    }

    int width = header.finishX - header.startX;
    int height = header.finishY - header.startY;

    if (image->isNull())
    {
        *image = QImage(width, height, format);
        image->fill(0);
        for (int i = 0; i < header.layerCount; i++)
        {
            layers[i].startX -= header.startX;
            layers[i].startY -= header.startY;
            layers[i].finishX -= header.startX;
            layers[i].finishY -= header.startY;
        }
    }
    else if (image->format() != format)
    {
        *image = image->convertToFormat(format);
    }

    bool result = loadGIImageData(device, image, header, layers, offset, background);

    delete[] layers;

    return result;
}
}
