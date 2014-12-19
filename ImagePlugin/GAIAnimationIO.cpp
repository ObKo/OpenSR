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

#include "GAIAnimationIO.h"
#include <QImage>
#include <QVariant>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

namespace OpenSR
{
GAIAnimationIO::GAIAnimationIO(): QImageIOHandler(), m_currentFrame(-1)
{
}


GAIAnimationIO::~GAIAnimationIO()
{
}

bool GAIAnimationIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::Animation)
        return true;
    return false;
}

QVariant GAIAnimationIO::option(ImageOption option) const
{
    if (!checkGAIHeader(device()))
        return QVariant();

    GAIHeader header = peekGAIHeader(device());

    if (option == QImageIOHandler::Animation)
        return true;

    if (option == QImageIOHandler::Size)
        return QSize(header.finishX - header.startX, header.finishY - header.startX);

    return QVariant();
}

bool GAIAnimationIO::canRead() const
{
    QIODevice *d = device();

    if (!d)
        return false;

    if (m_currentFrame < 0)
    {
        return checkGAIHeader(d);
    }
    else
    {
        if (m_currentFrame >= m_header.frameCount)
            return false;
    }

    return true;
}

bool GAIAnimationIO::read(QImage *image)
{
    QIODevice *d = device();

    if (!d)
        return false;

    if (m_currentFrame < 0)
    {
        m_header = readGAIHeader(d);
        m_times = loadGAITimes(d, m_header);
        m_currentFrame = 0;
    }

    if (m_currentFrame >= m_header.frameCount)
        return false;

    if (m_currentFrame == 0 && m_header.haveBackground)
    {
        QFile *f = qobject_cast<QFile*>(d);
        if (f)
        {
            QFileInfo gaiFile(f->fileName());
            QString externalGIFrame = gaiFile.dir().canonicalPath() + QDir::separator() + gaiFile.baseName() + ".gi";
            if (!QFileInfo(externalGIFrame).exists())
                externalGIFrame = gaiFile.dir().canonicalPath() + QDir::separator() + gaiFile.baseName() + ".GI";

            if (QFileInfo(externalGIFrame).exists())
                m_prev = QImage(externalGIFrame);
        }
    }

    *image = loadGAIFrame(device(), m_header, m_currentFrame, m_prev);

    if (!image->isNull())
        m_currentFrame++;

    m_prev = *image;

    return true;
}

int GAIAnimationIO::currentImageNumber() const
{
    return m_currentFrame;
}

int GAIAnimationIO::imageCount() const
{
    GAIHeader header = m_header;
    if (m_currentFrame < 0)
    {
        header = peekGAIHeader(device());
    }
    return header.frameCount;
}

int GAIAnimationIO::nextImageDelay() const
{
    if (m_currentFrame < 0)
        return 0;
    if (m_currentFrame >= m_times.count())
        return 0;

    return m_times[m_currentFrame];
}

int GAIAnimationIO::loopCount() const
{
    return -1;
}
}
