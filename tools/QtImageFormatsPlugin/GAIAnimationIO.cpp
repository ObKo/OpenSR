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
#include "Types.h"
#include <QImage>
#include <QVariant>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

namespace Rangers
{
GAIAnimationIO::GAIAnimationIO(): QImageIOHandler(),
    m_header(0), m_currentFrame(0)
{
}


GAIAnimationIO::~GAIAnimationIO()
{
    delete m_header;
}

bool GAIAnimationIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::Animation)
        return true;
    return false;
}

QVariant GAIAnimationIO::option(ImageOption option) const
{
    peekHeader();

    if (option == QImageIOHandler::Animation)
        return true;

    if (!m_header)
        return QVariant();

    if (option == QImageIOHandler::Size)
        return QSize(m_header->finishX - m_header->startX, m_header->finishY - m_header->startX);

    return QVariant();
}


void GAIAnimationIO::peekHeader() const
{
    if (m_header)
        return;

    QIODevice *d = device();

    if (!d)
        return;

    if (!m_header)
    {
        m_header = new GAIHeader();
        if (d->peek((char*)m_header, sizeof(GAIHeader)) != sizeof(GAIHeader))
        {
            delete m_header;
            m_header = 0;
            return;
        }
    }

    if (m_header->signature != 0x00696167)
    {
        delete m_header;
        m_header = 0;
        return;
    }
}

void GAIAnimationIO::readTimes()
{
    if (m_times.size())
        return;

    if (!m_header)
        return;

    if (!m_header->waitSeek || !m_header->waitSize)
        return;

    QIODevice *d = device();

    uint8_t *waitData = new uint8_t[m_header->waitSize];

    d->seek(m_header->waitSeek);
    if (d->read((char*)waitData, m_header->waitSize) != m_header->waitSize)
    {
        delete[] waitData;
        return;
    }

    m_times.resize(m_header->frameCount);
    for (int i = 0; i < m_header->frameCount; i++)
        m_times[i] = 0;

    uint32_t timeBlockCount = *((uint32_t*)waitData);
    uint8_t *p = waitData + 4 + timeBlockCount * 8 + 2;

    for (int i = 0; i < timeBlockCount; i++)
    {
        uint32_t blockFrameCount = *((uint32_t *)p);
        for (int j = 0; j < blockFrameCount; j++)
        {
            uint32_t frame = *((uint32_t *)(p + 4 + j * 8));
            uint32_t time = *((uint32_t *)(p + 4 + j * 8 + 4));
            if (frame < m_times.count())
                m_times[frame] = time;
        }
        p += blockFrameCount * 8 + 4 + 2;
    }

    delete[] waitData;
}

bool GAIAnimationIO::canRead() const
{
    QIODevice *d = device();

    if (!d)
        return false;

    peekHeader();

    if (!m_header || m_header->signature != 0x00696167)
        return false;

    if (m_currentFrame >= m_header->frameCount)
        return false;

    return true;
}

bool GAIAnimationIO::read(QImage *image)
{
    QIODevice *d = device();

    if (!d)
        return false;

    peekHeader();

    if (!m_header || m_header->signature != 0x00696167)
        return false;

    if (!m_times.count())
        readTimes();

    if (m_currentFrame >= m_header->frameCount)
        return false;

    if (m_currentFrame == 0 && m_header->haveBackground)
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

    bool result = loadGAIFrame(device(), image, m_header, m_currentFrame, &m_prev);
    m_prev = *image;

    if (result)
        m_currentFrame++;

    return result;
}

int GAIAnimationIO::currentImageNumber() const
{
    return m_currentFrame;
}

int GAIAnimationIO::imageCount() const
{
    peekHeader();

    if (!m_header || m_header->signature != 0x00696167)
        return 0;

    return m_header->frameCount;
}

int GAIAnimationIO::nextImageDelay() const
{
    peekHeader();

    if (m_currentFrame >= m_times.count())
        return 0;

    return m_times[m_currentFrame];
}

int GAIAnimationIO::loopCount() const
{
    return 0;
}
}
