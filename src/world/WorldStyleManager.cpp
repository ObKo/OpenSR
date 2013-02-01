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

#include "WorldStyleManager.h"
#include <libRanger.h>

#include <OpenSR/ResourceManager.h>
#include <OpenSR/Engine.h>
#include <OpenSR/Log.h>
#include <OpenSR/JSONHelper.h>
#include <json/reader.h>

namespace Rangers
{
namespace World
{
void WorldStyleManager::loadStyles()
{
    std::wstring skinFile = fromUTF8(Engine::instance().properties()->get<std::string>("world.skin", "World/Skin.json").c_str());
    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(skinFile);

    if (!json)
        return;

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Cannot parse World skin: " << reader.getFormatedErrorMessages();
    }
    bool error;
    std::map<std::wstring, ResourceDescriptor> resources = JSONHelper::parseResources(root, error);

    if (error)
        return;

    Json::Value::Members members = root.getMemberNames();

    if (std::find(members.begin(), members.end(), "InfoWidgetStyle") != members.end())
    {
        Json::Value style = root.get("InfoWidgetStyle", Json::Value());
        Json::Value::Members styleMembers = style.getMemberNames();

        if (std::find(styleMembers.begin(), styleMembers.end(), "background") != styleMembers.end())
            m_infoWidgetStyle.background = JSONHelper::getResource(style.get("background", "").asString(), resources, error);
        if (std::find(styleMembers.begin(), styleMembers.end(), "content-rect") != styleMembers.end())
            m_infoWidgetStyle.contentRect = JSONHelper::parseRect(style.get("content-rect", ""), error);
        if (error)
        {
            m_infoWidgetStyle = InfoWidgetStyle();
            return;
        }

    }
}

InfoWidgetStyle WorldStyleManager::infoWidgetStyle() const
{
    return m_infoWidgetStyle;
}

}
}