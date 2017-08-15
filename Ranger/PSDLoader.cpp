/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2017 Kosyak <ObKo@mail.ru>

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

#include <OpenSR/libRangerQt.h>

#include <QIODevice>
#include <QImage>
#include <QDebug>

namespace OpenSR
{
namespace {
    static const int PSD_HEADER_SIZE = 26;
    static const int PSD_COLOR_MAP_SIZE = 256;
}

static inline uint32_t psdReadUint32(QIODevice *dev)
{
    uint8_t data[4];
    dev->read((char *)data, 4);
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]);
}
static inline uint16_t psdReadUint16(QIODevice *dev)
{
    uint8_t data[2];
    dev->read((char *)data, 2);
    return (data[0] << 8) | (data[1]);
}

static PSDHeader psdData2Header(const uint8_t* data)
{
    PSDHeader h;
    const uint8_t *p = data;
    h.signature = *((uint32_t*)p); p += 4;
    h.version = (p[0] << 8) | (p[1]); p += 2;
    p += 6;
    h.numChannels = (p[0] << 8) | (p[1]); p += 2;
    h.height = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3]); p += 4;
    h.width = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3]); p += 4;
    h.depth = (p[0] << 8) | (p[1]); p += 2;
    h.colorMode = (p[0] << 8) | (p[1]); p += 2;

    return h;
}

static void psdUnpackRLE(QIODevice *dev, uint16_t n, uint8_t *out, uint8_t stride)
{
    uint16_t count = n;
    uint8_t *outp = out;

    while (count > 0)
    {
        uint8_t h;
        dev->read((char *)&h, 1);
        count--;

        if (h == 0x80)
            continue;
        else if (h > 0x80)
        {
            uint8_t cnt = 256 - h + 1;
            uint8_t b;
            dev->read((char*)&b, 1);
            count--;
            for (int i = 0; i < cnt; i++)
            {
                *outp = b;
                outp += stride;
            }
        }
        else
        {
            uint8_t cnt = h + 1;
            for (int i = 0; i < cnt; i++)
            {
                uint8_t b;
                dev->read((char*)&b, 1);
                count--;
                *outp = b;
                outp += stride;
            }
        }
    }
}

static void psdReadStride(QIODevice *dev, int n, uint8_t *out, uint8_t stride)
{
    uint8_t *outp = out;
    for (int i = 0; i < n; i++)
    {
        dev->read((char*)outp, 1);
        outp += stride;
    }
}

static void psdReadIndexed(QImage& image, QIODevice *dev, const PSDHeader& header,
                           uint8_t *pallete, bool rle)
{
    bool outIndexed = image.format() == QImage::Format_Indexed8;
    uint8_t outStride = image.depth() / 8;
    QVector<QRgb> colorTable;
    if (pallete)
    {
        colorTable = QVector<QRgb>(PSD_COLOR_MAP_SIZE);
        for (int i = 0; i < PSD_COLOR_MAP_SIZE; i++)
        {
            uint8_t r = pallete[0 * PSD_COLOR_MAP_SIZE + i];
            uint8_t g = pallete[1 * PSD_COLOR_MAP_SIZE + i];
            uint8_t b = pallete[2 * PSD_COLOR_MAP_SIZE + i];
            colorTable[i] = qRgb(r, g, b);
        }
        if (image.format() == QImage::Format_Indexed8)
            image.setColorTable(colorTable);
    }
    uint16_t *rleSizes = nullptr;
    if (rle)
    {
        rleSizes = new uint16_t[header.height * header.numChannels];
        for (int i = 0; i < header.height * header.numChannels; i++)
            rleSizes[i] = psdReadUint16(dev);
    }
    for (int c = 0; c < header.numChannels; c++)
    {
        if ((c > 1) && outIndexed)
            break;

        for (int i = 0; i < header.height; i++)
        {
            uint8_t *out = (uint8_t*)image.scanLine(i);
            if (outIndexed)
            {
                if (rle)
                    psdUnpackRLE(dev, rleSizes[c * header.height + i], out, 1);
                else
                    psdReadStride(dev, header.width, out, 1);
            }
            else
            {
                if (c == 0)
                {
                    if (rle)
                        psdUnpackRLE(dev, rleSizes[c * header.height + i], out, outStride);
                    else
                        psdReadStride(dev, header.width, out, outStride);

                    for (int j = 0; j < header.width; j++)
                    {
                        uint8_t c = out[j * outStride];
                        image.setPixel(j, i, colorTable.at(c));
                    }
                }
                else if (c == 1)
                {
                    if (rle)
                        psdUnpackRLE(dev, rleSizes[c * header.height + i], out + 3, outStride);
                    else
                        psdReadStride(dev, header.width, out + 3, outStride);
                }
            }
        }
    }
    if (rleSizes)
        delete[] rleSizes;
}

