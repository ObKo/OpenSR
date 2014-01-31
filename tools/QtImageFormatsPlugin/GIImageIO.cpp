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
#include "Types.h"

#include <QImage>
#include <QVariant>

namespace Rangers
{
GIImageIO::GIImageIO() :
    QImageIOHandler(), m_header(0)
{
}


GIImageIO::~GIImageIO()
{
    delete m_header;
}

void GIImageIO::peekHeader() const
{
    if (m_header)
        return;

    QIODevice *d = device();

    if (!d)
        return;

    if (!m_header)
    {
        m_header = new GIFrameHeader();
        if (d->peek((char*)m_header, sizeof(GIFrameHeader)) != sizeof(GIFrameHeader))
        {
            delete m_header;
            m_header = 0;
            return;
        }
    }

    if (m_header->signature != 0x00006967)
    {
        delete m_header;
        m_header = 0;
        return;
    }
}

bool GIImageIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::ImageFormat)
        return true;

    return false;
}

QVariant GIImageIO::option(ImageOption option) const
{
    peekHeader();

    if (!m_header || m_header->signature != 0x00006967)
        return QVariant();

    if (option == QImageIOHandler::Size)
    {
        return QSize(m_header->finishX - m_header->startX, m_header->finishY - m_header->startY);
    }

    if (option == QImageIOHandler::ImageFormat)
    {
        switch (m_header->type)
        {
        case 1:
            return QImage::Format_RGB16;
        case 0:
            if ((m_header->aBitmask == 0xFF000000) && (m_header->rBitmask == 0xFF0000) && (m_header->gBitmask == 0xFF00) && (m_header->bBitmask == 0xFF))
                return QImage::Format_ARGB32;
            else if ((m_header->rBitmask == 0xF800) && (m_header->gBitmask == 0x7E0) && (m_header->bBitmask == 0x1F))
                return QImage::Format_RGB16;
            else
                return QImage::Format_Invalid;
        case 3:
            return QImage::Format_ARGB32_Premultiplied;
        case 2:
        case 4:
        case 5:
            return QImage::Format_ARGB32;
        default:
            return QImage::Format_Invalid;
        }
    }
    return QVariant();
}

bool GIImageIO::canRead() const
{
    QIODevice *d = device();

    if (!d)
        return false;

    peekHeader();

    if (!m_header || m_header->signature != 0x00006967)
        return false;

    return true;
}

bool GIImageIO::read(QImage *image)
{
    peekHeader();

    if (!m_header || m_header->signature != 0x00006967)
        return false;

    return loadGIFrame(device(), image);
}
}
