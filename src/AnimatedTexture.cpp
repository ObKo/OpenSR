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

using namespace Rangers;

/*!
 * \param a hai animation
 */
AnimatedTexture::AnimatedTexture(HAIAnimation a)
{
    ::Texture();

    waitSeek = 600;
    waitSize = 600;
    realWidth = a.width;
    realHeight = a.height;
    frameCount = a.frameCount;

    textureIDs = new GLuint[frameCount];
    textureIDs[0] = textureID;
    glGenTextures(frameCount - 1, textureIDs + 1);

    for (int i = 0; i < frameCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_R8G8B8A8), a.width, a.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, a.frames + a.width * a.height * 4 * i);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

/*!
 * \param a gai animation
 */
AnimatedTexture::AnimatedTexture(GAIAnimation a)
{
    ::Texture();

    waitSeek = a.waitSeek;
    waitSize = a.waitSize;
    realWidth = a.width;
    realHeight = a.height;
    frameCount = a.frameCount;

    textureIDs = new GLuint[frameCount];
    textureIDs[0] = textureID;
    glGenTextures(frameCount - 1, textureIDs + 1);

    for (int i = 0; i < frameCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_R8G8B8A8), a.frames[i].width, a.frames[i].height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, a.frames[i].data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

AnimatedTexture::~AnimatedTexture()
{
    textureID = textureIDs[0];
    glDeleteTextures(frameCount - 1, textureIDs + 1);
}

/*!
 * \param i frame index
 * \return OpenGL texture id
 */
GLuint AnimatedTexture::openGLTexture(int i) const
{
    return textureIDs[i % frameCount];
}

/*!
 * \return seek
 */
int AnimatedTexture::seek() const
{
    return waitSeek;
}

/*!
 * \return size
 */
int AnimatedTexture::size() const
{
    return waitSize;
}

/*!
 * \return frame count
 */
int AnimatedTexture::count() const
{
    return frameCount;
}
