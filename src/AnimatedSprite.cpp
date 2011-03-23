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


#include "AnimatedSprite.h"
#include "Engine.h"

using namespace Rangers;

/*!
 * \param texture texture
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(boost::shared_ptr<AnimatedTexture> texture,  Object *parent): Sprite(texture, parent)
{
    t = 0;
    animFrame= 0;
    singleShot = false;
    if(!texture)
    {
        started = false;
	frameTime = 0;
    }
    else
    {
        started = true;
        frameTime = texture->seek() > 1000 ? 50 : 100;
    }
}

AnimatedSprite::AnimatedSprite(const Rangers::AnimatedSprite& other): Sprite(other)
{
    t = other.t;
    animFrame = other.animFrame;
    singleShot = other.singleShot;
    started = other.started;
    frameTime = other.frameTime;
}

AnimatedSprite& AnimatedSprite::operator=(const Rangers::AnimatedSprite& other)
{
    if (this == &other)
        return *this;
    
    t = other.t;
    animFrame = other.animFrame;
    singleShot = other.singleShot;
    started = other.started;
    animFrame = other.animFrame;
    frameTime = other.frameTime;
      
    ::Sprite::operator=(other);
    return *this;
}



void AnimatedSprite::processLogic(int dt)
{
    if(!spriteTexture)
        return;
    AnimatedTexture *texture = static_cast<AnimatedTexture *>(spriteTexture.get());

    if(started)
    {
	while (t > frameTime)
	{
	    if((texture->loadedFrames() == texture->count()) || (animFrame < texture->loadedFrames() - 1))
		animFrame = (animFrame + 1) % texture->count();
	    t -= frameTime;
	}

	t += dt;
    }
}

void AnimatedSprite::draw() const
{
    if (!spriteTexture)
        return;
    lock();
    prepareDraw();

    glBindTexture(GL_TEXTURE_2D, ((AnimatedTexture*)spriteTexture.get())->openGLTexture(animFrame));
    if (textureScaling == TEXTURE_TILE_X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    if (textureScaling == TEXTURE_TILE_Y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    if (textureScaling == TEXTURE_TILE)
    {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }   

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float)*2));

    glDrawArrays(GL_QUADS, 0, vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
    unlock();
}

int AnimatedSprite::currentFrame() const
{
    return animFrame;
}

float AnimatedSprite::fps() const
{
    return 1000.0f / frameTime;
}

void AnimatedSprite::setFrame(int f)
{
    animFrame = f;
}

void AnimatedSprite::setFPS(float f)
{
    if(f <= 0.0f)
        frameTime = INT_MAX;
    frameTime = 1000.0f / f;
}

void AnimatedSprite::setSingleShot(bool ss)
{
    singleShot = ss;
}

bool AnimatedSprite::isSingleShot() const
{
    return singleShot;
}

bool AnimatedSprite::isStarted() const
{
    return started;
}

void AnimatedSprite::start()
{
    started = true;
}

void AnimatedSprite::stop()
{
    started = false;
}

void AnimatedSprite::reset()
{
    started = false;
    animFrame = 0;
    t = 0;
}


