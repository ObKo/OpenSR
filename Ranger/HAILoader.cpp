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
extern void blitARGBI(QImage &result, int x, int y, int w, int h, QIODevice *dev);

bool checkHAIHeader(QIODevice *dev)
{
    quint32 sig;
    qint64 size = dev->peek((char*)&sig, sizeof(quint32));

    if (size != sizeof(quint32) || sig != HAI_SIGNATURE)
        return false;

    return true;
}

QImage loadHAIFrame(QIODevice *dev, const HAIHeader& header, int i)
{
    if (i >= header.count)
        return QImage();

    QImage result(header.width, header.height, QImage::Format_Indexed8);
    result.fill(0);

    dev->seek(sizeof(HAIHeader) + i * (header.height * header.rowBytes + header.palSize));

    blitARGBI(result, 0, 0, header.width, header.height, dev);

    QVector<QRgb> pal(header.palSize / 4);
    dev->read((char*)pal.data(), header.palSize);

    //FIXME: Does color table really requires a premultiply?
    for (int i = 0; i < header.palSize / 4; i++)
        pal[i] = qPremultiply(qRgba(pal[i] & 0xFF, ((pal[i] >> 8) & 0xFF), ((pal[i] >> 16) & 0xFF), (pal[i] >> 24) & 0xFF));

    result.setColorTable(pal);

    return result;
}

Animation loadHAIAnimation(QIODevice *dev)
{
    if (!checkHAIHeader(dev))
        return Animation();

    HAIHeader header = readHAIHeader(dev);

    Animation result;
    result.images = QVector<QImage>(header.count);
    result.times = QVector<int>(header.count, HAI_FRAME_TIME);

    for (int i = 0; i < header.count; i++)
    {
        result.images[i] = loadHAIFrame(dev, header, i);
    }

    return result;
}

HAIHeader peekHAIHeader(QIODevice *dev)
{
    HAIHeader result;
    dev->peek((char*)&result, sizeof(HAIHeader));
    return result;
}

HAIHeader readHAIHeader(QIODevice *dev)
{
    HAIHeader result;
    dev->read((char*)&result, sizeof(HAIHeader));
    return result;
}

}
