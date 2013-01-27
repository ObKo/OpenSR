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

#include "PlanetManager.h"
#include "Types.h"
#include <libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <OpenSR/JSONHelper.h>
#include <OpenSR/Engine.h>
#include <json/value.h>

namespace Rangers
{
namespace World
{
PlanetManager& PlanetManager::instance()
{
    static PlanetManager manager;
    return manager;
}


PlanetManager::PlanetManager()
{
    std::wstring stylesPath = fromUTF8(Engine::instance().properties()->get<std::string>("world.planetStyles", "ORC/planets.json").c_str());
    if (!stylesPath.empty())
        loadStyles(stylesPath);
}

boost::shared_ptr<PlanetStyle> PlanetManager::style(const std::wstring& name)
{
    uint32_t hash = textHash32(name);
    return style(hash);
}

boost::shared_ptr<PlanetStyle> PlanetManager::style(uint32_t id)
{
    return m_styles.at(id);
}

void PlanetManager::loadStyles(const std::wstring& styleFile)
{
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
        bool ok;
        PlanetStyle *style = new PlanetStyle();
        style->ambientColor = JSONHelper::parseColor(jsonStyle.get("ambientColor", "#FFFFFF").asString(), ok);
        style->texture = fromUTF8(jsonStyle.get("texture", "").asCString());
        style->speed = jsonStyle.get("speed", 0.0f).asDouble();
        style->hasCloud = jsonStyle.get("hasCloud", false).asBool();
        style->id = fromUTF8((*i).c_str());
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
        m_styles[textHash32(style->id)] = boost::shared_ptr<PlanetStyle>(style);
    }
    Log::info() << "Loaded " << members.size() << " planet styles";
}
}
}
