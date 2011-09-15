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

#include "Texture.h"
#include "Engine.h"

namespace Rangers
{
unsigned int nextPowerOfTwo(unsigned int x)
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

Texture::Texture()
{
    glGenTextures(1, &m_openGLTexture);
    m_height = 0;
    m_height = 0;
}

Texture::Texture(int width, int height, TextureType type, unsigned char *rawData)
{
    glGenTextures(1, &m_openGLTexture);

    setRawData(width, height, type, rawData);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_openGLTexture);
}

int Texture::width() const
{
    return m_width;
}

int Texture::height() const
{
    return m_height;
}

GLuint Texture::openGLTexture()
{
    return m_openGLTexture;
}

void Texture::setRawData(int width, int height, TextureType type, unsigned char *rawData)
{
    glBindTexture(GL_TEXTURE_2D, m_openGLTexture);
    GLint internalFormat = Engine::instance()->textureInternalFormat(type);

    switch (type)
    {
    case TEXTURE_R8G8B8A8:
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, rawData);
        break;
    case TEXTURE_A8:
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, rawData);
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_width = width;
    m_height = height;
}
}
