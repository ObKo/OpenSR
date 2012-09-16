/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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
#include "ResourceManager.h"
#include "AnimatedTexture.h"

#include "private/AnimatedSprite_p.h"

namespace Rangers
{
AnimatedSpritePrivate::AnimatedSpritePrivate(): SpritePrivate()
{
    m_animationTime = 0;
    m_currentFrame = 0;
    m_singleShot = false;
    m_animationStarted = false;
    m_frameDuration = 0;
    m_region = TextureRegion(boost::shared_ptr<AnimatedTexture>());
}
/*!
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(Object *parent): Sprite(*(new AnimatedSpritePrivate()), parent)
{
}

/*!
 * \param texture texture
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(boost::shared_ptr<AnimatedTexture> texture, Object *parent): Sprite(*(new AnimatedSpritePrivate()), parent)
{
    RANGERS_D(AnimatedSprite);
    d->m_region = TextureRegion(texture);
    if (texture)
    {
        d->m_animationStarted = true;
//TODO: Find relations between seek/size and FPS.
        d->m_frameDuration = texture->waitSeek() > 1000 ? 50 : 100;
    }
}

/*!
 * \param animation animation resource name
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(const std::wstring& animation, Object *parent): Sprite(*(new AnimatedSpritePrivate()), parent)
{
    RANGERS_D(AnimatedSprite);
    boost::shared_ptr<AnimatedTexture> animTexture = ResourceManager::instance()->loadAnimation(animation);
    d->m_region = TextureRegion(animTexture);
    if (animTexture)
    {
        d->m_animationStarted = true;
//TODO: Find relations between seek/size and FPS.
        d->m_frameDuration = animTexture->waitSeek() > 1000 ? 50 : 100;
        d->m_width = animTexture->width();
        d->m_height = animTexture->height();
    }
    markToUpdate();
}

AnimatedSprite::AnimatedSprite(const Rangers::AnimatedSprite& other): Sprite(*(new AnimatedSpritePrivate()), other)
{
    RANGERS_D(AnimatedSprite);
    d->m_animationTime = other.d_func()->m_animationTime;
    d->m_currentFrame = other.d_func()->m_currentFrame;
    d->m_singleShot = other.d_func()->m_singleShot;
    d->m_animationStarted = other.d_func()->m_animationStarted;
    d->m_frameDuration = other.d_func()->m_frameDuration;
}

AnimatedSprite& AnimatedSprite::operator=(const Rangers::AnimatedSprite& other)
{
    RANGERS_D(AnimatedSprite);
    if (this == &other)
        return *this;

    d->m_animationTime = other.d_func()->m_animationTime;
    d->m_currentFrame = other.d_func()->m_currentFrame;
    d->m_singleShot = other.d_func()->m_singleShot;
    d->m_animationStarted = other.d_func()->m_animationStarted;
    d->m_currentFrame = other.d_func()->m_currentFrame;
    d->m_frameDuration = other.d_func()->m_frameDuration;

    Sprite::operator=(other);
    return *this;
}

AnimatedSprite::AnimatedSprite(AnimatedSpritePrivate &p, Object *parent): Sprite(p, parent)
{
    RANGERS_D(AnimatedSprite);
    d->m_animationTime = 0;
    d->m_currentFrame = 0;
    d->m_singleShot = false;
    d->m_animationStarted = false;
    d->m_frameDuration = 0;
    d->m_region = TextureRegion(boost::shared_ptr<AnimatedTexture>());
}

AnimatedSprite::AnimatedSprite(AnimatedSpritePrivate &p, const AnimatedSprite& other): Sprite(p, other)
{
    RANGERS_D(AnimatedSprite);
    d->m_animationTime = other.d_func()->m_animationTime;
    d->m_currentFrame = other.d_func()->m_currentFrame;
    d->m_singleShot = other.d_func()->m_singleShot;
    d->m_animationStarted = other.d_func()->m_animationStarted;
    d->m_frameDuration = other.d_func()->m_frameDuration;
}

void AnimatedSprite::processLogic(int dt)
{
    RANGERS_D(AnimatedSprite);
    if (!d->m_region.texture)
        return;

    lock();
    AnimatedTexture *texture = static_cast<AnimatedTexture *>(d->m_region.texture.get());

    if (d->m_animationStarted)
    {
        while (d->m_animationTime > d->m_frameDuration)
        {
            if ((texture->loadedFrames() == texture->frameCount()) || (d->m_currentFrame < texture->loadedFrames() - 1))
                d->m_currentFrame = (d->m_currentFrame + 1) % texture->frameCount();
            d->m_animationTime -= d->m_frameDuration;
        }

        d->m_animationTime += dt;
    }
    unlock();
}

void AnimatedSprite::draw() const
{
    RANGERS_D(const AnimatedSprite);
    if (!d->m_region.texture)
        return;

    GLuint texture = ((AnimatedTexture*)d->m_region.texture.get())->openGLTexture(d->m_currentFrame);
    if (!texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, texture);
    if (d->m_scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    if (d->m_scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    if (d->m_scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, d->m_vertexCount);

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
    return d->m_currentFrame;
}

/*!
 * \returns current animation frame
 */
float AnimatedSprite::frameRate() const
{
    RANGERS_D(const AnimatedSprite);
    return 1000.0f / d->m_frameDuration;
}

/*!
 * \param f frame number
 */
void AnimatedSprite::setFrame(int f)
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->m_currentFrame = f;
    unlock();
}

/*!
 * \param f new framerate
 */
void AnimatedSprite::setFrameRate(float f)
{
    lock();
    RANGERS_D(AnimatedSprite);
    if (f <= 0.0f)
        d->m_frameDuration = INT_MAX;
    d->m_frameDuration = 1000.0f / f;
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
    d->m_singleShot = ss;
    unlock();
}

/*!
 * \returns animation is single shot
 */
bool AnimatedSprite::isSingleShot() const
{
    RANGERS_D(const AnimatedSprite);
    return d->m_singleShot;
}

/*!
 * \returns animation is playing
 */
bool AnimatedSprite::isStarted() const
{
    RANGERS_D(const AnimatedSprite);
    return d->m_animationStarted;
}

void AnimatedSprite::start()
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->m_animationStarted = true;
    unlock();
}

void AnimatedSprite::stop()
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->m_animationStarted = false;
    unlock();
}

void AnimatedSprite::reset()
{
    lock();
    RANGERS_D(AnimatedSprite);
    d->m_animationStarted = false;
    d->m_currentFrame = 0;
    d->m_animationTime = 0;
    unlock();
}
}
