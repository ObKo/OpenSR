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

#include "WorldStyleManager.h"
#include "Planet.h"

namespace Rangers
{
namespace World
{
SpaceInfoWidget::SpaceInfoWidget(const InfoWidgetStyle& style, Widget* parent): Widget(parent), m_bgSprite(0)
{
    if (style.background.type == ResourceDescriptor::SPRITE)
        m_bgSprite = new Sprite(boost::get<TextureRegionDescriptor>(style.background.resource), this);
    else if (style.background.type == ResourceDescriptor::NINEPATCH)
        m_bgSprite = new NinePatch(boost::get<NinePatchDescriptor>(style.background.resource), this);

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
    if (m_bgSprite)
    {
        m_bgSprite->setWidth(width());
        m_bgSprite->setHeight(height());
    }
}

void SpaceInfoWidget::draw() const
{
    if (!prepareDraw())
        return;

    if (m_bgSprite)
        m_bgSprite->draw();

    endDraw();
}

void SpaceInfoWidget::clear()
{
    setColor(0xffffffff);
}

void SpaceInfoWidget::showPlanet(boost::shared_ptr<Planet> planet)
{
    setColor(0xff0000ff);
}

}
}