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
#include <string>
#include "Engine.h"
#include "ResourceManager.h"
#include "AnimatedTexture.h"

namespace Rangers
{
/*!
 * \param texture texture
 * \param parent object parent
 */
AnimatedSprite::AnimatedSprite(Object *parent): Sprite(parent)
{
    m_animationTime = 0;
    m_currentFrame = 0;
    m_singleShot = false;
    m_animationStarted = false;
    m_frameDuration = 0;
    m_texture = boost::shared_ptr<AnimatedTexture>();
}

AnimatedSprite::AnimatedSprite(boost::shared_ptr<AnimatedTexture> texture,  Object *parent): Sprite(texture, parent)
{
    m_animationTime = 0;
    m_currentFrame = 0;
    m_singleShot = false;
    if (!texture)
    {
        m_animationStarted = false;
        m_frameDuration = 0;
    }
    else
    {
        m_animationStarted = true;
//TODO: Find relations between seek/size and FPS.
        m_frameDuration = texture->waitSeek() > 1000 ? 50 : 100;
    }
}

AnimatedSprite::AnimatedSprite(const std::wstring& animation,  Object *parent): Sprite(parent)
{
    m_animationTime = 0;
    m_currentFrame = 0;
    m_singleShot = false;
    boost::shared_ptr<AnimatedTexture> animTexture = ResourceManager::instance()->loadAnimation(animation);
    if (!animTexture)
    {
        m_animationStarted = false;
        m_frameDuration = 0;
    }
    else
    {
        m_texture = animTexture;
        m_animationStarted = true;
//TODO: Find relations between seek/size and FPS.
        m_frameDuration = animTexture->waitSeek() > 1000 ? 50 : 100;
        m_width = animTexture->width();
        m_height = animTexture->height();
    }
    markToUpdate();
}

AnimatedSprite::AnimatedSprite(const Rangers::AnimatedSprite& other): Sprite(other)
{
    m_animationTime = other.m_animationTime;
    m_currentFrame = other.m_currentFrame;
    m_singleShot = other.m_singleShot;
    m_animationStarted = other.m_animationStarted;
    m_frameDuration = other.m_frameDuration;
}

AnimatedSprite& AnimatedSprite::operator=(const Rangers::AnimatedSprite& other)
{
    if (this == &other)
        return *this;

    m_animationTime = other.m_animationTime;
    m_currentFrame = other.m_currentFrame;
    m_singleShot = other.m_singleShot;
    m_animationStarted = other.m_animationStarted;
    m_currentFrame = other.m_currentFrame;
    m_frameDuration = other.m_frameDuration;

    Sprite::operator=(other);
    return *this;
}



void AnimatedSprite::processLogic(int dt)
{
    if (!m_texture)
        return;

    lock();
    AnimatedTexture *texture = static_cast<AnimatedTexture *>(m_texture.get());

    if (m_animationStarted)
    {
        while (m_animationTime > m_frameDuration)
        {
            if ((texture->loadedFrames() == texture->frameCount()) || (m_currentFrame < texture->loadedFrames() - 1))
                m_currentFrame = (m_currentFrame + 1) % texture->frameCount();
            m_animationTime -= m_frameDuration;
        }

        m_animationTime += dt;
    }
    unlock();
}

void AnimatedSprite::draw() const
{
    if (!m_texture)
        return;

    GLuint texture = ((AnimatedTexture*)m_texture.get())->openGLTexture(m_currentFrame);
    if (!texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, texture);
    if (m_scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    if (m_scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    if (m_scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, m_vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

int AnimatedSprite::currentFrame() const
{
    return m_currentFrame;
}

float AnimatedSprite::frameRate() const
{
    return 1000.0f / m_frameDuration;
}

void AnimatedSprite::setFrame(int f)
{
    lock();
    m_currentFrame = f;
    unlock();
}

void AnimatedSprite::setFrameRate(float f)
{
    lock();
    if (f <= 0.0f)
        m_frameDuration = INT_MAX;
    m_frameDuration = 1000.0f / f;
    unlock();
}

void AnimatedSprite::setSingleShot(bool ss)
{
    lock();
    m_singleShot = ss;
    unlock();
}

bool AnimatedSprite::isSingleShot() const
{
    return m_singleShot;
}

bool AnimatedSprite::isStarted() const
{
    return m_animationStarted;
}

void AnimatedSprite::start()
{
    lock();
    m_animationStarted = true;
    unlock();
}

void AnimatedSprite::stop()
{
    lock();
    m_animationStarted = false;
    unlock();
}

void AnimatedSprite::reset()
{
    lock();
    m_animationStarted = false;
    m_currentFrame = 0;
    m_animationTime = 0;
    unlock();
}
}
