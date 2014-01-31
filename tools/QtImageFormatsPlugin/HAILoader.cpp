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

namespace Rangers
{
extern void blitRGBAI(QImage *image, int x, int y, int w, int h, char *graphbuf, char *pal);

bool loadHAIFrame(QIODevice *device, QImage *image, HAIHeader *header, int num)
{
    if (!device || !header)
        return false;

    uint8_t *frameData = new uint8_t[header->frameSize];

    device->seek(sizeof(HAIHeader) + header->frameSize * num);
    device->read((char*)frameData, header->frameSize);

    *image = QImage(header->width, header->height, QImage::Format_ARGB32);
    blitRGBAI(image, 0, 0, header->width, header->height, (char*)frameData, (char*)(frameData + header->rowBytes * header->height));
    delete[] frameData;

    return true;
}
}
