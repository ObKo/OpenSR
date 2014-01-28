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

#include "SkinManager.h"
#include <OpenSR/libRanger.h>

#include <OpenSR/ResourceManager.h>
#include <OpenSR/Engine.h>
#include <OpenSR/Log.h>
#include <json/value.h>

namespace Rangers
{
namespace World
{
namespace
{
class InfoWidgetStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<InfoWidgetStyle> s = boost::shared_ptr<InfoWidgetStyle>(new InfoWidgetStyle());

        s->background = manager.loadObject(object.get("background", Json::Value()), "texture");
        s->font = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("font", Json::Value()), "font"));
        s->captionFont = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("caption-font", Json::Value()), "font"));
        s->color = parseColor(object.get("color", "#FFFFFF"));
        s->captionColor = parseColor(object.get("caption-color", "#FFFFFF"));
        s->labelColor = parseColor(object.get("label-color", "#FFFFFF"));
        s->iconPosition = parseVector(object.get("icon-position", Json::Value()));
        s->raceIconPosition = parseVector(object.get("race-icon-position", Json::Value()));
        s->iconSize = object.get("icon-size", 0).asInt();
        s->contentRect = parseRect(object.get("content-rect", Json::Value()));
        s->captionContentRect = parseRect(object.get("caption-content-rect", Json::Value()));

        return s;
    }
};
}

InfoWidgetStyle::InfoWidgetStyle():
    iconSize(0)
{

}

SkinManager::SkinManager()
{
    ResourceManager::instance().objectManager().addFactory("info-widget-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new InfoWidgetStyleFactory()));
}

void SkinManager::loadStyles()
{
    std::string skinDir = Engine::instance().properties()->get<std::string>("world.skin", "/world/skin");

    m_infoWidgetStyle = ResourceManager::instance().objectManager().getObject<InfoWidgetStyle>(skinDir + "/info-widget");
}

boost::shared_ptr<InfoWidgetStyle> SkinManager::infoWidgetStyle() const
{
    return m_infoWidgetStyle;
}

}
}