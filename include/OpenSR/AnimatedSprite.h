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

#ifndef RANGERS_ANIMATEDSPRITE_H
#define RANGERS_ANIMATEDSPRITE_H

#include "Sprite.h"

namespace Rangers
{
class AnimatedSprite;
class AnimatedTexture;
class AnimatedSpritePrivate;

//! Animated sprite using animated texture
/*!
 * Represents animation with adjustable frame rate.
 * Animation can be single shot or continuous playing.
 */

class RANGERS_ENGINE_API AnimatedSprite: public Sprite
{
    RANGERS_DECLARE_PRIVATE(AnimatedSprite)
public:
    AnimatedSprite(Object *parent = 0);
    //! Create sprite from animated texture
    AnimatedSprite(boost::shared_ptr<AnimatedTexture> texture,  Object *parent = 0);
    //! Create sprite from resource name
    AnimatedSprite(const std::wstring& animation,  Object *parent = 0);

    virtual void processLogic(int dt);
    virtual void draw() const;

    //! Set animation single shot
    void setSingleShot(bool ss);
    //! Start animation
    void start();
    //! Stop animation
    void stop();
    //! Reset animation
    void reset();

    //! Animation is playing
    bool isStarted() const;
    //! Animation is single shot
    bool isSingleShot() const;

    //! Current animation frame
    int currentFrame() const;
    //! Animation frames per second
    float frameRate() const;
    //! Set current frame
    void setFrame(int f);
    //! Set animation frame rate
    void setFrameRate(float f);

protected:
    AnimatedSprite(AnimatedSpritePrivate &p, Object *parent = 0);

    RANGERS_DISABLE_COPY(AnimatedSprite)
};
}

#endif
