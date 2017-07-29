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

#include "PSDImageIO.h"
#include "OpenSR/libRangerQt.h"

#include <QImage>
#include <QVariant>

namespace OpenSR
{
PSDImageIO::PSDImageIO() :
    QImageIOHandler()
{
}


PSDImageIO::~PSDImageIO()
{
}

bool PSDImageIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::ImageFormat)
        return true;

    return false;
}

QVariant PSDImageIO::option(ImageOption option) const
{
    if (!checkPSDHeader(device()))
        return QVariant();

    PSDHeader header = peekPSDHeader(device());

    if (option == QImageIOHandler::Size)
    {
        return QSize(header.width, header.height);
    }

    if (option == QImageIOHandler::ImageFormat)
    {
        return getPSDFormat(header);
    }
    return QVariant();
}

bool PSDImageIO::canRead() const
{
    return checkPSDHeader(device());
}

bool PSDImageIO::read(QImage *image)
{
    *image = loadPSDFrame(device());
    return true;
}
}
