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

#include <OpenSR/libRangerQt.h>

#include <QIODevice>
#include <QImage>
#include <QDebug>

namespace OpenSR
{
bool checkGIHeader(QIODevice *dev)
{
    quint32 sig;
    qint64 size = dev->peek((char*)&sig, sizeof(quint32));

    if (size != sizeof(quint32) || sig != GI_FRAME_SIGNATURE)
        return false;

    return true;
}

void blitARGB(QImage &result, int x, int y, int w, int h, QIODevice *dev)
{
    for (int i = y; i < y + h; i++)
        dev->read((char *)((QRgb*)result.scanLine(i)) + x, w * 4);
}

void blitR5G6B5(QImage &result, int x, int y, int w, int h, QIODevice *dev)
{
    for (int i = y; i < y + h; i++)
        dev->read((char *)((quint16*)result.scanLine(i)) + x, w * 2);
}

void blitARGBI(QImage &result, int x, int y, int w, int h, QIODevice *dev)
{
    for (int i = y; i < y + h; i++)
        dev->read((char *)(result.scanLine(i) + x), w);
}

void drawR5G6B5(QImage &result, int x, int y, QIODevice *dev)
{
    int size, cnt, line = y;
    int32_t tmp[4];

    dev->read((char*)tmp, 16);
    size = tmp[0];

    uchar *row = result.scanLine(line) + x * (result.depth() / 8);

    while (size > 0)
    {
        uint8_t byte = 0;
        dev->read((char*)&byte, 1);
        size--;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = result.scanLine(line) + x * (result.depth() / 8);
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt * 2;

            if (result.format() == QImage::Format_RGB16)
            {
                dev->read((char *)row, cnt * 2);
                row += cnt * 2;
            }
            else if (result.format() == QImage::Format_ARGB32 || result.format() == QImage::Format_ARGB32_Premultiplied)
            {
                do
                {
                    quint16 color;
                    dev->read((char *)&color, 2);
                    *((uint32_t *)row) = (0xff000000) | (((color >> 11) & 0x1f) << 19) | (((color >> 5) & 0x3f) << 10) | (((color) & 0x1f) << 3);
                    row += 4;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
            {
                qWarning() << "Unsupported image format in drawR5G6B5()";
                return;
            }
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * (result.depth() / 8);
        }
    }
}

void drawRGBI(QImage &result, int x, int y, QIODevice *dev)
{
    int size, cnt, palsize, i, line = y;
    uint16_t pal[256];

    int32_t tmp[4];

    dev->read((char*)tmp, 16);

    size = tmp[0];
    palsize = ((uint8_t*)tmp)[12];

    if (palsize == 0)
        palsize = 256;

    dev->read((char*)pal, palsize * 2);

    uchar *row = result.scanLine(line) + x * (result.depth() / 8);

    while (size > 0)
    {
        uint8_t byte = 0;
        dev->read((char*)&byte, 1);
        size--;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = result.scanLine(line) + x * (result.depth() / 8);
        }
        else if (byte > 0x80)
        {
            cnt = byte & 0x7f;
            size -= cnt;

            if (result.format() == QImage::Format_RGB16)
            {
                do
                {
                    quint8 index;
                    dev->read((char *)&index, 1);
                    *((uint16_t *)row) = pal[index];
                    row += 2;
                    cnt--;
                }
                while (cnt > 0);
            }
            else if (result.format() == QImage::Format_ARGB32 || result.format() == QImage::Format_ARGB32_Premultiplied)
            {
                do
                {
                    quint8 index;
                    dev->read((char *)&index, 1);
                    *((uint32_t *)row) = (0xff000000) | (((pal[index] >> 11) & 0x1f) << 19) | (((pal[index] >> 5) & 0x3f) << 10) | (((pal[index]) & 0x1f) << 3);
                    row += 4;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
            {
                qWarning() << "Unsupported image format in drawRGBI()";
                return;
            }
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * (result.depth() / 8);
        }
    }
}

void drawAI(QImage &result, int x, int y, QIODevice *dev)
{
    int size, cnt, palsize, i, line = y;
    uint32_t pal[256 * 2];

    int32_t tmp[4];

    dev->read((char*)tmp, 16);

    size = tmp[0];
    palsize = ((uint8_t*)tmp)[12];

    if (palsize == 0)
        palsize = 256;

    for (int i = 0; i < palsize; i++)
    {
        uint16_t data[2];
        dev->read((char*)&data, 4);
        uint8_t alpha = 4 * (63 - * ((uint8_t*)&data[1]));
        pal[i] = (alpha << 24)
                 | (((data[0] >> 11) & 0x1f) << 19)
                 | (((data[0] >> 5) & 0x3f) << 10)
                 | (((data[0]) & 0x1f) << 3);
    }

    uchar *row = result.scanLine(line) + x * (result.depth() / 8);

    while (size > 0)
    {
        uint8_t byte = 0;
        dev->read((char*)&byte, 1);
        size--;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = result.scanLine(line) + x * (result.depth() / 8);
        }
        else if (byte > 0x80)
        {
            cnt = byte & 0x7f;
            size -= cnt;
            if (result.format() == QImage::Format_ARGB32_Premultiplied)
            {
                do
                {
                    quint8 index;
                    dev->read((char *)&index, 1);
                    *((uint32_t *)row) = pal[index];
                    row += 4;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
            {
                qWarning() << "Unsupported image format in drawAI()";
                return;
            }
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * (result.depth() / 8);
        }
    }
}

void drawA6(QImage &result, int x, int y, QIODevice *dev)
{
    int size, cnt, line = y;
    int32_t tmp[4];

    dev->read((char*)tmp, 16);
    size = tmp[0];

    uchar *row = result.scanLine(y) + x * (result.depth() / 8) + 3;

    while (size > 0)
    {
        uint8_t byte;
        dev->read((char*)&byte, 1);
        size--;

        if ((byte == 0) || (byte == 0x80))
        {
            //goto new scanline
            line++;
            row = result.scanLine(line) + x * (result.depth() / 8) + 3;
        }
        else if (byte > 0x80)
        {
            //pixels found
            cnt = byte & 0x7f;
            size -= cnt;

            if (result.format() == QImage::Format_ARGB32 || result.format() == QImage::Format_ARGB32_Premultiplied)
            {
                do
                {
                    quint8 alpha;
                    dev->read((char*)&alpha, 1);
                    *row = 4 * (63 - alpha);
                    row += 4;
                    cnt--;
                }
                while (cnt > 0);
            }
            else
            {
                qWarning() << "Unsupported image format in drawA6()";
                return;
            }
        }
        else if (byte < 0x80)
        {
            //shift to right
            row += byte * (result.depth() / 8);
        }
    }
}

void decodeGAIDeltaFrame(QImage &dest, int x, int y, QIODevice *dev)
{
    int i, cnt, cnt2, shlc;
    int shlca[4];
    unsigned char byte;
    int line = y;
    uint8_t tmp[16];

    dev->read((char*)tmp, 16);

    byte = tmp[12];
    shlc = shlca[0] = 8 - (byte & 15);
    shlca[1] = 8 - (byte >> 4);
    byte = tmp[13];
    shlca[2] = 8 - (byte & 15);
    shlca[3] = 8 - (byte >> 4);

    quint32 offset = (((uint32_t *)tmp)[2]) << 2;
    dev->seek(dev->pos() + offset);

    unsigned char *bufdesrow = dest.scanLine(line) + x * (dest.depth() / 8);

    uint32_t channel = 0;
    while (true)
    {
        dev->read((char*)&byte, 1);

        if ((byte & 0x80) != 0)
        {
            cnt = (byte & 0xf) + 1;
            byte = (byte >> 4) & 7;
            if (byte == 0)
            {
                do
                {
                    cnt2 = (cnt > 8) ? 8 : cnt;
                    dev->read((char*)&byte, 1);
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
                    dev->read((char*)&byte, 1);
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
                    dev->read((char*)&byte, 1);
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
                    dev->read((char*)&byte, 1);
                    *bufdesrow = *bufdesrow + ((byte + 1) << shlc);
                    bufdesrow += 4;
                }
            }
            else if (byte == 4)
            {
                do
                {
                    cnt2 = (cnt > 8) ? 8 : cnt;
                    dev->read((char*)&byte, 1);
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
                    dev->read((char*)&byte, 1);
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
                    dev->read((char*)&byte, 1);
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
                    dev->read((char*)&byte, 1);
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
                bufdesrow = dest.scanLine(line) + x * (dest.depth() / 8) + channel;
            }
            else
            {
                channel = 0;
                line++;
                bufdesrow = dest.scanLine(line) + x * (dest.depth() / 8);
            }
            shlc = shlca[channel];
        }
        else if (byte == 0x03f)
        {
            quint16 shift;
            dev->read((char*)&shift, 2);
            bufdesrow += ((uint32_t)shift) << 2;
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

QImage loadFrameType0(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, qint64 offset)
{

    if (image.type != 0 || !image.layerCount || !layers)
        return QImage();

    int width = image.finishX;
    int height = image.finishY;

    QImage result;

    if ((image.aBitmask == 0xFF000000) && (image.rBitmask == 0xFF0000) && (image.gBitmask == 0xFF00) && (image.bBitmask == 0xFF))
    {
        result = QImage(width, height, QImage::Format_ARGB32);
        result.fill(0);

        if (layers[0].size)
        {
            dev->seek(offset + layers[0].seek);
            blitARGB(result, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), dev);
        }
    }
    else if ((image.rBitmask == 0xF800) && (image.gBitmask == 0x7E0) && (image.bBitmask == 0x1F))
    {
        result = QImage(width, height, QImage::Format_RGB16);
        result.fill(0);

        if (layers[0].size)
        {
            dev->seek(offset + layers[0].seek);
            blitR5G6B5(result, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), dev);
        }
    }
    return result;
}

QImage loadFrameType1(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, qint64 offset)
{
    if (image.type != 1 || image.layerCount < 1 || !layers)
        return QImage();

    int width = image.finishX;
    int height = image.finishY;

    QImage result(width, height, QImage::Format_RGB16);
    result.fill(0);

    if (layers[0].size)
    {
        dev->seek(offset + layers[0].seek);
        drawR5G6B5(result, layers[0].startX, layers[0].startY, dev);
    }

    return result;
}

QImage loadFrameType2(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, qint64 offset)
{
    if (image.type != 2 || image.layerCount < 3 || !layers)
        return QImage();

    int width = image.finishX;
    int height = image.finishY;

    QImage result(width, height, QImage::Format_ARGB32);
    result.fill(0);

    if (layers[0].size)
    {
        dev->seek(offset + layers[0].seek);
        drawR5G6B5(result, layers[0].startX, layers[0].startY, dev);
    }

    if (layers[1].size)
    {
        dev->seek(offset + layers[1].seek);
        drawR5G6B5(result, layers[1].startX, layers[1].startY, dev);
    }

    if (layers[2].size)
    {
        dev->seek(offset + layers[2].seek);
        drawA6(result, layers[2].startX, layers[2].startY, dev);
    }

    return result;
}

QImage loadFrameType3(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, qint64 offset)
{
    if (image.type != 3 || image.layerCount < 2 || !layers)
        return QImage();

    int width = image.finishX;
    int height = image.finishY;

    QImage result(width, height, QImage::Format_ARGB32_Premultiplied);
    result.fill(0);

    if (layers[0].size)
    {
        dev->seek(offset + layers[0].seek);
        drawRGBI(result, layers[0].startX, layers[0].startY, dev);
    }

    if (layers[1].size)
    {
        dev->seek(offset + layers[1].seek);
        drawAI(result, layers[1].startX, layers[1].startY, dev);
    }

    return result;
}

QImage loadFrameType4(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, qint64 offset)
{

    if (image.type != 4 || image.layerCount < 2 || !layers)
        return QImage();

    int width = image.finishX;
    int height = image.finishY;

    QImage result(width, height, QImage::Format_Indexed8);
    result.fill(0);

    if (layers[0].size && layers[1].size)
    {
        dev->seek(offset + layers[0].seek);
        blitARGBI(result, layers[0].startX, layers[0].startY, (layers[0].finishX - layers[0].startX), (layers[0].finishY - layers[0].startY), dev);

        dev->seek(offset + layers[1].seek);
        QVector<QRgb> pal(layers[1].size / 4);
        dev->read((char*)pal.data(), layers[1].size);

        //FIXME: Does color table really requires a premultiply?
        for (int i = 0; i < layers[1].size / 4; i++)
            pal[i] = qPremultiply(qRgba(pal[i] & 0xFF, ((pal[i] >> 8) & 0xFF), ((pal[i] >> 16) & 0xFF), (pal[i] >> 24) & 0xFF));

        result.setColorTable(pal);
    }

    return result;
}

QImage loadFrameType5(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, const QImage &background, qint64 offset)
{
    if (image.type != 5 || !image.layerCount || !layers)
        return QImage();

    int width = image.finishX;
    int height = image.finishY;

    QImage result;

    if (!background.isNull())
    {
        result = background;
    }
    else
    {
        result = QImage(width, height, QImage::Format_ARGB32);
        result.fill(0);
    }


    if (layers[0].size)
    {
        dev->seek(offset + layers[0].seek);
        decodeGAIDeltaFrame(result, layers[0].startX, layers[0].startY, dev);
    }

    return result;
}

QImage loadGIImageData(const GIFrameHeader& image, const GILayerHeader *layers, QIODevice *dev, const QImage &background, qint64 offset)
{
    switch (image.type)
    {
    case 0:
        return loadFrameType0(image, layers, dev, offset);
    case 1:
        return loadFrameType1(image, layers, dev, offset);
    case 2:
        return loadFrameType2(image, layers, dev, offset);
    case 3:
        return loadFrameType3(image, layers, dev, offset);
    case 4:
        return loadFrameType4(image, layers, dev, offset);
    case 5:
        return loadFrameType5(image, layers, dev, background, offset);
    default:
        qWarning() << "Unknown GI frame type: " << image.type << endl;
    }
    return QImage();
}

QImage loadGIFrame(QIODevice *dev, bool animation, const QImage &background, int startX, int startY, int finishX, int finishY)
{
    GIFrameHeader image;
    GILayerHeader *layers;

    if (!checkGIHeader(dev))
        return QImage();

    qint64 offset = dev->pos();
    dev->read((char*)&image, sizeof(GIFrameHeader));

    layers = new GILayerHeader[image.layerCount];

    for (int i = 0; i < image.layerCount; i++)
    {
        dev->read((char*)&layers[i], sizeof(GILayerHeader));

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

    QImage resultFrame = loadGIImageData(image, layers, dev, background, offset);

    delete[] layers;

    return resultFrame;
}

GIFrameHeader peekGIHeader(QIODevice *dev)
{
    GIFrameHeader result;
    dev->peek((char *)&result, sizeof(GIFrameHeader));
    return result;
}

GIFrameHeader readGIHeader(QIODevice *dev)
{
    GIFrameHeader result;
    dev->read((char *)&result, sizeof(GIFrameHeader));
    return result;
}
}
