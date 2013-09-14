/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/World/world-global.h"

#include <OpenSR/Widget.h>
#include <map>

namespace Rangers
{
class Sprite;
class WidgetNode;
class SpriteWidget;
class Button;

namespace World
{
class Asteroid;
class InfoWidget;
class SystemPlanetWidget;
class PlanetarySystem;

class RANGERS_WORLD_API SystemWidget: public Widget
{
public:
    SystemWidget(boost::shared_ptr<PlanetarySystem> system = boost::shared_ptr<PlanetarySystem>());

    virtual void draw() const;

    virtual void processMain();
    virtual void processLogic(int dt);

    virtual void mouseMove(const Vector &p);

    boost::shared_ptr<PlanetarySystem> system() const;

    void setSystem(boost::shared_ptr<PlanetarySystem> system);

private:
    enum MoveDirection {NONE, LEFT, RIGHT, TOP, BOTTOM, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};

    class SystemWidgetListener;
    boost::shared_ptr<PlanetarySystem> m_system;
    std::list<boost::shared_ptr<SystemPlanetWidget> > m_planetWidgets;
    //TODO: Use more complex widget than SpriteWidget
    std::map<boost::shared_ptr<Asteroid>, boost::shared_ptr<SpriteWidget> > m_asteroidsWidgets;

    boost::shared_ptr<Sprite> m_bgSprite;
    boost::shared_ptr<SpriteWidget> m_starWidget;
    float m_xOffset;
    float m_yOffset;
    MoveDirection m_moveDirection;

    boost::shared_ptr<InfoWidget> m_infoWidget;
    boost::shared_ptr<SystemWidgetListener> m_actionListener;
    boost::shared_ptr<WidgetNode> m_node;

    bool m_turn;
    int m_turnTime;

    //FIXME: Temp
    boost::shared_ptr<Button> m_turnButton;
};
}
}

#endif
