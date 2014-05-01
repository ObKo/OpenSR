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
            style->star = object.get("star", "").asString();
            style->animated = object.get("star-animated", true).asBool();
            style->color = parseColor(object.get("star-color", "#FFFFFF"));
            style->background = object.get("background", "").asString();
        }
        catch (...)
        {
            Log::error() << "Invalid system style: " << currentPath;
            return boost::shared_ptr< ResourceObject >();
        }

        return style;
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

bool StyleManager::serialize(std::ostream &stream) const
{
    stream.write((const char*)&STYLE_MANAGER_SIGNATURE, 4);

    uint32_t count = m_planetStyles.size();
    stream.write((const char*)&count, 4);

    if (!stream.good())
        return false;

    std::map<uint32_t, boost::shared_ptr<PlanetStyle> >::const_iterator end = m_planetStyles.end();
    for (std::map<uint32_t, boost::shared_ptr<PlanetStyle> >::const_iterator i = m_planetStyles.begin(); i != end; ++i)
    {
        if (!((*i).second)->serialize(stream))
            return false;
    }

    count = m_systemStyles.size();
    stream.write((const char*)&count, 4);

    if (!stream.good())
        return false;

    std::map<uint32_t, boost::shared_ptr<SystemStyle> >::const_iterator send = m_systemStyles.end();
    for (std::map<uint32_t, boost::shared_ptr<SystemStyle> >::const_iterator i = m_systemStyles.begin(); i != send; ++i)
    {
        if (!((*i).second)->serialize(stream))
            return false;
    }

    count = m_asteroidStyles.size();
    stream.write((const char*)&count, 4);

    if (!stream.good())
        return false;

    std::map<uint32_t, boost::shared_ptr<AsteroidStyle> >::const_iterator aend = m_asteroidStyles.end();
    for (std::map<uint32_t, boost::shared_ptr<AsteroidStyle> >::const_iterator i = m_asteroidStyles.begin(); i != aend; ++i)
    {
        if (!((*i).second)->serialize(stream))
            return false;
    }

    return true;
}

bool StyleManager::deserialize(std::istream &stream)
{
    m_planetStyles.clear();

    uint32_t sig;
    stream.read((char*)&sig, 4);

    if (sig != STYLE_MANAGER_SIGNATURE)
        return false;

    uint32_t count;
    stream.read((char*)&count, 4);

    for (int i = 0; i < count; i++)
    {
        boost::shared_ptr<PlanetStyle> style = boost::shared_ptr<PlanetStyle>(new PlanetStyle());
        if (!style->deserialize(stream))
            return false;

        m_planetStyles[textHash32(style->id)] = style;
    }

    stream.read((char*)&count, 4);

    for (int i = 0; i < count; i++)
    {
        boost::shared_ptr<SystemStyle> style = boost::shared_ptr<SystemStyle>(new SystemStyle());
        if (!style->deserialize(stream))
            return false;
        m_systemStyles[textHash32(style->id)] = style;
    }

    stream.read((char*)&count, 4);

    for (int i = 0; i < count; i++)
    {
        boost::shared_ptr<AsteroidStyle> style = boost::shared_ptr<AsteroidStyle>(new AsteroidStyle());
        if (!style->deserialize(stream))
            return false;
        m_asteroidStyles[textHash32(style->id)] = style;
    }
    return true;
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
}
}
