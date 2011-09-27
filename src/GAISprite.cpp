/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#include "GAISprite.h"
#include "Log.h"
#include "Texture.h"
#include <cstdlib>

extern void drawF5ToBGRA(unsigned char * bufdes, int bufdesll, const unsigned char * graphbuf);
extern void copyImageData(unsigned char *bufdes, int destwidth, int x, int y, int w, int h, unsigned char *graphbuf);

namespace Rangers
{

GAISprite::GAISprite(const char *data, int size, GIFrame baseFrame, Object *parent): AnimatedSprite(parent)
{
    m_dataSize = size;
    m_gaiHeader = loadGAIHeader(data);
    m_animationTime = 0;
    m_currentFrame = 0;
    m_singleShot = false;
    m_baseFrame = baseFrame;
    m_needNextFrame = true;
    m_textureBuffer = 0;

    if (!m_gaiHeader.haveBackground)
        m_gaiFrames = 0;
    else
    {
        m_gaiFrames = new char*[m_gaiHeader.frameCount];
        m_width = m_gaiHeader.finishX - m_gaiHeader.startX;
        m_height = m_gaiHeader.finishY - m_gaiHeader.startY;
        int width = m_gaiHeader.finishX - m_gaiHeader.startX;
        int height = m_gaiHeader.finishY - m_gaiHeader.startY;
        //m_currentData = new unsigned char[width * height * 4];

        for (int i = 0; i < m_gaiHeader.frameCount; i++)
        {
            uint32_t giSeek , giSize;
            const char *p = data + sizeof(GAIHeader) - sizeof(GIFrame *) + i * 2 * sizeof(uint32_t);
            giSeek = *((uint32_t *)p);
            p += sizeof(uint32_t);
            giSize = *((uint32_t *)p);
            p += sizeof(uint32_t);

            if (giSeek && giSize)
            {
                uint32_t signature;
                p = data + giSeek;
                signature = *((uint32_t *)p);
                p += sizeof(uint32_t);

                if (signature == 0x31304c5a)
                {
                    p = data + giSeek;
                    size_t outsize;
                    m_gaiFrames[i] = (char*)unpackZL01((const unsigned char*)p, giSize, outsize);
                }
                else
                {
                    p = data + giSeek;
                    m_gaiFrames[i] = new char[giSize];
                    memcpy(m_gaiFrames, p, giSize);
                }
            }
        }

        m_animationStarted = true;
    }
    setFrameRate(15);
    m_texture = boost::shared_ptr<Texture>(new Texture(m_width, m_height));
    markToUpdate();
}

GAISprite::~GAISprite()
{
    delete[] m_gaiFrames;
}

void GAISprite::processLogic(int dt)
{
    if (m_animationStarted && m_frameDuration)
    {
        if ((m_animationTime > m_frameDuration) && (!m_needNextFrame))
        {
            m_currentFrame = (m_currentFrame + 1) % m_gaiHeader.frameCount;
            m_animationTime -= m_frameDuration;
            m_needNextFrame = true;
            markToUpdate();
        }

        m_animationTime += dt;
    }
}

void GAISprite::draw()
{
    if (!m_gaiFrames)
        return;

    if (!prepareDraw())
        return;

    if (!m_texture)
        return;

    glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());

    if (m_scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    if (m_scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (m_scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, m_vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

void GAISprite::processMain()
{
    Sprite::processMain();
    lock();

    if (!m_gaiFrames)
        return;

    if (!m_textureBuffer)
    {
        glGenBuffers(1, &m_textureBuffer);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_textureBuffer);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

    if (m_needNextFrame)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_textureBuffer);
        if (m_currentFrame == 0)
        {
            //m_texture->setRawData(m_baseFrame.width, m_baseFrame.height, TEXTURE_B8G8R8A8, m_baseFrame.data, 4 * m_baseFrame.width * m_baseFrame.height);
            unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
            memset(data, 0, m_width * m_height * 4);
            copyImageData(data, m_width, 0, 0, m_baseFrame.width, m_baseFrame.height, m_baseFrame.data);
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
            //FIXME: null as pointer to rgba looks ugly
            m_texture->setRawData(m_baseFrame.width, m_baseFrame.height, TEXTURE_B8G8R8A8, 0, 4 * m_baseFrame.width * m_baseFrame.height);

        }
        else
        {
            drawFrame(m_currentFrame);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        m_needNextFrame = false;
    }
    unlock();
}

void GAISprite::loadGIFrame5(const char *data, unsigned char *background, int startX, int startY, int finishX, int finishY)
{
    const char *buffer = data;
    GIFrameHeader image = *((GIFrameHeader*)data);

    if (image.type != 5)
        return;

    image.layers = new GILayerHeader[image.layerCount];

    if (finishX)
        image.finishX = finishX;

    if (finishY)
        image.finishY = finishY;

    image.startX -= startX;
    image.startY -= startY;
    image.finishX -= startX;
    image.finishY -= startY;

    for (int i = 0; i < image.layerCount; i++)
    {
        buffer = data + 64 + i * 32;
        image.layers[i] = *((GILayerHeader*)buffer);
        buffer = data + image.layers[i].seek;
        //FIXME: const unsigned char* -> unsigned char*
        image.layers[i].data = (unsigned char*)buffer;

        image.layers[i].startX -= startX;
        image.layers[i].startY -= startY;
        image.layers[i].finishX -= startX;
        image.layers[i].finishY -= startY;
    }

    int width;
    int height;


    width = image.finishX;
    height = image.finishY;

    if (image.layers[0].size)
        drawF5ToBGRA(background + (image.layers[0].startY * width + image.layers[0].startX) * 4, width * 4, (const unsigned char *)image.layers[0].data);

    delete image.layers;
}

void GAISprite::drawFrame(int i)
{
    unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    loadGIFrame5((char *)m_gaiFrames[i], data, m_gaiHeader.startX,  m_gaiHeader.startY,  m_gaiHeader.finishX,  m_gaiHeader.finishY);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
}

void GAISprite::setFrame(int f)
{
    Log::warning() << "Cannot set frame on GAISprite";
}

void GAISprite::reset()
{
    if (!m_gaiFrames)
        return;
    m_animationStarted = false;
    m_currentFrame = 0;
    m_animationTime = 0;
    markToUpdate();
}

}
