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

#include "StyleManager.h"
#include "Types.h"
#include <OpenSR/libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <OpenSR/JSONHelper.h>

namespace
{
const uint32_t STYLE_MANAGER_SIGNATURE = *((uint32_t*)"SRSM");
}

namespace Rangers
{
namespace World
{
StyleManager::StyleManager()
{
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

void StyleManager::loadPlanetStyles(const std::wstring& styleFile)
{
    m_planetStyles.clear();

    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(styleFile);
    if (!json)
        return;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Error parsing planet styles JSON: " << reader.getFormatedErrorMessages();
        return;
    }
    if (!root.isObject())
    {
        Log::error() << "Invalid planet styles JSON";
        return;
    }
    Json::Value::Members members = root.getMemberNames();
    Json::Value::Members::const_iterator end = members.end();
    for (Json::Value::Members::const_iterator i = members.begin(); i != end; ++i)
    {
        Json::Value jsonStyle = root[(*i)];

        if (!jsonStyle.isObject())
        {
            Log::error() << "Invalid planet styles JSON";
            return;
        }
        bool error = false;
        PlanetStyle *style = new PlanetStyle();
        style->ambientColor = JSONHelper::parseColor(jsonStyle.get("ambientColor", "#FFFFFF").asString(), error);
        style->texture = fromUTF8(jsonStyle.get("texture", "").asCString());
        style->speed = jsonStyle.get("speed", 0.0f).asDouble();
        style->hasCloud = jsonStyle.get("hasCloud", false).asBool();
        style->id = (*i);
        style->hasRing = jsonStyle.get("hasRing", false).asBool();
        style->hasRingBackground = jsonStyle.get("hasRingBackground", false).asBool();

        if (style->hasCloud)
        {
            style->cloud = fromUTF8(jsonStyle.get("cloud", "").asCString());
            style->cloudSpeed = jsonStyle.get("cloudSpeed", 0.0f).asDouble();
        }
        if (style->hasRing)
        {
            style->ring = fromUTF8(jsonStyle.get("ring", "").asCString());
            style->ringOffsetX = jsonStyle.get("ringOffsetX", 0.0f).asDouble();
            style->ringOffsetY = jsonStyle.get("ringOffsetY", 0.0f).asDouble();
        }
        if (style->hasRingBackground)
        {
            style->ringBackground = fromUTF8(jsonStyle.get("ringBackground", "").asCString());
            style->ringBgOffsetX = jsonStyle.get("ringBgOffsetX", 0.0f).asDouble();
            style->ringBgOffsetY = jsonStyle.get("ringBgOffsetY", 0.0f).asDouble();
        }
        m_planetStyles[textHash32(style->id)] = boost::shared_ptr<PlanetStyle>(style);
    }
    Log::info() << "Loaded " << members.size() << " planet styles";
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

void StyleManager::loadSystemStyles(const std::wstring& styleFile)
{
    m_systemStyles.clear();

    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(styleFile);
    if (!json)
        return;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Error parsing system styles JSON: " << reader.getFormatedErrorMessages();
        return;
    }
    if (!root.isObject())
    {
        Log::error() << "Invalid system styles JSON";
        return;
    }
    Json::Value::Members members = root.getMemberNames();
    Json::Value::Members::const_iterator end = members.end();
    for (Json::Value::Members::const_iterator i = members.begin(); i != end; ++i)
    {
        Json::Value jsonStyle = root[(*i)];

        if (!jsonStyle.isObject())
        {
            Log::error() << "Invalid system styles JSON";
            return;
        }
        bool error = false;
        SystemStyle *style = new SystemStyle();
        style->id = (*i);
        style->star = fromUTF8(jsonStyle.get("star", "").asCString());
        style->animated = jsonStyle.get("star-animated", "").asBool();
        style->color = JSONHelper::parseColor(jsonStyle.get("star-color", "#FFFFFF").asString(), error);
        style->background = fromUTF8(jsonStyle.get("background", "").asCString());
        m_systemStyles[textHash32(style->id)] = boost::shared_ptr<SystemStyle>(style);
    }
    Log::info() << "Loaded " << members.size() << " system styles";
}

void StyleManager::loadAsteroidStyles(const std::wstring& styleFile)
{
    m_asteroidStyles.clear();

    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(styleFile);
    if (!json)
        return;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Error parsing asteroid styles JSON: " << reader.getFormatedErrorMessages();
        return;
    }
    if (!root.isObject())
    {
        Log::error() << "Invalid asteroid styles JSON";
        return;
    }
    Json::Value::Members members = root.getMemberNames();
    Json::Value::Members::const_iterator end = members.end();
    for (Json::Value::Members::const_iterator i = members.begin(); i != end; ++i)
    {
        Json::Value jsonStyle = root[(*i)];

        if (!jsonStyle.isObject())
        {
            Log::error() << "Invalid asteroid styles JSON";
            return;
        }
        bool error = false;
        AsteroidStyle *style = new AsteroidStyle();
        style->id = (*i);
        style->sprite = fromUTF8(jsonStyle.get("sprite", "").asCString());
        style->animated = jsonStyle.get("animated", "").asBool();
        m_asteroidStyles[textHash32(style->id)] = boost::shared_ptr<AsteroidStyle>(style);
    }
    Log::info() << "Loaded " << members.size() << " asteroid styles";
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

StyleManager::StyleManager(const StyleManager& other)
{
}
}
}
