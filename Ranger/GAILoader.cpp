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
#include <QBuffer>

namespace OpenSR
{
bool checkGAIHeader(QIODevice *dev)
{
    quint32 sig;
    qint64 size = dev->peek((char*)&sig, sizeof(quint32));

    if (size != sizeof(quint32) || sig != GAI_SIGNATURE)
        return false;

    return true;
}

QVector<int> loadGAITimes(QIODevice *dev, const GAIHeader& header)
{
    QVector<int> times(header.frameCount);

    if (!header.waitSize)
        return times;

    uint8_t *waitData = new uint8_t[header.waitSize];
    dev->seek(header.waitSeek);
    dev->read((char*)waitData, header.waitSize);

    uint32_t timeBlockCount = *((uint32_t*)waitData);
    uint8_t *p = waitData + 4 + timeBlockCount * 8 + 2;

    for (int i = 0; i < timeBlockCount; i++)
    {
        uint32_t blockFrameCount = *((uint32_t *)p);
        for (int j = 0; j < blockFrameCount; j++)
        {
            uint32_t frame = *((uint32_t *)(p + 4 + j * 8));
            uint32_t time = *((uint32_t *)(p + 4 + j * 8 + 4));
            if (frame < header.frameCount)
                times[frame] = time;
        }
        p += blockFrameCount * 8 + 4 + 2;
    }

    delete[] waitData;
    return times;
}

QImage loadGAIFrame(QIODevice *dev, const GAIHeader& header, int i, const QImage &background)
{
    if (i >= header.frameCount)
        return QImage();

    QImage result;
    uint32_t giSeek, giSize;
    dev->seek(sizeof(GAIHeader) + i * 2 * sizeof(uint32_t));
    dev->read((char*)&giSeek, sizeof(uint32_t));
    dev->read((char*)&giSize, sizeof(uint32_t));

    if (giSeek && giSize)
    {
        qint64 giOffset = giSeek;
        uint32_t signature;
        dev->seek(giOffset);
        dev->peek((char*)&signature, sizeof(uint32_t));

        if (signature == ZL01_SIGNATURE || signature == ZL02_SIGNATURE)
        {
            QByteArray zlibData = dev->read(giSize);
            QByteArray data = unpackZL(zlibData);

            QBuffer bufdev(&data);
            bufdev.open(QIODevice::ReadOnly);
            result = loadGIFrame(&bufdev, true, background, header.startX, header.startY, header.finishX, header.finishY);
        }
        else
        {
            result = loadGIFrame(dev, true, background, header.startX, header.startY, header.finishX, header.finishY);
        }
    }
    else
    {
        if (!background.isNull())
            result = background;
        else
            result = QImage(header.finishX - header.startX, header.finishY - header.startY, QImage::Format_ARGB32);
    }
    return result;
}

Animation loadGAIAnimation(QIODevice *dev, const QImage &background)
{
    if (!checkGAIHeader(dev))
        return Animation();

    GAIHeader header = readGAIHeader(dev);
    QImage bg = background;

    Animation result;
    result.images = QVector<QImage>(header.frameCount);
    result.times = loadGAITimes(dev, header);

    for (int i = 0; i < header.frameCount; i++)
    {
        result.images[i] = loadGAIFrame(dev, header, i, bg);
        bg = result.images[i];
    }

    return result;
}

GAIHeader peekGAIHeader(QIODevice *dev)
{
    GAIHeader result;
    dev->peek((char*)&result, sizeof(GAIHeader));
    return result;
}

GAIHeader readGAIHeader(QIODevice *dev)
{
    GAIHeader result;
    dev->read((char*)&result, sizeof(GAIHeader));
    return result;
}

}
