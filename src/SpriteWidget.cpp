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

#include "OpenSR/SpriteWidget.h"
#include "OpenSR/private/SpriteWidget_p.h"

#include "OpenSR/Sprite.h"

namespace Rangers
{
SpriteWidgetPrivate::SpriteWidgetPrivate(): WidgetPrivate()
{
}

SpriteWidget::SpriteWidget(boost::shared_ptr<Sprite> sprite, Widget* parent): Widget(*(new SpriteWidgetPrivate()), parent)
{
    RANGERS_D(SpriteWidget);
    d->sprite = sprite;
    d->width = sprite->width();
    d->height = sprite->height();
    addChild(d->sprite);
}

SpriteWidget::SpriteWidget(SpriteWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
}

SpriteWidget::~SpriteWidget()
{
    RANGERS_D(SpriteWidget);
}

void SpriteWidget::draw() const
{
    if (!prepareDraw())
        return;

    RANGERS_D(const SpriteWidget);

    if (d->sprite)
        d->sprite->draw();

    endDraw();
}

void SpriteWidget::processMain()
{
    RANGERS_D(SpriteWidget);
    if (d->sprite)
    {
        d->sprite->setGeometry(d->width, d->height);
    }
}

void SpriteWidget::processLogic(int dt)
{
    RANGERS_D(SpriteWidget);
    if (d->sprite)
    {
        d->sprite->processLogic(dt);
    }
}
}