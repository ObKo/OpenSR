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

#include "SystemWidget.h"
#include <OpenSR/AnimatedSprite.h>
#include <OpenSR/Sprite.h>
#include <OpenSR/Engine.h>

namespace Rangers
{
namespace World
{
SystemWidget::SystemWidget(boost::shared_ptr<SolarSystem> system, Widget* parent): Widget(parent),
    m_xOffset(0), m_yOffset(0), m_bgSprite(0), m_starSprite(0), m_moveDirection(NONE)
{
    setSystem(system);

    setWidth(Engine::instance()->screenWidth());
    setHeight(Engine::instance()->screenHeight());

    setPosition(0, 0);
}

SystemWidget::~SystemWidget()
{
    delete m_starSprite;
    delete m_bgSprite;
}

void SystemWidget::processLogic(int dt)
{
    Widget::processLogic(dt);
    m_starSprite->processLogic(dt);
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

    if (!m_system)
        return;

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
    glTranslatef(width() / 2.0f + m_xOffset, height() / 2.0f + m_yOffset, 0);

    m_starSprite->draw();

    glPopMatrix();

    endDraw();
}
}
}