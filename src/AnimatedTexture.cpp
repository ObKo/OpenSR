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

#include "AnimatedTexture.h"
#include "Engine.h"
#include "Log.h"

namespace Rangers
{
/*!
 * \param a hai animation
 */
AnimatedTexture::AnimatedTexture(const HAIAnimation& a)
{
    m_waitSeek = 600;
    m_waitSize = 600;
    m_width = a.width;
    m_height = a.height;
    m_frameCount = a.frameCount;

    m_textures = new GLuint[m_frameCount];
    m_textures[0] = m_openGLTexture;
    glGenTextures(m_frameCount - 1, m_textures + 1);

    for (int i = 0; i < m_frameCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_B8G8R8A8), a.width, a.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, a.frames + a.width * a.height * 4 * i);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    m_loadedFrames = m_frameCount;
    m_needFrames = false;
}

/*!
 * \param a gai animation
 */
AnimatedTexture::AnimatedTexture(const GAIAnimation& a)
{
    m_waitSeek = a.waitSeek;
    m_waitSize = a.waitSize;
    m_width = a.width;
    m_height = a.height;
    m_frameCount = a.frameCount;

    m_textures = new GLuint[m_frameCount];
    m_textures[0] = m_openGLTexture;
    glGenTextures(m_frameCount - 1, m_textures + 1);

    size_t s = 0;

    for (int i = 0; i < m_frameCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_B8G8R8A8), a.frames[i].width, a.frames[i].height, 0, GL_BGRA, GL_UNSIGNED_BYTE, a.frames[i].data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    m_loadedFrames = m_frameCount;
    m_needFrames = false;
}

AnimatedTexture::~AnimatedTexture()
{
    m_openGLTexture = m_textures[0];
    glDeleteTextures(m_frameCount - 1, &m_textures[1]);
}

/*!
 * \param i frame index
 * \return OpenGL texture id
 */
GLuint AnimatedTexture::openGLTexture(int i) const
{
    if (!m_loadedFrames)
    {
        m_needFrames = true;
        return 0;
    }
    if (i >= m_loadedFrames - 2)
        m_needFrames = true;
    else
        m_needFrames = false;

    if (i >= m_loadedFrames)
        return m_textures[m_loadedFrames - 1];
    else
        return m_textures[i];
}

/*!
 * \return seek
 */
int AnimatedTexture::waitSeek() const
{
    return m_waitSeek;
}

/*!
 * \return size
 */
int AnimatedTexture::waitSize() const
{
    return m_waitSize;
}

/*!
 * \return frame count
 */
int AnimatedTexture::frameCount() const
{
    return m_frameCount;
}

AnimatedTexture::AnimatedTexture(int width, int height, int seek, int size, int count)
{
    m_waitSeek = seek;
    m_waitSize = size;
    m_width = width;
    m_height = height;
    m_frameCount = count;

    m_textures = new GLuint[m_frameCount];
    m_textures[0] = m_openGLTexture;
    glGenTextures(m_frameCount - 1, m_textures + 1);
    m_loadedFrames = 0;
    m_needFrames = true;
}

int AnimatedTexture::loadedFrames() const
{
    return m_loadedFrames;
}

void AnimatedTexture::loadFrame(const char* data, int width, int height, TextureType type, int dataSize)
{
    if (m_loadedFrames == m_frameCount)
        return;

    glBindTexture(GL_TEXTURE_2D, m_textures[m_loadedFrames]);
    GLint internalFormat = Engine::instance()->textureInternalFormat(type);

    switch (type)
    {
    case TEXTURE_A8:
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    case TEXTURE_B8G8R8A8:
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        break;
    case TEXTURE_R8G8B8A8:
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    case TEXTURE_DXT1:
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, dataSize, data);
        break;
    case TEXTURE_DXT3:
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, width, height, 0, dataSize, data);
        break;
    case TEXTURE_DXT5:
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, dataSize, data);
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_loadedFrames++;
    m_needFrames = false;
}

bool AnimatedTexture::needFrames() const
{
    return m_needFrames;
}
}
