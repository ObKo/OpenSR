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

using namespace Rangers;

/*!
 * \param a hai animation
 */
AnimatedTexture::AnimatedTexture(const HAIAnimation& a)
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
    loadedAnimationFrames = frameCount;
    m_needFrames = false;
}

/*!
 * \param a gai animation
 */
AnimatedTexture::AnimatedTexture(const GAIAnimation& a)
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

    size_t s = 0;
    
    for (int i = 0; i < frameCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
	
        glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_R8G8B8A8), a.frames[i].width, a.frames[i].height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, a.frames[i].data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    loadedAnimationFrames = frameCount;
    m_needFrames = false;
}

AnimatedTexture::~AnimatedTexture()
{
    textureID = textureIDs[0];
    glDeleteTextures(frameCount - 1, &textureIDs[1]);
}

/*!
 * \param i frame index
 * \return OpenGL texture id
 */
GLuint AnimatedTexture::openGLTexture(int i)
{
    if(!loadedAnimationFrames)
    {
        m_needFrames = true;
        return 0;
    }
    if(i >= loadedAnimationFrames - 2)
        m_needFrames = true;
    else
        m_needFrames = false;
    
    if(i >= loadedAnimationFrames)
        return textureIDs[loadedAnimationFrames - 1];
    else
        return textureIDs[i];
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

AnimatedTexture::AnimatedTexture(int width, int height, int seek, int size, int count)
{
    ::Texture();

    waitSeek = seek;
    waitSize = size;
    realWidth = width;
    realHeight = height;
    frameCount = count;

    textureIDs = new GLuint[frameCount];
    textureIDs[0] = textureID;
    glGenTextures(frameCount - 1, textureIDs + 1);
    loadedAnimationFrames = 0;
    m_needFrames = true;
}

int AnimatedTexture::loadedFrames() const
{
    return loadedAnimationFrames;
}

void AnimatedTexture::loadFrame(const char* data, int width, int height, TextureType type)
{
	if(loadedAnimationFrames == frameCount)
	    return;
	
        glBindTexture(GL_TEXTURE_2D, textureIDs[loadedAnimationFrames]);
	
	switch(type)
	{
	case TEXTURE_A8: 
	    glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_A8), width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
	    break;
	case TEXTURE_R8G8B8A8:
	    glTexImage2D(GL_TEXTURE_2D, 0, Engine::instance()->textureInternalFormat(TEXTURE_R8G8B8A8), width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
	    break;
	}

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	loadedAnimationFrames++;
 	m_needFrames = false;
}

bool AnimatedTexture::needFrames() const
{
    return m_needFrames;
}
