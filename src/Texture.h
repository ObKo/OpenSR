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

#ifndef RANGERS_TEXTURE_H
#define RANGERS_TEXTURE_H

#include "Types.h"

namespace Rangers
{
enum TextureType {TEXTURE_A8, TEXTURE_R8G8B8A8, TEXTURE_B8G8R8A8, TEXTURE_DXT1, TEXTURE_DXT3, TEXTURE_DXT5};

class Texture
{
public:
    Texture();
    Texture(int width, int height, TextureType type, unsigned char *rawData, int dataSize = 0);
    ~Texture();

    int width() const;
    int height() const;
    GLuint openGLTexture();

    void setRawData(int width, int height, TextureType type, unsigned char *rawData, int dataSize = 0);

protected:
    int m_width, m_height;
    GLuint m_openGLTexture;
};
}

#endif
