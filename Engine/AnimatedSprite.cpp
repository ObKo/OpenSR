/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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


#include "OpenSR/AnimatedSprite.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/AnimatedTexture.h"

#include "OpenSR/private/AnimatedSprite_p.h"

namespace Rangers
{
AnimatedSpritePrivate::AnimatedSpritePrivate(): SpritePrivate()
{
    animationTime = 0;
    currentFrame = 0;
    singleShot = false;
    animationStarted = false;
    speed = 1.0f;
    region = TextureRegion(boost::shared_ptr<AnimatedTexture>());
}
/*!
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(): Sprite(*(new AnimatedSpritePrivate()))
{
}

/*!
 * \param texture texture
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(boost::shared_ptr<AnimatedTexture> texture):
    Sprite(*(new AnimatedSpritePrivate()))
{
    RANGERS_D(AnimatedSprite);
    d->region = TextureRegion(texture);
    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
        start();
    }
}

/*!
 * \param animation animation resource name
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(const std::string& animation):
    Sprite(*(new AnimatedSpritePrivate()))
{
    RANGERS_D(AnimatedSprite);
    boost::shared_ptr<AnimatedTexture> animTexture = ResourceManager::instance().loadAnimation(animation);
    d->region = TextureRegion(animTexture);
    if (animTexture)
    {
        d->width = animTexture->width();
        d->height = animTexture->height();
        start();
    }
    markToUpdate();
}

AnimatedSprite::AnimatedSprite(AnimatedSpritePrivate &p): Sprite(p)
{
}

void AnimatedSprite::processLogic(int dt)
{
    RANGERS_D(AnimatedSprite);

    if (!d->region.texture)
        return;

    lock();
    boost::shared_ptr<AnimatedTexture> texture = boost::static_pointer_cast<AnimatedTexture>(d->region.texture);

    if (d->animationStarted)
    {
        while (texture->time(d->currentFrame) && d->animationTime > (texture->time(d->currentFrame) / d->speed))
        {
            if (d->currentFrame == texture->frameCount() - 1 && d->singleShot)
            {
                stop();
                d->animationTime = 0;
                break;
            }
            d->animationTime -= texture->time(d->currentFrame) / d->speed;
            if ((texture->loadedFrames() == texture->frameCount()) || (d->currentFrame < texture->loadedFrames() - 1))
                d->currentFrame = (d->currentFrame + 1) % texture->frameCount();
        }

        d->animationTime += dt;
    }
    unlock();
}

void AnimatedSprite::draw() const
{
    RANGERS_D(const AnimatedSprite);
    if (!d->region.texture)
        return;

    boost::shared_ptr<AnimatedTexture> texture = boost::static_pointer_cast<AnimatedTexture>(d->region.texture);
    if (!texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, texture->openGLTexture(d->currentFrame));
    if (d->scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    if (d->scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    if (d->scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, d->vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

/*!
 * \returns current frame number
 */
int AnimatedSprite::currentFrame() const
{
    RANGERS_D(const AnimatedSprite);
    return d->currentFrame;
}

/*!
 * \returns current animation speed
 */
float AnimatedSprite::speed() const
{
    RANGERS_D(const AnimatedSprite);
    return d->speed;
}

/*!
 * \param f frame number
 */
void AnimatedSprite::setFrame(int f)
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->currentFrame = f;
    unlock();
}

/*!
 * \param f new speed. 1.0f - original speed, 2.0f - twice faster, etc.
 */
void AnimatedSprite::setSpeed(float f)
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->speed = f;
    unlock();
}

/*!
 * Set animation being played only once
 * \param ss animation is single shot
 */
void AnimatedSprite::setSingleShot(bool ss)
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->singleShot = ss;
    unlock();
}

/*!
 * \returns animation is single shot
 */
bool AnimatedSprite::isSingleShot() const
{
    RANGERS_D(const AnimatedSprite);
    return d->singleShot;
}

/*!
 * \returns animation is playing
 */
bool AnimatedSprite::isStarted() const
{
    RANGERS_D(const AnimatedSprite);
    return d->animationStarted;
}

int AnimatedSprite::frameCount() const
{
    RANGERS_D(const AnimatedSprite);
    boost::shared_ptr<AnimatedTexture> texture = boost::static_pointer_cast<AnimatedTexture>(d->region.texture);
    if (!texture)
        return 0;
    return texture->frameCount();
}

void AnimatedSprite::start()
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->animationStarted = true;
    unlock();
}

void AnimatedSprite::stop()
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->animationStarted = false;
    unlock();
}

void AnimatedSprite::reset()
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->animationStarted = false;
    d->currentFrame = 0;
    d->animationTime = 0;
    unlock();
}
}
