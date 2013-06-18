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

#include "SystemWidget.h"
#include "SystemPlanetWidget.h"
#include "SolarSystem.h"
#include "Planet.h"
#include "WorldManager.h"
#include "SpaceInfoWidget.h"

#include <cmath>
#include <OpenSR/AnimatedSprite.h>
#include <OpenSR/Sprite.h>
#include <OpenSR/Engine.h>
#include <OpenSR/ActionListener.h>
#include <OpenSR/Action.h>
#include <OpenSR/Log.h>
#include <OpenSR/WidgetNode.h>
#include <OpenSR/Button.h>
#include <OpenSR/AnimatedTexture.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/SpriteWidget.h>

namespace Rangers
{
namespace World
{
class SystemWidget::SystemWidgetListener: public ActionListener
{
public:
    SystemWidgetListener(SystemWidget *parent): m_parent(parent)
    {
    }

    virtual void actionPerformed(const Action &action)
    {
        if (action.type() == Action::MOUSE_LEAVE)
        {
            m_parent->m_infoWidget->clear();
            m_parent->m_infoWidget->setVisible(false);
        }
        if (action.type() == Action::MOUSE_ENTER)
        {
            boost::shared_ptr<SystemPlanetWidget> w = boost::dynamic_pointer_cast<SystemPlanetWidget>(action.source());
            if (w)
            {
                m_parent->m_infoWidget->showPlanet(w->planet());
                m_parent->m_infoWidget->setVisible(true);
            }
            else if (action.source() == m_parent->m_starWidget)
            {
                m_parent->m_infoWidget->showSystem(m_parent->m_system);
                m_parent->m_infoWidget->setVisible(true);
            }
        }
    }

private:
    SystemWidget *m_parent;
};

SystemWidget::SystemWidget(boost::shared_ptr<SolarSystem> system, Widget* parent): Widget(parent),
    m_xOffset(0), m_yOffset(0), m_moveDirection(NONE)
{
    setWidth(Engine::instance().screenWidth());
    setHeight(Engine::instance().screenHeight());
    setPosition(0, 0);

    m_actionListener = boost::shared_ptr<SystemWidgetListener>(new SystemWidgetListener(this));

    m_node = boost::shared_ptr<WidgetNode>(new WidgetNode());
    addWidget(m_node);

    m_infoWidget = boost::shared_ptr<SpaceInfoWidget>(new SpaceInfoWidget(WorldManager::instance().styleManager().infoWidgetStyle()));
    m_infoWidget->setPosition(10, 10);
    m_infoWidget->setWidth(250);
    m_infoWidget->setHeight(200);
    m_infoWidget->setVisible(false);
    addWidget(m_infoWidget);

    setSystem(system);
}

SystemWidget::~SystemWidget()
{
    /*std::list<boost::shared_ptr<SystemPlanetWidget> >::const_iterator end = m_planetWidgets.end();
    for (std::list<boost::shared_ptr<SystemPlanetWidget> >::const_iterator i = m_planetWidgets.begin(); i != end; ++i)
    {
        m_node->removeWidget(*i);
    }
    m_planetWidgets.clear();*/
}

void SystemWidget::processLogic(int dt)
{
    Widget::processLogic(dt);

    m_node->processLogic(dt);

    switch (m_moveDirection)
    {
    case TOP:
        m_yOffset += (500.0f * dt) / 1000.0f;
        break;
    case BOTTOM:
        m_yOffset -= (500.0f * dt) / 1000.0f;
        break;
    case LEFT:
        m_xOffset += (500.0f * dt) / 1000.0f;
        break;
    case RIGHT:
        m_xOffset -= (500.0f * dt) / 1000.0f;
        break;
    case TOP_LEFT:
        m_yOffset += (500.0f * dt) / 1000.0f;
        m_xOffset += (500.0f * dt) / 1000.0f;
        break;
    case TOP_RIGHT:
        m_yOffset += (500.0f * dt) / 1000.0f;
        m_xOffset -= (500.0f * dt) / 1000.0f;
        break;
    case BOTTOM_LEFT:
        m_yOffset -= (500.0f * dt) / 1000.0f;
        m_xOffset += (500.0f * dt) / 1000.0f;
        break;
    case BOTTOM_RIGHT:
        m_yOffset -= (500.0f * dt) / 1000.0f;
        m_xOffset -= (500.0f * dt) / 1000.0f;
        break;
    default:
        break;
    }
    m_bgSprite->setPosition(m_xOffset / 10 - (m_bgSprite->width() - width()) / 2, m_yOffset / 10 - (m_bgSprite->height() - height()) / 2);
    m_node->setPosition(m_xOffset + width() / 2, m_yOffset + height() / 2);
}

void SystemWidget::setSystem(boost::shared_ptr< SolarSystem > system)
{
    if (!system || system != m_system)
    {
        removeWidget(m_starWidget);
        removeChild(m_bgSprite);
        m_starWidget = boost::shared_ptr<SpriteWidget>();
        m_bgSprite = boost::shared_ptr<Sprite>();
    }
    m_system = system;

    std::list<boost::shared_ptr<SystemPlanetWidget> >::const_iterator wend = m_planetWidgets.end();
    for (std::list<boost::shared_ptr<SystemPlanetWidget> >::const_iterator i = m_planetWidgets.begin(); i != wend; ++i)
    {
        removeWidget(*i);
    }
    m_planetWidgets.clear();

    if (!m_system)
        return;

    std::list<boost::shared_ptr<SystemObject> > objects = system->systemObjects();

    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = objects.end();
    for (std::list<boost::shared_ptr<SystemObject> >::const_iterator i = objects.begin(); i != end; ++i)
    {
        boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(*i);
        if (planet)
        {
            boost::shared_ptr<SystemPlanetWidget> w = boost::shared_ptr<SystemPlanetWidget>(new SystemPlanetWidget(planet));
            w->addListener(m_actionListener);

            m_planetWidgets.push_back(w);
            m_node->addWidget(w);
        }
    }

    m_starWidget = boost::shared_ptr<SpriteWidget>(new SpriteWidget(boost::shared_ptr<Sprite>(new AnimatedSprite(L"DATA/Star/Star00.gai"))));
    m_starWidget->setPosition(-m_starWidget->width() / 2, -m_starWidget->height() / 2);
    m_starWidget->addListener(m_actionListener);
    m_node->addWidget(m_starWidget);

    m_bgSprite = boost::shared_ptr<Sprite>(new AnimatedSprite(L"DATA/BGObj/bg00.gai"));
    m_bgSprite->setPosition(m_xOffset / 10 - (m_bgSprite->width() - width()) / 2, m_yOffset / 10 - (m_bgSprite->height() - height()) / 2);
    addChild(m_bgSprite);
}

boost::shared_ptr< SolarSystem > SystemWidget::system() const
{
    return m_system;
}


void SystemWidget::processMain()
{
    Widget::processMain();
}

void SystemWidget::mouseMove(const Vector &p)
{
    if (p.x > width() - 15)
    {
        if (p.y < 15)
            m_moveDirection = TOP_RIGHT;
        else if (p.y > height() - 15)
            m_moveDirection = BOTTOM_RIGHT;
        else
            m_moveDirection = RIGHT;
    }
    else if (p.x < 15)
    {
        if (p.y < 15)
            m_moveDirection = TOP_LEFT;
        else if (p.y > height() - 15)
            m_moveDirection = BOTTOM_LEFT;
        else
            m_moveDirection = LEFT;
    }
    else
    {
        if (p.y < 15)
            m_moveDirection = TOP;
        else if (p.y > height() - 15)
            m_moveDirection = BOTTOM;
        else
            m_moveDirection = NONE;
    }
    Widget::mouseMove(p);
}

void SystemWidget::draw() const
{
    if (!prepareDraw())
        return;

    m_bgSprite->draw();
    m_node->draw();
    m_infoWidget->draw();

    endDraw();
}
}
}