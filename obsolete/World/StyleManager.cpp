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

#include "StyleManager.h"
#include "Types.h"
#include <OpenSR/libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <json/value.h>

namespace
{
const uint32_t STYLE_MANAGER_SIGNATURE = *((uint32_t*)"SRSM");
}

namespace Rangers
{
namespace World
{
namespace
{
struct PlanetStyles: public ResourceObject
{
    std::map<std::string, boost::shared_ptr<PlanetStyle> > styles;
};

struct SystemStyles: public ResourceObject
{
    std::map<std::string, boost::shared_ptr<SystemStyle> > styles;
};

struct AsteroidStyles: public ResourceObject
{
    std::map<std::string, boost::shared_ptr<AsteroidStyle> > styles;
};

struct SystemBackgrounds: public ResourceObject
{
    std::map<std::string, std::string > backgrounds;
};

class StylesFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<PlanetStyles> planetStyles;
        boost::shared_ptr<AsteroidStyles> asteroidStyles;
        boost::shared_ptr<SystemStyles> systemStyles;
        std::string type = object.get("@type", "").asString();

        if (type == "planet-styles")
            planetStyles = boost::shared_ptr<PlanetStyles>(new PlanetStyles());
        else if (type == "asteroid-styles")
            asteroidStyles = boost::shared_ptr<AsteroidStyles>(new AsteroidStyles());
        else if (type == "system-styles")
            systemStyles = boost::shared_ptr<SystemStyles>(new SystemStyles());

        for (const std::string& s : object.getMemberNames())
        {
            if (s == "@type")
                continue;

            boost::shared_ptr<ResourceObject> o = manager.loadObject(currentPath + "/" + s, object.get(s, Json::Value()));
            if (type == "planet-styles")
            {
                boost::shared_ptr<PlanetStyle> ps = boost::dynamic_pointer_cast<PlanetStyle>(o);
                if (ps)
                {
                    ps->id = s;
                    planetStyles->styles[s] = ps;
                }
            }
            else if (type == "asteroid-styles")
            {
                boost::shared_ptr<AsteroidStyle> as = boost::dynamic_pointer_cast<AsteroidStyle>(o);
                if (as)
                {
                    as->id = s;
                    asteroidStyles->styles[s] = as;
                }
            }
            else if (type == "system-styles")
            {
                boost::shared_ptr<SystemStyle> ss = boost::dynamic_pointer_cast<SystemStyle>(o);
                if (ss)
                {
                    ss->id = s;
                    systemStyles->styles[s] = ss;
                }
            }
        }
        if (type == "planet-styles")
            return planetStyles;
        else if (type == "asteroid-styles")
            return asteroidStyles;
        else if (type == "system-styles")
            return systemStyles;

        return boost::shared_ptr< ResourceObject >();

    }
};

class PlanetStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<PlanetStyle> style = boost::shared_ptr<PlanetStyle>(new PlanetStyle());

        try
        {
            style->ambientColor = parseColor(object.get("ambientColor", "#FFFFFF"));
            style->cloud = object.get("cloud", "").asString();
            style->cloudSpeed = object.get("cloudSpeed", 0).asDouble();
            style->hasCloud = object.get("hasCloud", false).asBool();
            style->hasRing = object.get("hasRing", false).asBool();
            style->hasRingBackground = object.get("hasRingBackground", false).asBool();
            style->ring = object.get("ring", "").asString();
            style->ringBackground = object.get("ringBackground", "").asString();
            style->ringBgOffsetX = object.get("ringBgOffsetX", 0).asDouble();
            style->ringBgOffsetY = object.get("ringBgOffsetY", 0).asDouble();
            style->ringOffsetX = object.get("ringOffsetX", 0).asDouble();
            style->ringOffsetY = object.get("ringOffsetY", 0).asDouble();
            style->speed = object.get("speed", 0).asDouble();
            style->texture = object.get("texture", "").asString();
        }
        catch (...)
        {
            Log::error() << "Invalid planet style: " << currentPath;
            return boost::shared_ptr< ResourceObject >();
        }

        return style;
    }
};

class SystemStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<SystemStyle> style = boost::shared_ptr<SystemStyle>(new SystemStyle());

        try
        {
            style->starImage = object.get("star-image", "").asString();
            style->starAnimation = object.get("star-animation", "").asString();
            style->color = parseColor(object.get("star-color", "#FFFFFF"));
        }
        catch (...)
        {
            Log::error() << "Invalid system style: " << currentPath;
            return boost::shared_ptr< ResourceObject >();
        }

        return style;
    }
};

class SystemBackgroundsFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<SystemBackgrounds> bgs(new SystemBackgrounds());
        for (const std::string& s : object.getMemberNames())
        {
            if (s == "@type")
                continue;
            bgs->backgrounds[s] = object.get(s, "").asString();
        }

        return bgs;
    }
};


class AsteroidStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<AsteroidStyle> style = boost::shared_ptr<AsteroidStyle>(new AsteroidStyle());

        try
        {
            style->sprite = object.get("sprite", "").asString();
            style->animated = object.get("animated", true).asBool();
        }
        catch (...)
        {
            Log::error() << "Invalid asteroid style: " << currentPath;
            return boost::shared_ptr< ResourceObject >();
        }

        return style;
    }
};
}

