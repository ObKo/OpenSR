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
#include <libintl.h>
#include <libRanger.h>

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

    m_caption = new Label(L"", this, m_captionFont);
    m_caption->setOrigin(POSITION_X_CENTER, POSITION_Y_TOP);

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
    int xOffset;
    int yOffset;
    if (m_contentRect.valid() && m_bgSprite)
    {
        xOffset = (m_bgSprite->width() - m_bgSprite->normalWidth() + m_contentRect.width) / 2 + m_contentRect.x;
        yOffset = m_contentRect.y;
    }
    m_caption->setPosition((int(m_caption->width()) % 2) / 2.0f + xOffset, yOffset + 5);
}

void SpaceInfoWidget::draw() const
{
    if (!prepareDraw())
        return;

    if (m_bgSprite)
        m_bgSprite->draw();

    m_caption->draw();

    endDraw();
}

void SpaceInfoWidget::clear()
{
    m_caption->setText(L"");
    markToUpdate();
}

void SpaceInfoWidget::showPlanet(boost::shared_ptr<Planet> planet)
{
    m_caption->setText(fromLocal((dgettext("OpenSR-World", "Planet") + std::string(" ") + dgettext("OpenSR-World", planet->name().c_str())).c_str()));
    markToUpdate();
}

}
}