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

#ifndef RANGERS_GAISPRITE_H
#define RANGERS_GAISPRITE_H

#include "AnimatedSprite.h"

namespace Rangers
{
class GAISpritePrivate;
struct GIFrame;
class RANGERS_ENGINE_API GAISprite: public AnimatedSprite
{
    RANGERS_DECLARE_PRIVATE(GAISprite)
public:
    GAISprite(Object *parent = 0);
    GAISprite(const char *data, int size, const GIFrame& baseFrame, Object *parent = 0);
    GAISprite(const std::wstring& name, Object *parent = 0);
    GAISprite(const GAISprite &other);
    virtual ~GAISprite();

    virtual void draw() const;
    virtual void processLogic(int dt);
    virtual void processMain();
    void setFrame(int f);

    void reset();

    GAISprite& operator=(const GAISprite& other);
    
protected:
    GAISprite(GAISpritePrivate &p, Object *parent = 0);
    GAISprite(GAISpritePrivate &p, const GAISprite& other);

private:
    void drawFrame(int i);
    void loadGIFrame5(const char *data, unsigned char *background, int startX, int startY, int finishX, int finishY);
    void loadGAI(const char *data, int size, const GIFrame& baseFrame);
};

}

#endif /* GAISPRITE_H_ */
