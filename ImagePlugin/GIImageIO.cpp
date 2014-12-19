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

#include "GIImageIO.h"
#include "OpenSR/libRangerQt.h"

#include <QImage>
#include <QVariant>

namespace OpenSR
{
GIImageIO::GIImageIO() :
    QImageIOHandler()
{
}


GIImageIO::~GIImageIO()
{
}

bool GIImageIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::ImageFormat)
        return true;

    return false;
}

QVariant GIImageIO::option(ImageOption option) const
{
    if (!checkGIHeader(device()))
        return QVariant();

    GIFrameHeader header = peekGIHeader(device());

    if (option == QImageIOHandler::Size)
    {
        return QSize(header.finishX - header.startX, header.finishY - header.startY);
    }

    if (option == QImageIOHandler::ImageFormat)
    {
        switch (header.type)
        {
        case 1:
            return QImage::Format_RGB16;
        case 0:
            if ((header.aBitmask == 0xFF000000) && (header.rBitmask == 0xFF0000) && (header.gBitmask == 0xFF00) && (header.bBitmask == 0xFF))
                return QImage::Format_ARGB32;
            else if ((header.rBitmask == 0xF800) && (header.gBitmask == 0x7E0) && (header.bBitmask == 0x1F))
                return QImage::Format_RGB16;
            else
                return QImage::Format_Invalid;
        case 3:
            return QImage::Format_ARGB32_Premultiplied;
        case 2:
        case 5:
            return QImage::Format_ARGB32;
        case 4:
            return QImage::Format_Indexed8;
        default:
            return QImage::Format_Invalid;
        }
    }
    return QVariant();
}

bool GIImageIO::canRead() const
{
    return checkGIHeader(device());
}

bool GIImageIO::read(QImage *image)
{
    *image = loadGIFrame(device());
    return true;
}
}
