/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_SYSTEMWIDGET_H
#define WORLD_SYSTEMWIDGET_H

#include <OpenSR/Widget.h>

namespace Rangers
{
class Sprite;
namespace World
{
class SystemPlanetWidget;
class SolarSystem;
class SystemWidget: public Widget
{
public:
    SystemWidget(boost::shared_ptr<SolarSystem> system = boost::shared_ptr<SolarSystem>(), Widget* parent = 0);
    virtual ~SystemWidget();

    virtual void draw() const;

    virtual void processMain();
    virtual void processLogic(int dt);

    virtual void mouseMove(const Vector &p);

    boost::shared_ptr<SolarSystem> system() const;

    void setSystem(boost::shared_ptr<SolarSystem> system);

private:
    enum MoveDirection {NONE, LEFT, RIGHT, TOP, BOTTOM, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};

    boost::shared_ptr<SolarSystem> m_system;
    std::list<SystemPlanetWidget*> m_planetWidgets;

    Sprite *m_bgSprite;
    Sprite *m_starSprite;
    float m_xOffset;
    float m_yOffset;
    MoveDirection m_moveDirection;
};
}
}

#endif
