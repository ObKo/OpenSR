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
#include <OpenSR/Log.h>
#include <OpenSR/Utils.h>
#include <OpenSR/AnimatedSprite.h>
#include <libintl.h>
#include <OpenSR/libRanger.h>

#include "Planet.h"
#include "PlanetarySystem.h"
#include "PlanetManager.h"
#include "HabitablePlanet.h"
#include "WorldManager.h"
#include "Asteroid.h"

namespace
{
const int SMALL_ICON_SIZE = 16;
}

namespace Rangers
{
namespace World
{
InfoWidget::InfoWidget(boost::shared_ptr<InfoWidgetStyle> style): Widget(), m_type(INFO_NONE)
{
    if (style)
    {
        boost::shared_ptr<TextureRegionDescriptor> texture = boost::dynamic_pointer_cast<TextureRegionDescriptor>(style->background);
        boost::shared_ptr<NinePatchDescriptor> ninepatch = boost::dynamic_pointer_cast<NinePatchDescriptor>(style->background);;

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
    }

    if (!m_font)
        m_font = Engine::instance().coreFont();

    if (!m_captionFont)
        m_captionFont = Engine::instance().coreFont();

    m_caption = boost::shared_ptr<Label>(new Label(L"", m_captionFont));
    m_caption->setOrigin(POSITION_X_CENTER, POSITION_Y_CENTER);
    m_caption->setColor(m_captionColor);

    if (m_bgSprite)
    {
        setWidth(m_bgSprite->normalWidth());
        setHeight(m_bgSprite->normalHeight());
    }
}

void InfoWidget::processMain()
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
    Rect realCaptionContentRect;
    if (m_captionContentRect.valid() && m_bgSprite)
    {
        realCaptionContentRect.x = m_captionContentRect.x;
        realCaptionContentRect.y = m_captionContentRect.y;
        realCaptionContentRect.width = (m_bgSprite->width() - m_bgSprite->normalWidth() + m_captionContentRect.width);
        realCaptionContentRect.height = m_captionContentRect.height;
    }
    else
    {
        realCaptionContentRect.x = 0;
        realCaptionContentRect.y = 0;
        realCaptionContentRect.width = width();
        realCaptionContentRect.height = height();
    }
    m_caption->setPosition((int(m_caption->width()) % 2) / 2.0f + int(realCaptionContentRect.width) / 2 + realCaptionContentRect.x,
                           (int(m_caption->height()) % 2) / 2.0f + int(realCaptionContentRect.height) / 2 + realCaptionContentRect.y);
    if (m_iconSprite)
    {
        m_iconSprite->setPosition(m_iconPosition.x, m_iconPosition.y);
        m_iconSprite->setGeometry(m_iconSize, m_iconSize);
    }

    if (m_raceIconSprite)
    {
        m_raceIconSprite->setPosition(m_raceIconPosition.x + (width() - m_bgSprite->normalWidth()) - m_raceIconSprite->width() / 2,
                                      m_raceIconPosition.y + (height() - m_bgSprite->normalHeight()) - m_raceIconSprite->height() / 2);
    }

