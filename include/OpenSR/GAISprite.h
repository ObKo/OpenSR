/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/AnimatedSprite.h"

namespace Rangers
{
class GAISpritePrivate;
struct GIFrame;
class RANGERS_ENGINE_API GAISprite: public AnimatedSprite
{
    RANGERS_DECLARE_PRIVATE(GAISprite)
public:
    GAISprite();
    GAISprite(const char *data, int size, const GIFrame& baseFrame);
    GAISprite(const std::string& name);

    virtual ~GAISprite();

    virtual void draw() const;
    virtual void processLogic(int dt);
    virtual void processMain();
    void setFrame(int f);

    void reset();

protected:
    GAISprite(GAISpritePrivate &p);

    RANGERS_DISABLE_COPY(GAISprite);
};

}

#endif /* GAISPRITE_H_ */
