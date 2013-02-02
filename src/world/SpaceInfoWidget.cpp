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

#include "SpaceInfoWidget.h"

#include <OpenSR/Sprite.h>
#include <OpenSR/NinePatch.h>
#include <OpenSR/Label.h>
#include <OpenSR/Font.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Engine.h>
#include <OpenSR/Log.h>
#include <OpenSR/Utils.h>
#include <libintl.h>
#include <libRanger.h>

#include "WorldStyleManager.h"
#include "Planet.h"
#include "SolarSystem.h"
#include "PlanetManager.h"

namespace Rangers
{
namespace World
{
SpaceInfoWidget::SpaceInfoWidget(const InfoWidgetStyle& style, Widget* parent): Widget(parent), m_bgSprite(0), m_iconSprite(0)
{
    if (style.background.type == ResourceDescriptor::SPRITE)
        m_bgSprite = new Sprite(boost::get<TextureRegionDescriptor>(style.background.resource), this);
    else if (style.background.type == ResourceDescriptor::NINEPATCH)
        m_bgSprite = new NinePatch(boost::get<NinePatchDescriptor>(style.background.resource), this);

    if (style.font.type == ResourceDescriptor::FONT)
    {
        FontDescriptor d = boost::get<FontDescriptor>(style.font.resource);
        m_font = ResourceManager::instance().loadFont(d.path, d.size);
    }
    if (!m_font)
        m_font = Engine::instance().coreFont();

    if (style.captionFont.type == ResourceDescriptor::FONT)
    {
        FontDescriptor d = boost::get<FontDescriptor>(style.captionFont.resource);
        m_captionFont = ResourceManager::instance().loadFont(d.path, d.size);
    }
    if (!m_captionFont)
        m_captionFont = Engine::instance().coreFont();

    m_captionColor = style.captionColor;

    m_caption = new Label(L"", this, m_captionFont);
    m_caption->setOrigin(POSITION_X_CENTER, POSITION_Y_TOP);
    m_caption->setColor(m_captionColor);

    m_contentRect = style.contentRect;

    if (m_bgSprite)
    {
        setWidth(m_bgSprite->normalWidth());
        setHeight(m_bgSprite->normalHeight());
    }
}

SpaceInfoWidget::~SpaceInfoWidget()
{
    if (m_bgSprite)
        delete m_bgSprite;
}

void SpaceInfoWidget::processMain()
{
    Widget::processMain();

    if (m_bgSprite)
    {
        m_bgSprite->setWidth(width());
        m_bgSprite->setHeight(height());
    }
    Rect realContentRect;
    if (m_contentRect.valid() && m_bgSprite)
    {
        realContentRect.x = m_contentRect.x;
        realContentRect.y = m_contentRect.y;
        realContentRect.width = (m_bgSprite->width() - m_bgSprite->normalWidth() + m_contentRect.width);
        realContentRect.height = (m_bgSprite->height() - m_bgSprite->normalHeight() + m_contentRect.height);
    }
    else
    {
        realContentRect.x = 0;
        realContentRect.y = 0;
        realContentRect.width = width();
        realContentRect.height = height();
    }
    if (m_iconSprite)
    {
        m_iconSprite->setPosition(realContentRect.x, realContentRect.y);
        m_caption->setPosition((int(m_caption->width()) % 2) / 2.0f + int(realContentRect.width) / 2 + realContentRect.x + int(m_iconSprite->width()) / 2,
                               realContentRect.y + int(m_iconSprite->height()) / 2 - int(m_caption->height()) / 2);
    }
    else
    {
        m_caption->setPosition((int(m_caption->width()) % 2) / 2.0f + int(realContentRect.width) / 2 + realContentRect.x,
                               realContentRect.y);
    }
}

Rect SpaceInfoWidget::getBoundingRect() const
{
    return Rect();
}

void SpaceInfoWidget::draw() const
{
    if (!prepareDraw())
        return;

    if (m_bgSprite)
        m_bgSprite->draw();

    m_caption->draw();

    if (m_iconSprite)
        m_iconSprite->draw();

    endDraw();
}

void SpaceInfoWidget::clear()
{
    m_caption->setText(L"");
    if (m_iconSprite)
    {
        delete m_iconSprite;
        m_iconSprite = 0;
    }
    markToUpdate();
}

void SpaceInfoWidget::showPlanet(boost::shared_ptr<Planet> planet)
{
    m_caption->setText(_("Planet", "OpenSR-World") + L" " + _(planet->name(), "OpenSR-World"));
    m_iconSprite = new Sprite(PlanetManager::instance().getPlanetImage(planet->style(), 48));
    markToUpdate();
}

void SpaceInfoWidget::showSystem(boost::shared_ptr<SolarSystem> system)
{
    m_caption->setText(_("System", "OpenSR-World") + L" " + _(system->name(), "OpenSR-World"));
    markToUpdate();
}
}
}