#include "HAIAnimationIO.h"

#include <QImage>
#include <QVariant>
#include "Types.h"

namespace Rangers
{
HAIAnimationIO::HAIAnimationIO(): QImageIOHandler(),
    m_header(0), m_currentFrame(0)
{
}


HAIAnimationIO::~HAIAnimationIO()
{
    delete m_header;
}

bool HAIAnimationIO::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Size || option == QImageIOHandler::Animation)
        return true;
    return false;
}

QVariant HAIAnimationIO::option(ImageOption option) const
{
    peekHeader();

    if (option == QImageIOHandler::Animation)
        return true;

    if (!m_header)
        return QVariant();

    if (option == QImageIOHandler::Size)
        return QSize(m_header->width, m_header->height);

    return QVariant();
}


void HAIAnimationIO::peekHeader() const
{
    if (m_header)
        return;

    QIODevice *d = device();

    if (!d)
        return;

    if (!m_header)
    {
        m_header = new HAIHeader();
        if (d->peek((char*)m_header, sizeof(HAIHeader)) != sizeof(HAIHeader))
        {
            delete m_header;
            m_header = 0;
            return;
        }
    }

    if (m_header->signature != 0x04210420)
    {
        delete m_header;
        m_header = 0;
        return;
    }
}

bool HAIAnimationIO::canRead() const
{
    QIODevice *d = device();

    if (!d)
        return false;

    peekHeader();

    if (!m_header || m_header->signature != 0x04210420)
        return false;

    if (m_currentFrame >= m_header->count)
        return false;

    return true;
}

bool HAIAnimationIO::read(QImage *image)
{
    QIODevice *d = device();

    if (!d)
        return false;

    peekHeader();

    if (!m_header || m_header->signature != 0x04210420)
        return false;

    if (m_currentFrame >= m_header->count)
        return false;

    bool result = loadHAIFrame(device(), image, m_header, m_currentFrame);

    if (result)
        m_currentFrame++;

    return true;//result;
}

int HAIAnimationIO::currentImageNumber() const
{
    return m_currentFrame;
}

int HAIAnimationIO::imageCount() const
{
    peekHeader();

    if (m_header && m_header->signature != 0x04210420)
        return 0;

    return m_header->count;
}

int HAIAnimationIO::nextImageDelay() const
{
    return 50;
}

int HAIAnimationIO::loopCount() const
{
    return 0;
}
}
