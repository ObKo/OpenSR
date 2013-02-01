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
        Log::debug() << "KOKOKO";
        if (action.type() == Action::MOUSE_LEAVE)
            m_parent->m_infoWidget->clear();
        if (action.type() == Action::MOUSE_ENTER)
        {
            SystemPlanetWidget *w = dynamic_cast<SystemPlanetWidget*>(action.source());
            if (w)
            {
                m_parent->m_infoWidget->showPlanet(w->planet());
            }
        }
    }

private:
    SystemWidget *m_parent;
};

SystemWidget::SystemWidget(boost::shared_ptr<SolarSystem> system, Widget* parent): Widget(parent),
    m_xOffset(0), m_yOffset(0), m_bgSprite(0), m_starSprite(0), m_infoWidget(0), m_moveDirection(NONE)
{
    setSystem(system);

    setWidth(Engine::instance().screenWidth());
    setHeight(Engine::instance().screenHeight());

    m_actionListener = new SystemWidgetListener(this);
    setPosition(0, 0);
    m_infoWidget = new SpaceInfoWidget(WorldManager::instance().styleManager().infoWidgetStyle(), this);
    m_infoWidget->setPosition(10, 10);
    m_infoWidget->setWidth(200);
    m_infoWidget->setHeight(150);
}

SystemWidget::~SystemWidget()
{
    std::list<SystemPlanetWidget*>::const_iterator end = m_planetWidgets.end();
    for (std::list<SystemPlanetWidget*>::const_iterator i = m_planetWidgets.begin(); i != end; ++i)
    {
        delete *i;
    }
    m_planetWidgets.clear();

    delete m_starSprite;
    delete m_bgSprite;
    delete m_infoWidget;
    delete m_actionListener;
}

void SystemWidget::processLogic(int dt)
{
    Widget::processLogic(dt);
    m_starSprite->processLogic(dt);

    std::list<SystemPlanetWidget*>::const_iterator end = m_planetWidgets.end();
    for (std::list<SystemPlanetWidget*>::const_iterator i = m_planetWidgets.begin(); i != end; ++i)
        (*i)->processLogic(dt);

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
}

void SystemWidget::setSystem(boost::shared_ptr< SolarSystem > system)
{
    if (!system || system != m_system)
    {
        delete m_bgSprite;
        delete m_starSprite;
        m_starSprite = 0;
        m_bgSprite = 0;
    }
    m_system = system;

    std::list<SystemPlanetWidget*>::const_iterator wend = m_planetWidgets.end();
    for (std::list<SystemPlanetWidget*>::const_iterator i = m_planetWidgets.begin(); i != wend; ++i)
    {
        delete *i;
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
            SystemPlanetWidget *w = new SystemPlanetWidget(planet, this);
            w->setPosition(planet->position().x, planet->position().y);
            w->addListener(m_actionListener);
            m_planetWidgets.push_back(w);
        }
    }

    m_starSprite = new AnimatedSprite(L"DATA/Star/Star00.gai");
    m_starSprite->setPosition(-m_starSprite->width() / 2, -m_starSprite->height() / 2);
    m_bgSprite = new AnimatedSprite(L"DATA/BGObj/bg00.gai");
    m_bgSprite->setPosition(m_xOffset / 10 - (m_bgSprite->width() - width()) / 2, m_yOffset / 10 - (m_bgSprite->height() - height()) / 2);
}

boost::shared_ptr< SolarSystem > SystemWidget::system() const
{
    return m_system;
}


void SystemWidget::processMain()
{
    Widget::processMain();
    m_starSprite->processMain();
    m_bgSprite->processMain();
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
}

void SystemWidget::draw() const
{
    if (!prepareDraw())
        return;

    m_bgSprite->draw();

    glPushMatrix();
    glTranslatef(floor(width() / 2.0f + m_xOffset), floor(height() / 2.0f + m_yOffset), 0);

    m_starSprite->draw();

    std::list<SystemPlanetWidget*>::const_iterator end = m_planetWidgets.end();
    for (std::list<SystemPlanetWidget*>::const_iterator i = m_planetWidgets.begin(); i != end; ++i)
        (*i)->draw();

    glPopMatrix();

    m_infoWidget->draw();

    endDraw();
}
}
}