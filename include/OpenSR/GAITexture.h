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

#ifndef OPENSR_GAITEXTURE_H
#define OPENSR_GAITEXTURE_H

#include <OpenSR/OpenSR.h>
#include <QSGTexture>
#include <OpenSR/libRangerQt.h>

class QOpenGLBuffer;
class QOpenGLFunctions;

namespace OpenSR
{
class ENGINE_API GAITexture: public QSGTexture
{
public:
    GAITexture(const GAIHeader& header, const QImage& background);
    virtual ~GAITexture();

    virtual void bind();
    virtual bool hasAlphaChannel() const;
    virtual bool hasMipmaps() const;
    virtual int textureId() const;
    virtual QSize textureSize() const;

    void drawNextFrame(const QByteArray& frameData, const QPoint& start);
    void drawNextFrame(const QImage& frame);
    void reset();

private:
    int m_texID;
    QOpenGLBuffer *m_pbo;
    QImage m_bg;
    GAIHeader m_header;
    bool m_needDraw;
    QSize m_size;

    QByteArray m_nextFrameData;
    QPoint m_decodeStart;
};
}

#endif // OPENSR_GAITEXTURE_H