static void psdReadByte(QImage& image, QIODevice *dev, const PSDHeader& header,
                        bool rle)
{
    uint16_t *rleSizes = nullptr;
    if (rle)
    {
        rleSizes = new uint16_t[header.height * header.numChannels];
        for (int i = 0; i < header.height * header.numChannels; i++)
            rleSizes[i] = psdReadUint16(dev);
    }
    for (int i = 0; i < header.height; i++)
    {
        uint8_t *out = (uint8_t*)image.scanLine(i);
        if (rle)
            psdUnpackRLE(dev, rleSizes[header.height + i], out, 1);
        else
            psdReadStride(dev, header.width, out, 1);
    }
    if (rleSizes)
        delete rleSizes;
}

static void psdReadRGB(QImage& image, QIODevice *dev, const PSDHeader& header,
                       bool rle)
{
    int outNumCh = image.hasAlphaChannel() ? 4 : 3;
    uint16_t *rleSizes = nullptr;
    if (rle)
    {
        rleSizes = new uint16_t[header.height * header.numChannels];
        for (int i = 0; i < header.height * header.numChannels; i++)
            rleSizes[i] = psdReadUint16(dev);
    }
    for (int c = 0; c < outNumCh; c++)
    {
        for (int i = 0; i < header.height; i++)
        {
            uint8_t *out = (uint8_t*)image.scanLine(i);
            if (rle)
                psdUnpackRLE(dev, rleSizes[c * header.height + i], out + c, outNumCh);
            else
                psdReadStride(dev, header.width, out + c, outNumCh);
        }
    }
    if (rleSizes)
        delete rleSizes;
}

bool checkPSDHeader(QIODevice *dev)
{
    quint32 sig;
    qint64 size = dev->peek((char*)&sig, sizeof(quint32));

    if (size != sizeof(quint32) || sig != PSD_SIGNATURE)
        return false;

    return true;
}

PSDHeader peekPSDHeader(QIODevice *dev)
{
    uint8_t data[PSD_HEADER_SIZE];
    dev->peek((char *)data, PSD_HEADER_SIZE);
    return psdData2Header(data);
}

PSDHeader readPSDHeader(QIODevice *dev)
{
    uint8_t data[PSD_HEADER_SIZE];
    dev->read((char *)data, PSD_HEADER_SIZE);
    return psdData2Header(data);
}

QImage::Format getPSDFormat(const PSDHeader& header)
{
    switch (header.colorMode)
    {
    case 0:
        if (header.numChannels == 1)
            return QImage::Format_Mono;
        else
            return QImage::Format_Invalid;
    case 1:
        if (header.depth == 8)
            return QImage::Format_Grayscale8;
        else
            return QImage::Format_Invalid;
    case 2:
        if (header.numChannels == 1)
            return QImage::Format_Indexed8;
        else if (header.numChannels == 2)
            return QImage::Format_RGBA8888;
        else
            return QImage::Format_Invalid;
    case 3:
        if (header.numChannels == 4)
            return QImage::Format_RGBA8888;
        else if (header.numChannels == 3)
            return QImage::Format_RGB888;
        else
            return QImage::Format_Invalid;
    default:
        return QImage::Format_Invalid;
    }
}

QImage loadPSDFrame(QIODevice *dev)
{
    PSDHeader header = readPSDHeader(dev);
    QImage::Format imageFormat = getPSDFormat(header);

    if (imageFormat == QImage::Format_Invalid)
        return QImage();

    uint8_t *pallete = nullptr;

    uint32_t colorSectionSize = psdReadUint32(dev);

    if ((header.colorMode == 2) && colorSectionSize)
    {
        pallete = new uint8_t[colorSectionSize];
        dev->read((char*)pallete, colorSectionSize);
    }
    else
        dev->seek(dev->pos() + colorSectionSize);

    uint32_t resourceSectionSize = psdReadUint32(dev);
    dev->seek(dev->pos() + resourceSectionSize);
    uint32_t layerMaskSectionSize = psdReadUint32(dev);
    dev->seek(dev->pos() + layerMaskSectionSize);
    uint16_t dataCompression = psdReadUint16(dev);

    if ((dataCompression != 0) && (dataCompression != 1))
        return QImage();

    bool rle = (dataCompression == 1);

    QImage image(header.width, header.height, imageFormat);

    switch (header.colorMode)
    {
    case 0:
    case 1:
        psdReadByte(image, dev, header, rle);
        break;
    case 2:
        psdReadIndexed(image, dev, header, pallete, rle);
        break;
    case 3:
        psdReadRGB(image, dev, header, rle);
        break;
    default:
        break;
    }

    if (pallete)
        delete pallete;

    return image;
}
}
