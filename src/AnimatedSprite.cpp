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
}

AnimatedSprite::AnimatedSprite(const Rangers::AnimatedSprite& other): Sprite(other)
{
    t = other.t;
    animFrame = other.animFrame;
}

AnimatedSprite& AnimatedSprite::operator=(const Rangers::AnimatedSprite& other)
{
    t = other.t;
    animFrame = other.animFrame;
    
    if (this == &other)
        return *this;
    
    ::Sprite::operator=(other);
    return *this;
}



void AnimatedSprite::processLogic(int dt)
{
    AnimatedTexture *texture = static_cast<AnimatedTexture *>(spriteTexture.get());

    while (t > texture->seek()/4)
    {
        animFrame = (animFrame + 1) % texture->count();
        t -= texture->seek()/4;
    }

    t += dt;
}

void AnimatedSprite::draw() const
{
    if (!spriteTexture)
        return;
    SDL_mutexP(objectMutex);
    prepareDraw();

    glBindTexture(GL_TEXTURE_2D, ((AnimatedTexture*)spriteTexture.get())->openGLTexture(animFrame));

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
    SDL_mutexV(objectMutex);
}

int AnimatedSprite::currentFrame() const
{
    return animFrame;
}

void AnimatedSprite::setFrame(int f)
{
    animFrame = f;
}
