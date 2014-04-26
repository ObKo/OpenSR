/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

#include "InfoWidget.h"

#include <OpenSR/Sprite.h>
#include <OpenSR/NinePatch.h>
#include <OpenSR/Label.h>
#include <OpenSR/Font.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Engine.h>

#include "OpenSR/World/InfoWidgetProvider.h"
#include "OpenSR/World/SkinManager.h"
#include "OpenSR/World/WorldObject.h"
#include "OpenSR/World/WorldManager.h"
#include "OpenSR/World/WorldHelper.h"

namespace
{
const int SMALL_ICON_SIZE = 16;
}

namespace Rangers
{
namespace World
{
InfoWidget::InfoWidget(boost::shared_ptr<InfoWidgetStyle> style): Widget()
{
    if (style)
    {
        boost::shared_ptr<TextureRegionDescriptor> texture = boost::dynamic_pointer_cast<TextureRegionDescriptor>(style->background);
        boost::shared_ptr<NinePatchDescriptor> ninepatch = boost::dynamic_pointer_cast<NinePatchDescriptor>(style->background);

        if (ninepatch)
        {
            m_bgSprite = boost::shared_ptr<Sprite>(new NinePatch(ninepatch));
            addChild(m_bgSprite);
        }
        else if (texture)
        {
            m_bgSprite = boost::shared_ptr<Sprite>(new Sprite(texture));
            addChild(m_bgSprite);
        }

        if (style->font)
            m_font = ResourceManager::instance().loadFont(style->font->path, style->font->size);

        if (style->captionFont)
            m_captionFont = ResourceManager::instance().loadFont(style->captionFont->path, style->captionFont->size);

        m_captionColor = style->captionColor;
        m_labelColor = style->labelColor;
        m_color = style->color;

        m_contentRect = style->contentRect;
        m_captionContentRect = style->captionContentRect;
        m_iconPosition = style->iconPosition;
        m_raceIconPosition = style->raceIconPosition;
        m_iconSize = style->iconSize;

        m_style = style;
    }

    if (!m_font)
        m_font = Engine::instance().coreFont();

    if (!m_captionFont)
        m_captionFont = Engine::instance().coreFont();

    m_caption = boost::shared_ptr<Label>(new Label(L"", m_captionFont));
    m_caption->setColor(m_captionColor);

    if (m_bgSprite)
    {
        setWidth(m_bgSprite->normalWidth());
        setHeight(m_bgSprite->normalHeight());
    }

    auto planetProvider = boost::shared_ptr<InfoWidgetProvider>(new PlanetInfoWidgetProvider());

    m_providers[WorldHelper::TYPE_ASTEROID] = boost::shared_ptr<InfoWidgetProvider>(new AsteroidInfoWidgetProvider());
    m_providers[WorldHelper::TYPE_HABITABLEPLANET] = planetProvider;
    m_providers[WorldHelper::TYPE_DESERTPLANET] = planetProvider;
    m_providers[WorldHelper::TYPE_PLANETARYSYSTEM] = boost::shared_ptr<InfoWidgetProvider>(new SystemInfoWidgetProvider());
}

void InfoWidget::addContentProvider(uint32_t typeID, boost::shared_ptr<InfoWidgetProvider> provider)
{
    m_providers[typeID] = provider;
}

void InfoWidget::processMain()
{
    Widget::processMain();

    Rect realContentRect;
    if (m_contentRect.valid())
        realContentRect = m_contentRect;
    else
    {
        realContentRect.x = 0;
        realContentRect.y = 0;
        realContentRect.width = width();
        realContentRect.height = height();
    }

    if (m_content && m_currentProvider && m_currentObject)
        m_currentProvider->updateContent(m_currentObject, m_content, realContentRect);

    if (m_bgSprite && m_contentRect.valid() && realContentRect.valid())
    {
        int width = m_bgSprite->normalWidth() + realContentRect.width - m_contentRect.width;
        int height = m_bgSprite->normalHeight() + realContentRect.height - m_contentRect.height;
        m_bgSprite->setGeometry(width, height);
    }

    Rect realCaptionContentRect;
    if (m_captionContentRect.valid())
    {
        realCaptionContentRect.x = m_captionContentRect.x;
        realCaptionContentRect.y = m_captionContentRect.y;
        realCaptionContentRect.width = realContentRect.width - m_contentRect.width + m_captionContentRect.width;
        realCaptionContentRect.height = m_captionContentRect.height;
    }
    else
    {
        realCaptionContentRect.x = 0;
        realCaptionContentRect.y = 0;
        realCaptionContentRect.width = width();
        realCaptionContentRect.height = height();
    }
    if (m_caption->needUpdate())
        m_caption->processMain();

    m_caption->setPosition(int(realCaptionContentRect.x + (realCaptionContentRect.width - m_caption->width()) / 2),
                           int(realCaptionContentRect.y + (realCaptionContentRect.height - m_caption->height()) / 2));
    if (m_iconSprite)
    {
        m_iconSprite->setPosition(m_iconPosition.x, m_iconPosition.y);
        m_iconSprite->setGeometry(m_iconSize, m_iconSize);
    }

    if (m_raceIconSprite)
    {
        m_raceIconSprite->setPosition(m_raceIconPosition.x + (realContentRect.width - m_contentRect.width) - m_raceIconSprite->width() / 2,
                                      m_raceIconPosition.y + (realContentRect.height - m_contentRect.height) - m_raceIconSprite->height() / 2);
    }
}

bool InfoWidget::containsPoint(const Vector &p) const
{
    return false;
}

void InfoWidget::draw() const
{
    if (!prepareDraw())
        return;

    if (m_bgSprite)
        m_bgSprite->draw();

    m_caption->draw();

    if (m_iconSprite)
        m_iconSprite->draw();

    if (m_raceIconSprite)
        m_raceIconSprite->draw();

    if (m_content)
    {
        for (auto w : m_content->widgets)
            w->draw();
    }

    endDraw();
}

void InfoWidget::clear()
{
    m_caption->setText(L"");
    if (m_iconSprite)
    {
        removeChild(m_iconSprite);
        m_iconSprite = boost::shared_ptr<Sprite>();
    }
    if (m_raceIconSprite)
    {
        removeChild(m_raceIconSprite);
        m_raceIconSprite = boost::shared_ptr<Sprite>();
    }
    if (m_content)
    {
        for (auto w : m_content->widgets)
            removeWidget(w);
    }

    m_content = boost::shared_ptr<InfoWidgetContent>();
    m_currentProvider = boost::shared_ptr<InfoWidgetProvider>();

    markToUpdate();
}

void InfoWidget::show(boost::shared_ptr<WorldObject> object)
{
    clear();

    if (!object)
        return;

    auto i = m_providers.find(object->type());
    if (i == m_providers.end())
        return;
    else
        m_currentProvider = i->second;

    if (!m_currentProvider)
        return;

    m_content = m_currentProvider->createContent(object, m_style);

    if (!m_content)
        return;

    m_currentObject = object;

    m_caption->setText(m_content->caption);
    if (m_content->hasIcon)
    {
        m_iconSprite = boost::shared_ptr<Sprite>(new Sprite(m_content->icon));
        addChild(m_iconSprite);
    }
    if (m_content->hasRace)
    {
        boost::shared_ptr<Race> race = WorldManager::instance().raceManager().race(m_content->race);
        if (race)
        {
            m_raceIconSprite = boost::shared_ptr<Sprite>(new Sprite(race->icon));
            addChild(m_raceIconSprite);
        }
    }

    for (auto w : m_content->widgets)
        addWidget(w);

    markToUpdate();
}
}
}