StyleManager::StyleManager()
{
    ResourceManager::instance().objectManager().addFactory("planet-styles", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new StylesFactory()));
    ResourceManager::instance().objectManager().addFactory("asteroid-styles", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new StylesFactory()));
    ResourceManager::instance().objectManager().addFactory("system-styles", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new StylesFactory()));
    ResourceManager::instance().objectManager().addFactory("system-backgrounds", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new SystemBackgroundsFactory()));
    ResourceManager::instance().objectManager().addFactory("planet-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new PlanetStyleFactory()));
    ResourceManager::instance().objectManager().addFactory("asteroid-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new AsteroidStyleFactory()));
    ResourceManager::instance().objectManager().addFactory("system-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new SystemStyleFactory()));
}

boost::shared_ptr<PlanetStyle> StyleManager::planetStyle(const std::string& name)
{
    uint32_t hash = textHash32(name);
    return planetStyle(hash);
}

boost::shared_ptr<PlanetStyle> StyleManager::planetStyle(uint32_t id)
{
    return m_planetStyles.at(id);
}

void StyleManager::loadPlanetStyles(const std::string& styleFile)
{
    m_planetStyles.clear();

    boost::shared_ptr<PlanetStyles> styles = ResourceManager::instance().objectManager().getObject<PlanetStyles>(styleFile);

    if (!styles)
        return;

    for (const std::pair<std::string, boost::shared_ptr<PlanetStyle> >& p : styles->styles)
        m_planetStyles[textHash32(p.first)] = p.second;

    Log::info() << "Loaded " << m_planetStyles.size() << " planet styles";
}

boost::shared_ptr<SystemStyle> StyleManager::systemStyle(const std::string& name)
{
    uint32_t hash = textHash32(name);
    return systemStyle(hash);
}

boost::shared_ptr<SystemStyle> StyleManager::systemStyle(uint32_t id)
{
    return m_systemStyles.at(id);
}

void StyleManager::loadSystemStyles(const std::string& styleFile)
{
    m_systemStyles.clear();

    boost::shared_ptr<SystemStyles> styles = ResourceManager::instance().objectManager().getObject<SystemStyles>(styleFile);

    if (!styles)
        return;

    for (const std::pair<std::string, boost::shared_ptr<SystemStyle> >& p : styles->styles)
        m_systemStyles[textHash32(p.first)] = p.second;

    Log::info() << "Loaded " << m_systemStyles.size() << " system styles";
}

void StyleManager::loadAsteroidStyles(const std::string& styleFile)
{
    m_asteroidStyles.clear();

    boost::shared_ptr<AsteroidStyles> styles = ResourceManager::instance().objectManager().getObject<AsteroidStyles>(styleFile);

    if (!styles)
        return;

    for (const std::pair<std::string, boost::shared_ptr<AsteroidStyle> >& p : styles->styles)
        m_asteroidStyles[textHash32(p.first)] = p.second;

    Log::info() << "Loaded " << m_asteroidStyles.size() << " asteroid styles";
}

boost::shared_ptr<AsteroidStyle> StyleManager::asteroidStyle(const std::string& name)
{
    uint32_t hash = textHash32(name);
    return asteroidStyle(hash);
}

boost::shared_ptr<AsteroidStyle> StyleManager::asteroidStyle(uint32_t id)
{
    return m_asteroidStyles.at(id);
}

void StyleManager::addPlanetStyle(boost::shared_ptr<PlanetStyle> style)
{
    uint32_t hash = textHash32(style->id);
    m_planetStyles[hash] = style;
}

void StyleManager::addSystemStyle(boost::shared_ptr<SystemStyle> style)
{
    uint32_t hash = textHash32(style->id);
    m_systemStyles[hash] = style;
}

void StyleManager::addAsteroidStyle(boost::shared_ptr<AsteroidStyle> style)
{
    uint32_t hash = textHash32(style->id);
    m_asteroidStyles[hash] = style;
}

StyleManager::StyleManager(const StyleManager& other)
{
}

void StyleManager::addSystemBackground(uint32_t id, const std::string& path)
{
    m_systemBackgrounds[id] = path;
}

void StyleManager::loadSystemBackgrounds(const std::string& path)
{
    m_systemBackgrounds.clear();

    boost::shared_ptr<SystemBackgrounds> bgs = ResourceManager::instance().objectManager().getObject<SystemBackgrounds>(path);

    if (!bgs)
        return;

    for (const std::pair<std::string, std::string >& p : bgs->backgrounds)
        m_systemBackgrounds[textHash32(p.first)] = p.second;

    Log::info() << "Loaded " << m_systemBackgrounds.size() << " system backgrounds";
}

std::string StyleManager::systemBackground(uint32_t id)
{
    return m_systemBackgrounds.at(id);
}

std::string StyleManager::systemBackground(const std::string& name)
{
    return m_systemBackgrounds.at(textHash32(name));
}

}
}
