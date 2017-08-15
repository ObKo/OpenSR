/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/GAITexture.h"

#include <OpenSR/libRangerQt.h>
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QDebug>
#include <QBuffer>

namespace OpenSR
{
GAITexture::GAITexture(const GAIHeader& header, const QImage& background): QSGTexture(),
    m_texID(0), m_header(header), m_needDraw(false), m_pbo(nullptr), m_bg(background)
{
    m_size = QSize(m_header.finishX - m_header.startX, m_header.finishY - m_header.startY);
    reset();
}

GAITexture::~GAITexture()
{
    if(m_pbo)
        delete m_pbo;
    m_pbo = nullptr;
}

void GAITexture::drawNextFrame(const QByteArray& frameData, const QPoint& start)
{
    m_nextFrameData = frameData;
    m_decodeStart = start;
    m_needDraw = true;
}

void GAITexture::reset()
{
    m_nextFrameData = QByteArray();
    m_needDraw = true;
}

void GAITexture::bind()
{
    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();

    if (m_texID && !m_needDraw)
    {
        gl->glBindTexture(GL_TEXTURE_2D, m_texID);
        updateBindOptions();
        return;
    }

    if (!m_texID)
    {
        gl->glGenTextures(1, (GLuint*)&m_texID);
        gl->glBindTexture(GL_TEXTURE_2D, m_texID);
        gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width(), m_size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        updateBindOptions(true);
    }

    if (!m_pbo)
    {
        m_pbo = new QOpenGLBuffer(QOpenGLBuffer::PixelUnpackBuffer);
        m_pbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
        m_pbo->create();
        m_pbo->bind();
        m_pbo->allocate(m_size.width() * m_size.height() * 4);
        gl->glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

    if (m_needDraw)
    {
        m_pbo->bind();
        uchar *pboData = (uchar*)m_pbo->map(QOpenGLBuffer::WriteOnly);

        if (m_nextFrameData.isEmpty())
        {
            if (!m_bg.isNull())
                memcpy(pboData, m_bg.constBits(), m_size.width() * m_size.height() * 4);
        }
        else
        {
            QImage img(pboData, m_size.width(), m_size.height(), QImage::Format_RGBA8888);
            QBuffer d(&m_nextFrameData);
            d.open(QIODevice::ReadOnly);
            decodeGAIDeltaFrame(img, m_decodeStart.x(), m_decodeStart.y(), &d);
            d.close();
        }
        m_pbo->unmap();

        m_needDraw = false;

        gl->glBindTexture(GL_TEXTURE_2D, m_texID);
        gl->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.width(), m_size.height(), GL_RGBA, GL_UNSIGNED_BYTE, 0);
        gl->glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
}

bool GAITexture::hasAlphaChannel() const
{
    return true;
}

bool GAITexture::hasMipmaps() const
{
    return false;
}

int GAITexture::textureId() const
{
    return m_texID;
}

QSize GAITexture::textureSize() const
{
    return m_size;
}
}
