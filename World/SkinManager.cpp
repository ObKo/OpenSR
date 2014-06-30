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

class InteractionWidgetStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<InteractionWidgetStyle> s = boost::shared_ptr<InteractionWidgetStyle>(new InteractionWidgetStyle());

        s->background = manager.loadObject(object.get("background", Json::Value()), "texture");
        s->font = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("font", Json::Value()), "font"));
        s->color = parseColor(object.get("color", "#FFFFFF"));
        s->activeColor = parseColor(object.get("active-color", "#0000FF"));
        s->inactiveColor = parseColor(object.get("inactive-color", "#808080"));
        s->scrollStyle = boost::dynamic_pointer_cast<ScrollBarStyle>(manager.loadObject(object.get("scroll-style", Json::Value()), "scroll-bar-style"));

        return s;
    }
};

class TrajectoryStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<TrajectoryStyle> s = boost::shared_ptr<TrajectoryStyle>(new TrajectoryStyle());

        s->currentTurnLine = object.get("current-turn-line", Json::Value()).asString();
        s->turnLine = object.get("turn-line", Json::Value()).asString();
        s->currentTurnPoint = object.get("current-turn-point", Json::Value()).asString();
        s->turnPoint = object.get("turn-point", Json::Value()).asString();
        return s;
    }
};
}

InfoWidgetStyle::InfoWidgetStyle():
    iconSize(0)
{
}

InteractionWidgetStyle::InteractionWidgetStyle()
{
}

TrajectoryStyle::TrajectoryStyle()
{

}

SkinManager::SkinManager()
{
    ResourceManager::instance().objectManager().addFactory("info-widget-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new InfoWidgetStyleFactory()));
    ResourceManager::instance().objectManager().addFactory("interaction-widget-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new InteractionWidgetStyleFactory()));
    ResourceManager::instance().objectManager().addFactory("trajectory-style", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new TrajectoryStyleFactory()));
}

void SkinManager::loadStyles()
{
    std::string skinDir = Engine::instance().properties()->get<std::string>("world.skin", "/world/skin");

    m_infoWidgetStyle = ResourceManager::instance().objectManager().getObject<InfoWidgetStyle>(skinDir + "/info-widget");
    m_trajectoryStyle = ResourceManager::instance().objectManager().getObject<TrajectoryStyle>(skinDir + "/trajectory");
}

boost::shared_ptr<InfoWidgetStyle> SkinManager::infoWidgetStyle() const
{
    return m_infoWidgetStyle;
}

boost::shared_ptr<TrajectoryStyle> SkinManager::trajectoryStyle() const
{
    return m_trajectoryStyle;
}

}
}