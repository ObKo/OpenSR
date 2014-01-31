/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Texture.h"

namespace Rangers
{
struct HAIAnimation;
struct GAIAnimation;
class RANGERS_ENGINE_API AnimatedTexture: public Texture
{
public:
    AnimatedTexture();
    //! Create texture from HAI animation
    AnimatedTexture(const HAIAnimation& a);
    //! Create texture from GAI animation
    AnimatedTexture(const GAIAnimation& a);
    //! Create empty animation for background loading
    AnimatedTexture(int width, int height, int seek, int size, int count);
    ~AnimatedTexture();

    //! Get OpenGL texture id
    GLuint openGLTexture(int i) const;
    //! Frame display time
    uint32_t time(int i) const;

    //! Get frame count
    int frameCount() const;
    //! Number of loaded frames (background loading)
    int loadedFrames() const;
    //! Animation need frame loading
    bool needFrames() const;

    //! Append frame (on-demand loading)
    void loadFrame(const char *data, int width, int height, TextureType type, int dataSize = 0);

private:
    int m_frameCount;
    int m_loadedFrames;
    mutable bool m_needFrames;
    std::vector<uint32_t> m_times;

    GLuint *m_textures;
};
}

#endif
