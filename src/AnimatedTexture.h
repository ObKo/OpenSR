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

#ifndef RANGERS_ANIMATEDTEXTURE_H
#define RANGERS_ANIMATEDTEXTURE_H

#include "Types.h"
#include "Texture.h"
#include <libRanger.h>

namespace Rangers
{
class AnimatedTexture: public Texture
{
public:
    //! Create texture from HAI animation
    AnimatedTexture(HAIAnimation a);
    //! Create texture from GAI animation
    AnimatedTexture(GAIAnimation a);
    ~AnimatedTexture();

    //! Get OpenGL texture id
    GLuint openGLTexture(int i) const;

    int seek() const;
    int size() const;
    //! Get frame count
    int count() const;

    //! Set texture tiling
    void setTiling(bool tx, bool ty);

private:
    int waitSeek, waitSize;
    int frameCount;

    GLuint *textureIDs;
};
};

#endif
