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

#include "SpaceObjectWidget.h"
#include "SpaceObject.h"

#include "Asteroid.h"
#include "WorldManager.h"

#include <OpenSR/Sprite.h>
#include <OpenSR/AnimatedSprite.h>

namespace Rangers
{
namespace World
{
SpaceObjectWidget::SpaceObjectWidget(boost::shared_ptr< SpaceObject > object): SpriteWidget()
{
    setObject(object);
}

SpaceObjectWidget::~SpaceObjectWidget()
{

}

boost::shared_ptr<SpaceObject> SpaceObjectWidget::object() const
{
    return m_object;
}

void SpaceObjectWidget::processLogic(int dt)
{
    SpriteWidget::processLogic(dt);
    updatePosition();
}

void SpaceObjectWidget::setObject(boost::shared_ptr< SpaceObject > object)
{
    m_object = object;
    if (!m_object)
        return;

    if (boost::shared_ptr<Asteroid> a = boost::dynamic_pointer_cast<Asteroid>(object))
    {
        boost::shared_ptr<AsteroidStyle> s = WorldManager::instance().styleManager().asteroidStyle(a->style());

        if (s->animated)
            setSprite(boost::shared_ptr<Sprite>(new AnimatedSprite(s->sprite)));
        else
            setSprite(boost::shared_ptr<Sprite>(new Sprite(s->sprite)));
    }

    updatePosition();
}

void SpaceObjectWidget::updatePosition()
{
    if (!m_object)
        return;
    boost::shared_ptr<Sprite> s = sprite();
    float dx = 0.0f, dy = 0.0f;
    if (s)
    {
        dx = s->width() / 2.0f;
        dy = s->height() / 2.0f;
    }
    Point p = m_object->position();
    setPosition(p.x - dx, p.y - dy);
}
}
}