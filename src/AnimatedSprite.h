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

#ifndef RANGERS_ANIMATEDSPRITE_H
#define RANGERS_ANIMATEDSPRITE_H

#include "Sprite.h"
#include "AnimatedTexture.h"

namespace Rangers
{
//! Animated sprite using animated texture
class AnimatedSprite: public Sprite
{
public:
    //! Create sprite from animated texture
    AnimatedSprite(boost::shared_ptr<AnimatedTexture> texture,  Object *parent = 0);
    AnimatedSprite(const AnimatedSprite& other);

    void processLogic(int dt);
    void draw() const;
    
    void setSingleShot(bool ss);
    void start();
    void stop();
    
    bool isStarted() const;
    bool isSingleShot() const;
    
    int currentFrame() const;
    float fps() const;
    void setFrame(int f);
    void setFPS(float f);
    
    AnimatedSprite& operator=(const AnimatedSprite& other);

private:
    int t;
    int animFrame;
    int frameTime;
    bool singleShot;
    bool started;
};
};

#endif
