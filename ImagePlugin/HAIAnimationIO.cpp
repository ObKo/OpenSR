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

#include "HAIAnimationIO.h"
#include <QImage>
#include <QVariant>

namespace OpenSR
{
HAIAnimationIO::HAIAnimationIO(): QImageIOHandler(), m_currentFrame(-1)
{
}


HAIAnimationIO::~HAIAnimationIO()
{
}

bool HAIAnimationIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::Animation)
        return true;
    return false;
}

QVariant HAIAnimationIO::option(ImageOption option) const
{
    if (!checkHAIHeader(device()))
        return QVariant();

    HAIHeader header = peekHAIHeader(device());

    if (option == QImageIOHandler::Animation)
        return true;

    if (option == QImageIOHandler::Size)
        return QSize(header.width, header.height);

    return QVariant();
}

bool HAIAnimationIO::canRead() const
{
    QIODevice *d = device();

    if (!d)
        return false;

    if (m_currentFrame < 0)
    {
        return checkHAIHeader(d);
    }
    else
    {
        if (m_currentFrame >= m_header.count)
            return false;
    }

    return true;
}

bool HAIAnimationIO::read(QImage *image)
{
    QIODevice *d = device();

    if (!d)
        return false;

    if (m_currentFrame < 0)
    {
        m_header = readHAIHeader(d);
        m_currentFrame = 0;
    }

    if (m_currentFrame >= m_header.count)
        return false;

    *image = loadHAIFrame(device(), m_header, m_currentFrame);

    if (!image->isNull())
        m_currentFrame++;

    return true;
}

int HAIAnimationIO::currentImageNumber() const
{
    return m_currentFrame;
}

int HAIAnimationIO::imageCount() const
{
    HAIHeader header = m_header;
    if (m_currentFrame < 0)
    {
        header = peekHAIHeader(device());
    }
    return header.count;
}

int HAIAnimationIO::nextImageDelay() const
{
    if (m_currentFrame < 0)
        return 0;

    return HAI_FRAME_TIME;
}

int HAIAnimationIO::loopCount() const
{
    return -1;
}
}
