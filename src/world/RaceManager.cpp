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

#include "RaceManager.h"

#include <libRanger.h>
#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <OpenSR/JSONHelper.h>

namespace Rangers
{
namespace World
{
RaceManager::RaceManager()
{
    std::wstring filePath = fromUTF8(Engine::instance().properties()->get<std::string>("world.races", "World/Races.json").c_str());
    loadRaces(filePath);
}

void RaceManager::loadRaces(const std::wstring& file)
{
    //TODO: relations
    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(file);

    if (!json)
        return;

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Error parsing races JSON: " << reader.getFormatedErrorMessages();
        return;
    }
    if (!root.isObject())
    {
        Log::error() << "Invalid planet races JSON";
        return;
    }
    Json::Value races = root.get("races", Json::Value());
    Json::Value invaders = root.get("invaders", Json::Value());
    if (!races.isObject() || !invaders.isArray())
    {
        Log::error() << "Invalid planet races JSON";
        return;
    }
    Json::Value::Members racesMembers = races.getMemberNames();
    Json::Value::Members::const_iterator end = racesMembers.end();
    for (Json::Value::Members::const_iterator i = racesMembers.begin(); i != end; ++i)
    {
        Json::Value race = races[*i];
        boost::shared_ptr<Race> r = boost::shared_ptr<Race>(new Race());
        bool error;
        r->icon = JSONHelper::parseTextureRegion(race.get("icon", Json::Value()), error);
        r->id = textHash32(*i);
        r->invader = false;
        r->name = race.get("name", Json::Value()).asString();

        if (error)
        {
            Log::error() << "Invalid planet races JSON";
            return;
        }

        m_races[r->id] = r;
    }
    Json::Value::iterator vend = invaders.end();
    for (Json::Value::iterator i = invaders.begin(); i != vend; ++i)
    {
        boost::shared_ptr<Race> r = race(textHash32((*i).asString()));
        if (r)
            r->invader = true;
    }
}

boost::shared_ptr<Race> RaceManager::race(uint32_t id) const
{
    std::map<uint32_t, boost::shared_ptr<Race> >::const_iterator v = m_races.find(id);
    if (v != m_races.end())
        return (*v).second;
    return boost::shared_ptr<Race>();
}

boost::shared_ptr<Race> RaceManager::race(const std::string& id) const
{
    return race(textHash32(id));
}

}
}
