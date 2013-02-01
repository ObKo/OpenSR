/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_SPRITEWIDGET_H
#define RANGERS_SPRITEWIDGET_H

#include "Widget.h"

namespace Rangers
{
class Sprite;
class SpriteWidgetPrivate;
class RANGERS_ENGINE_API SpriteWidget: public Widget
{
    RANGERS_DECLARE_PRIVATE(SpriteWidget)
public:
    SpriteWidget(Sprite *sprite, Widget *parent = 0);
    virtual ~SpriteWidget();

    virtual void draw() const;
    virtual void processMain();
    virtual void processLogic(int dt);

protected:
    SpriteWidget(SpriteWidgetPrivate &p, Widget *parent = 0);

    RANGERS_DISABLE_COPY(SpriteWidget)
};
}


#endif // RANGERS_SPRITEWIDGET_H