    if (m_type == INFO_PLANET)
    {
        for (int i = 0; i < m_infoWidget.size(); i++)
        {
            boost::shared_ptr<Label> l = boost::dynamic_pointer_cast<Label>(m_infoWidget.at(i));

            if (l->needUpdate())
                l->processMain();

            if ((i % 2) == 0)
            {
                l->setPosition(realContentRect.x + int(realContentRect.width) / 2 - l->width(), realContentRect.y + (m_font->size() + 5) * (i / 2));
            }
            else
            {
                l->setPosition(realContentRect.x + int(realContentRect.width) / 2, realContentRect.y + (m_font->size() + 5) * (i / 2));
            }
        }
    }
    if (m_type == INFO_ASTEROID)
    {
        for (int i = 0; i < m_infoWidget.size(); i++)
        {
            boost::shared_ptr<Label> l = boost::dynamic_pointer_cast<Label>(m_infoWidget.at(i));

            if (l->needUpdate())
                l->processMain();

            if ((i % 2) == 0)
            {
                l->setPosition(realContentRect.x + int(realContentRect.width) / 2 - l->width(), realContentRect.y + (m_font->size() + 5) * (i / 2));
            }
            else
            {
                l->setPosition(realContentRect.x + int(realContentRect.width) / 2, realContentRect.y + (m_font->size() + 5) * (i / 2));
            }
        }
    }
    else if (m_type == INFO_SYSTEM)
    {
        int line = 0;
        int sprites = 0;

        int lineHeight = std::max(SMALL_ICON_SIZE, m_font->size()) + 5;
        int labelOffset = std::max(SMALL_ICON_SIZE - m_font->size(), 0);
        int iconWidth = std::max(SMALL_ICON_SIZE, m_font->size()) + 5;
        for (int i = 0; i < m_infoWidget.size(); i++)
        {
            if (boost::shared_ptr<Label> l = boost::dynamic_pointer_cast<Label>(m_infoWidget.at(i)))
            {
                if (l->needUpdate())
                    l->processMain();

                l->setPosition(realContentRect.x + int(realContentRect.width) / 2 - l->width(),
                               realContentRect.y + lineHeight * line + labelOffset);

                line++;
                sprites = 0;
            }
            else if (boost::shared_ptr<Sprite> s = boost::dynamic_pointer_cast<Sprite>(m_infoWidget.at(i)))
            {
                if (s->needUpdate())
                    s->processMain();

                s->setPosition(realContentRect.x + int(realContentRect.width) / 2 + sprites * iconWidth + 5,
                               realContentRect.y + lineHeight * (line - 1));
                sprites++;
            }
        }
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

    std::vector<boost::shared_ptr<Object> >::const_iterator end = m_infoWidget.end();
    for (std::vector<boost::shared_ptr<Object> >::const_iterator i = m_infoWidget.begin(); i != end; ++i)
        (*i)->draw();

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
    std::vector<boost::shared_ptr<Object> >::iterator end = m_infoWidget.end();
    for (std::vector<boost::shared_ptr<Object> >::iterator i = m_infoWidget.begin(); i != end; ++i)
    {
        removeChild(*i);
    }
    m_infoWidget.clear();
    m_type = INFO_NONE;
    setHeight(m_bgSprite->normalHeight());
    markToUpdate();
}

void InfoWidget::showAsteroid(boost::shared_ptr<Asteroid> asteroid)
{
    clear();

    m_caption->setText(_("Asteroid", "OpenSR-World"));
    m_iconSprite = boost::shared_ptr<Sprite>(new Sprite(WorldManager::instance().styleManager().asteroidStyle(asteroid->style())->sprite));
    addChild(m_iconSprite);

    boost::shared_ptr<Label> l = boost::shared_ptr<Label>(new Label(_("Speed:", "OpenSR-World") + L" ", m_font));
    l->setColor(m_labelColor);
    m_infoWidget.push_back(l);
    addChild(l);

    std::wostringstream str;
    str << int(round(asteroid->speed()));

    l = boost::shared_ptr<Label>(new Label(str.str(), m_font));
    l->setColor(m_color);
    m_infoWidget.push_back(l);
    addChild(l);

    int requiredHeight = (m_font->size() + 5);
    int deltaHeight = requiredHeight - m_contentRect.height;
    setHeight(std::max(deltaHeight + m_bgSprite->normalHeight(), m_bgSprite->normalHeight()));

    m_type = INFO_ASTEROID;
    markToUpdate();
}

void InfoWidget::showPlanet(boost::shared_ptr<Planet> planet)
{
    clear();

    m_caption->setText(_(planet->name(), "OpenSR-World"));
    m_iconSprite = boost::shared_ptr<Sprite>(new Sprite(WorldManager::instance().planetManager().getPlanetImage(planet->style(), m_iconSize)));
    addChild(m_iconSprite);

    boost::shared_ptr<Label> l = boost::shared_ptr<Label>(new Label(_("Radius:", "OpenSR-World") + L" ", m_font));
    l->setColor(m_labelColor);
    m_infoWidget.push_back(l);
    addChild(l);

    std::wostringstream str;
    str << planet->radius();

    l = boost::shared_ptr<Label>(new Label(str.str(), m_font));
    l->setColor(m_color);
    m_infoWidget.push_back(l);
    addChild(l);

    int labelCount = 1;

    if (boost::shared_ptr<HabitablePlanet> hPlanet = boost::dynamic_pointer_cast<HabitablePlanet>(planet))
    {
        l = boost::shared_ptr<Label>(new Label(_("Population:", "OpenSR-World") + L" ", m_font));
        l->setColor(m_labelColor);
        m_infoWidget.push_back(l);
        addChild(l);

        str.clear();
        str << hPlanet->population() / (1000 * 1000);

        l = boost::shared_ptr<Label>(new Label(str.str(), m_font));
        l->setColor(m_color);
        m_infoWidget.push_back(l);
        addChild(l);
        labelCount++;

        l = boost::shared_ptr<Label>(new Label(_("Race:", "OpenSR-World") + L" ", m_font));
        l->setColor(m_labelColor);
        m_infoWidget.push_back(l);
        addChild(l);

        boost::shared_ptr<Race> race;

        if (hPlanet->landContext())
            race = WorldManager::instance().raceManager().race(hPlanet->landContext()->race());

        if (race)
        {
            l = boost::shared_ptr<Label>(new Label(_(race->name, "OpenSR-World"), m_font));
            if (race->icon)
            {
                m_raceIconSprite = boost::shared_ptr<Sprite>(new Sprite(race->icon));
                addChild(m_raceIconSprite);
            }
        }
        else
            l = boost::shared_ptr<Label>(new Label(_("Unknown", "OpenSR-World"), m_font));

        l->setColor(m_color);
        m_infoWidget.push_back(l);
        addChild(l);
        labelCount++;

    }

    int requiredHeight = (m_font->size() + 5) * labelCount;
    int deltaHeight = requiredHeight - m_contentRect.height;
    setHeight(std::max(deltaHeight + m_bgSprite->normalHeight(), m_bgSprite->normalHeight()));

    m_type = INFO_PLANET;
    markToUpdate();
}

void InfoWidget::showSystem(boost::shared_ptr<PlanetarySystem> system)
{
    clear();

    if (!system)
        return;

    m_caption->setText(_(system->name(), "OpenSR-World"));

    //FIXME: AnimatedSprite/Sprite
    boost::shared_ptr<SystemStyle> style = WorldManager::instance().styleManager().systemStyle(system->style());

    if (style)
    {
        m_iconSprite = boost::shared_ptr<Sprite>(new AnimatedSprite(style->star));
        m_iconSprite->setColor(style->color);
        addChild(m_iconSprite);
    }

    std::list<boost::shared_ptr<SystemObject> > objects = system->systemObjects();
    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = objects.end();
    int objectCount = 0;
    for (std::list<boost::shared_ptr<SystemObject> >::const_iterator i = objects.begin(); i != end; ++i)
    {
        boost::shared_ptr<SystemObject> object = *i;
        if (boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(object))
        {
            boost::shared_ptr<Label> l = boost::shared_ptr<Label>(new Label(_(planet->name(), "OpenSR-World"), m_font));
            l->setColor(m_color);
            m_infoWidget.push_back(l);
            addChild(l);

            boost::shared_ptr<Texture> planetImage = WorldManager::instance().planetManager().getPlanetImage(planet->style(), std::max(SMALL_ICON_SIZE, m_font->size()));

            if (planetImage)
            {
                boost::shared_ptr<Sprite> s = boost::shared_ptr<Sprite>(new Sprite(planetImage));
                m_infoWidget.push_back(s);
                addChild(s);
            }


            if (boost::shared_ptr<HabitablePlanet> hPlanet = boost::dynamic_pointer_cast<HabitablePlanet>(planet))
            {
                boost::shared_ptr<Race> race;

                if (hPlanet->landContext())
                    race = WorldManager::instance().raceManager().race(hPlanet->landContext()->race());

                if (race)
                {
                    if (race->icon)
                    {
                        boost::shared_ptr<Sprite> s = boost::shared_ptr<Sprite>(new Sprite(race->icon));
                        float aspect = std::min(s->height() / SMALL_ICON_SIZE, s->height() / m_font->size());
                        s->setGeometry(int(s->width() / aspect), int(s->height() / aspect));
                        m_infoWidget.push_back(s);
                        addChild(s);
                    }
                }
            }

            objectCount++;
        }
    }

    int lineHeight = std::max(SMALL_ICON_SIZE, m_font->size()) + 5;

    int requiredHeight = lineHeight * objectCount;
    int deltaHeight = requiredHeight - m_contentRect.height;
    setHeight(std::max(deltaHeight + m_bgSprite->normalHeight(), m_bgSprite->normalHeight()));

    m_type = INFO_SYSTEM;
    markToUpdate();
}
}
}