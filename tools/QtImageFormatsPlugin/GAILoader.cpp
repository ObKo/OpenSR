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
#include <QImage>
#include <QIODevice>
#include <QBuffer>

namespace Rangers
{
bool loadGAIFrame(QIODevice *device, QImage *image, GAIHeader *header, int num, QImage *background)
{
    if (num >= header->frameCount)
        return false;

    uint32_t seek, size;
    device->seek(sizeof(GAIHeader) + num * 2 * sizeof(uint32_t));
    device->read((char*)&seek, 4);
    device->read((char*)&size, 4);

    *image = QImage(header->finishX - header->startX, header->finishY - header->startY, QImage::Format_ARGB32);
    image->fill(0);

    if (seek && size)
    {
        uint32_t signature;
        device->seek(seek);
        device->peek((char*)&signature, 4);

        if (signature == 0x31304c5a)
        {
            char *buffer = new char[size];
            device->read((char *)buffer, size);

            uint32_t beSize = (((uint8_t *)buffer)[4] << 24) | (((uint8_t *)buffer)[5] << 16) |
                              (((uint8_t *)buffer)[6] << 8) | (((uint8_t *)buffer)[7]);
            ((uint32_t *)buffer)[1] = beSize;
            QByteArray gi = qUncompress(QByteArray::fromRawData(buffer + 4, size - 4));

            delete[] buffer;

            QBuffer dev(&gi);
            dev.open(QIODevice::ReadOnly);
            return loadGIFrame(&dev, image, 0, background, QPoint(header->startX, header->startY));
        }
        else
        {
            return loadGIFrame(device, image, seek, background, QPoint(header->startX, header->startY));
        }
    }
    else
    {
        if (!background || background->isNull())
            image->fill(0xFF000000);
        else
            *image = *background;
    }
    return true;
}
}
