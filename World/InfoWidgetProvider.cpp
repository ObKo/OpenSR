/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/World/InfoWidgetProvider.h"

#include "OpenSR/World/Asteroid.h"
#include "OpenSR/World/Planet.h"
#include "OpenSR/World/HabitablePlanet.h"
#include "OpenSR/World/PlanetarySystem.h"
#include "OpenSR/World/WorldManager.h"

#include <OpenSR/ResourceManager.h>
#include <OpenSR/Label.h>
#include <OpenSR/Font.h>
#include <OpenSR/LabelWidget.h>
#include <OpenSR/SpriteWidget.h>
#include <OpenSR/Utils.h>
#include <OpenSR/Engine.h>

#include <sstream>

namespace Rangers
{
namespace World
{
InfoWidgetContent::InfoWidgetContent()
{
    race = 0;
    hasRace = false;
    hasIcon = false;
}

boost::shared_ptr<InfoWidgetContent> AsteroidInfoWidgetProvider::createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const
{
    boost::shared_ptr<KeyValueContent> content = boost::shared_ptr<KeyValueContent>(new KeyValueContent());

    boost::shared_ptr<Asteroid> asteroid = boost::dynamic_pointer_cast<Asteroid>(object);

    if (!asteroid)
        return content;

    content->caption = _("Asteroid", "Asteroid.Name", "OpenSR-World");
    std::wstring sprite = WorldManager::instance().styleManager().asteroidStyle(asteroid->style())->sprite;
    if (!sprite.empty())
    {
        boost::shared_ptr<Texture> t = ResourceManager::instance().loadTexture(sprite);

        if (t)
        {
            content->hasIcon = true;
            content->icon = TextureRegion(t);
        }
    }

    std::wostringstream str;
    str << int(round(asteroid->speed()));

    std::list<std::pair<std::wstring, std::wstring> > kv {std::make_pair(_("Speed:", "", "OpenSR-World"), str.str())};
    generateWidgets(content, style, kv);

    return content;
}

boost::shared_ptr<InfoWidgetContent> PlanetInfoWidgetProvider::createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const
{
    boost::shared_ptr<KeyValueContent> content = boost::shared_ptr<KeyValueContent>(new KeyValueContent());

    boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(object);

    if (!planet)
        return content;

    content->caption = _("Planet", "Planet.Name", "OpenSR-World") + L" " + _(planet->name(), "", "OpenSR-World");
    if (style)
    {
        content->icon = WorldManager::instance().planetManager().getPlanetImage(planet->style(), style->iconSize);
        content->hasIcon = true;
    }

    std::wostringstream str;
    str << int(round(planet->radius()));
    std::list<std::pair<std::wstring, std::wstring> > kv {std::make_pair(_("Radius:", "", "OpenSR-World"), str.str())};

    if (boost::shared_ptr<HabitablePlanet> hPlanet = boost::dynamic_pointer_cast<HabitablePlanet>(planet))
    {
        str.clear();
        str << hPlanet->population() / (1000 * 1000);
        kv.push_back(std::make_pair(_("Population:", "", "OpenSR-World"), str.str()));

        if (hPlanet->landContext())
        {
            boost::shared_ptr<Race> race = WorldManager::instance().raceManager().race(hPlanet->landContext()->race());
            if (race)
            {
                kv.push_back(std::make_pair(_("Race:", "", "OpenSR-World"), _(race->name, "Race.Name." + race->id, "OpenSR-World")));
            }
            content->hasRace = true;
            content->race = hPlanet->landContext()->race();
        }
    }

    generateWidgets(content, style, kv);

    return content;
}

void KeyValueInfoWidgetProvider::updateContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetContent> content, Rect &contentRect) const
{
    boost::shared_ptr<KeyValueContent> kvcontent = boost::dynamic_pointer_cast<KeyValueContent>(content);

    if (!kvcontent)
        return;

    int y = 0;
    int widgetWidth = contentRect.width / 2 - 4;

    for (std::pair<boost::shared_ptr<LabelWidget>, boost::shared_ptr<LabelWidget> > &p : kvcontent->keyValueWidgets)
    {
        p.second->label()->setFixedWidth(widgetWidth);

        p.first->processMain();
        p.second->processMain();

        p.first->setPosition(contentRect.x + widgetWidth - 2 - p.first->width(), contentRect.y + y);
        p.second->setPosition(contentRect.x + widgetWidth + 4, contentRect.y + y);

        y += p.second->height() + 5;
    }

    contentRect.height = std::max((int)contentRect.height, y);
}


void KeyValueInfoWidgetProvider::generateWidgets(boost::shared_ptr<KeyValueContent> content, boost::shared_ptr<InfoWidgetStyle> style, const std::list<std::pair<std::wstring, std::wstring> >& keyValue) const
{
    boost::shared_ptr<Font> f;
    Color kc, vc;
    if (style)
    {
        if (style->font)
            f = ResourceManager::instance().loadFont(*(style->font));
        else
            f = Engine::instance().coreFont();

        kc = style->labelColor;
        vc = style->color;
    }
    else
    {
        f = Engine::instance().coreFont();
        kc = Color(1.0f, 1.0f, 1.0f);
        vc = Color(1.0f, 1.0f, 1.0f);
    }

    for (const std::pair<std::wstring, std::wstring> &p : keyValue)
    {

        boost::shared_ptr<LabelWidget> k = boost::shared_ptr<LabelWidget>(
                                               new LabelWidget(boost::shared_ptr<Label>(new Label(p.first, f)))
                                           );
        k->label()->setColor(style->labelColor);
        content->widgets.push_back(k);

        boost::shared_ptr<LabelWidget> v = boost::shared_ptr<LabelWidget>(
                                               new LabelWidget(boost::shared_ptr<Label>(new Label(p.second, f)))
                                           );
        v->label()->setWordWrap(true);
        v->label()->setColor(style->color);
        content->widgets.push_back(v);
        content->keyValueWidgets.push_back(std::pair<boost::shared_ptr<LabelWidget>, boost::shared_ptr<LabelWidget> >(k, v));
    }
}

boost::shared_ptr<InfoWidgetContent> SystemInfoWidgetProvider::createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const
{
    boost::shared_ptr<SystemContent> content = boost::shared_ptr<SystemContent>(new SystemContent());

    boost::shared_ptr<PlanetarySystem> system = boost::dynamic_pointer_cast<PlanetarySystem>(object);
    if (!system)
        return content;

    boost::shared_ptr<Font> f;
    Color c;
    if (style)
    {
        if (style->font)
            f = ResourceManager::instance().loadFont(*(style->font));
        else
            f = Engine::instance().coreFont();

        c = style->color;
    }
    else
    {
        f = Engine::instance().coreFont();
        c = Color(1.0f, 1.0f, 1.0f);
    }

    content->caption = _(system->name(), "", "OpenSR-World");

    boost::shared_ptr<SystemStyle> sstyle = WorldManager::instance().styleManager().systemStyle(system->style());

    if (sstyle)
    {
        boost::shared_ptr<Texture> t = ResourceManager::instance().loadTexture(sstyle->star);

        if (t)
        {
            content->hasIcon = true;
            content->icon = TextureRegion(t);
        }
    }

    std::list<boost::shared_ptr<SystemObject> > objects = system->systemObjects();
    for (boost::shared_ptr<SystemObject> o : objects)
    {
        if (boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(o))
        {
            SystemContent::SystemObjectLine objectLine;

            boost::shared_ptr<LabelWidget> n = boost::shared_ptr<LabelWidget>(
                                                   new LabelWidget(boost::shared_ptr<Label>(new Label(_(planet->name(), "", "OpenSR-World"), f)))
                                               );
            n->setColor(c);
            content->widgets.push_back(n);
            objectLine.name = n;

            boost::shared_ptr<Texture> planetImage = WorldManager::instance().planetManager().getPlanetImage(planet->style(), f->size() + 2);

            if (planetImage)
            {
                boost::shared_ptr<SpriteWidget> i = boost::shared_ptr<SpriteWidget>(
                                                        new SpriteWidget(boost::shared_ptr<Sprite>(new Sprite(planetImage)))
                                                    );
                content->widgets.push_back(i);
                objectLine.icon = i;
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
                        boost::shared_ptr<SpriteWidget> ri = boost::shared_ptr<SpriteWidget>(
                                new SpriteWidget(boost::shared_ptr<Sprite>(new Sprite(race->icon)))
                                                             );
                        content->widgets.push_back(ri);
                        objectLine.raceIcon = ri;
                    }
                }
            }
            content->objectWidgets.push_back(objectLine);
        }
    }
    return content;
}

