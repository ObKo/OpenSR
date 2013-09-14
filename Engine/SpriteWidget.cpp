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
SpriteWidgetPrivate::SpriteWidgetPrivate(): WidgetPrivate(), shape(SpriteWidget::SHAPE_RECT)
{
}

SpriteWidget::SpriteWidget(boost::shared_ptr<Sprite> sprite):
    Widget(*(new SpriteWidgetPrivate()))
{
    RANGERS_D(SpriteWidget);
    d->sprite = sprite;
    d->width = sprite->width();
    d->height = sprite->height();
    addChild(d->sprite);
}

SpriteWidget::SpriteWidget(SpriteWidgetPrivate &p): Widget(p)
{
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

bool SpriteWidget::containsPoint(const Vector &p) const
{
    RANGERS_D(const SpriteWidget);
    float x, y, size;
    switch (d->shape)
    {
    case SHAPE_RECT:
        if ((p.x >= 0) && (p.x <= d->width) && (p.y >= 0) && (p.y <= d->height))
            return true;
        break;
    case SHAPE_CIRCLE:
        size = std::min(d->width, d->height) / 2.0;
        x = p.x - d->width / 2.0f;
        y = p.y - d->height / 2.0f;
        return (x * x) + (y * y) < size * size;
        break;
    default:
        return false;
        break;
    }
    return false;
}

SpriteWidget::Shape SpriteWidget::shape() const
{
    RANGERS_D(const SpriteWidget);
    return d->shape;
}

void SpriteWidget::setShape(SpriteWidget::Shape s)
{
    RANGERS_D(SpriteWidget);
    d->shape = s;
}

boost::shared_ptr<Sprite> SpriteWidget::sprite() const
{
    RANGERS_D(const SpriteWidget);
    return d->sprite;
}

void SpriteWidget::setSprite(boost::shared_ptr<Sprite> sprite)
{
    RANGERS_D(SpriteWidget);
    d->sprite = sprite;
}
}