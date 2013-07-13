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

#include "SystemManager.h"
#include "Types.h"
#include <libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <OpenSR/JSONHelper.h>
#include <OpenSR/Engine.h>
#include <OpenSR/Texture.h>
#include <json/value.h>

namespace
{
const uint32_t SYSTEM_MANAGER_SIGNATURE = *((uint32_t*)"SRSM");
}

namespace Rangers
{
namespace World
{
SystemManager::SystemManager()
{
    std::wstring stylesPath = fromUTF8(Engine::instance().properties()->get<std::string>("world.systemStyles", "World/SystemStyles.json").c_str());
    if (!stylesPath.empty())
        loadStyles(stylesPath);
}

SystemManager::SystemManager(const SystemManager& other)
{

}

bool SystemManager::deserialize(std::istream& stream)
{
    uint32_t sig;
    stream.read((char*)&sig, 4);

    if (sig != SYSTEM_MANAGER_SIGNATURE)
        return false;

    uint32_t count;
    stream.read((char*)&count, 4);

    for (int i = 0; i < count; i++)
    {
        SystemStyle style;
        if (!style.deserialize(stream))
            return false;
    }
}

bool SystemManager::serialize(std::ostream& stream) const
{
    stream.write((const char*)&SYSTEM_MANAGER_SIGNATURE, 4);

    uint32_t count = m_styles.size();
    stream.write((const char*)&count, 4);

    if (!stream.good())
        return false;

    std::map<uint32_t, boost::shared_ptr<SystemStyle> >::const_iterator end = m_styles.end();
    for (std::map<uint32_t, boost::shared_ptr<SystemStyle> >::const_iterator i = m_styles.begin(); i != end; ++i)
    {
        if (!((*i).second)->serialize(stream))
            return false;
    }
    return true;
}

boost::shared_ptr<SystemStyle> SystemManager::style(const std::string& name)
{
    uint32_t hash = textHash32(name);
    return style(hash);
}

boost::shared_ptr<SystemStyle> SystemManager::style(uint32_t id)
{
    return m_styles.at(id);
}

void SystemManager::loadStyles(const std::wstring& styleFile)
{
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
        m_styles[textHash32(style->id)] = boost::shared_ptr<SystemStyle>(style);
    }
    Log::info() << "Loaded " << members.size() << " system styles";
}

std::list<boost::shared_ptr<PlanetarySystem> > SystemManager::systems() const
{
    return m_systems;
}

boost::shared_ptr<PlanetarySystem> SystemManager::currentSystem() const
{
    return m_currentSystem;
}

void SystemManager::setCurrentSystem(boost::shared_ptr<PlanetarySystem> system)
{
    std::list<boost::shared_ptr<PlanetarySystem> >::iterator i = std::find(m_systems.begin(), m_systems.end(), system);
    if (i != m_systems.end())
        m_currentSystem = system;
}

void SystemManager::addSystem(boost::shared_ptr<PlanetarySystem> system)
{
    m_systems.push_back(system);
}
}
}