void SystemInfoWidgetProvider::updateContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetContent> content, Rect &contentRect) const
{
    boost::shared_ptr<SystemContent> scontent = boost::dynamic_pointer_cast<SystemContent>(content);

    if (!scontent)
        return;

    int y = 0;
    int widgetWidth = contentRect.width / 2 - 4;

    for (SystemContent::SystemObjectLine &l : scontent->objectWidgets)
    {
        if (l.name->needUpdate())
            l.name->processMain();

        l.name->setPosition(contentRect.x + widgetWidth - 2 - l.name->width(), contentRect.y + y);

        if (l.icon)
            l.icon->setPosition(contentRect.x + widgetWidth + 4, contentRect.y + y - int(l.icon->height() - l.name->height()) / 2);

        if (l.raceIcon)
        {
            float aspect = float(l.raceIcon->sprite()->height()) / (l.name->height() + 2);
            l.raceIcon->setGeometry(int(l.raceIcon->sprite()->width() / aspect), int(l.raceIcon->sprite()->height() / aspect));
            l.raceIcon->processMain();
            l.raceIcon->setPosition(contentRect.x + widgetWidth + 4 + l.name->height() + 2 + 4, contentRect.y + y - int(l.raceIcon->height() - l.name->height()) / 2);
        }

        y += l.name->height() + 5;
    }

    contentRect.height = std::max((int)contentRect.height, y);
}
}
}
