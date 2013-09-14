/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/JSONHelper.h"
#include "OpenSR/Log.h"
#include "OpenSR/Texture.h"
#include <boost/shared_ptr.hpp>
#include "OpenSR/ResourceManager.h"

#include <OpenSR/libRanger.h>

#include <vector>

//TODO: Need error handling.
namespace Rangers
{
TextureRegionDescriptor JSONHelper::parseTextureRegion(const Json::Value& object, bool &error)
{
    TextureRegionDescriptor result;
    result.x = 0;
    result.y = 0;
    result.height = 0;
    result.width = 0;
    error = false;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return result;
    }
    result.texture = fromUTF8(object.get("texture", "").asString().c_str());

    result.x = object.get("x", 0).asInt();
    result.y = object.get("y", 0).asInt();
    result.width = object.get("width", -1).asInt();
    result.height = object.get("height", -1).asInt();

    return result;
}

FontDescriptor JSONHelper::parseFont(const Json::Value& object, bool &error)
{
    error = false;
    FontDescriptor font;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return font;
    }
    Json::Value::Members members = object.getMemberNames();
    if ((std::find(members.begin(), members.end(), "file") == members.end())
            || (std::find(members.begin(), members.end(), "size") == members.end()))
    {
        Log::warning() << "Invalid JSON font.";
        error = true;
        return font;
    }
    font.path = fromUTF8(object.get("file", "").asString().c_str());
    font.size = object.get("size", "").asInt();

    return font;
}

std::wstring JSONHelper::parseSound(const Json::Value& object, bool &error)
{
    error = false;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return std::wstring();
    }
    Json::Value::Members members = object.getMemberNames();
    if ((std::find(members.begin(), members.end(), "file") == members.end()))
    {
        Log::warning() << "Invalid JSON sound.";
        error = true;
        return std::wstring();
    }
    return fromUTF8(object.get("file", "").asString().c_str());
}

NinePatchDescriptor JSONHelper::parseNinePatch(const Json::Value& object, bool &error)
{
    error = false;
    NinePatchDescriptor result;
    result.rows = 0;
    result.columns = 0;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return result;
    }
    result.rows = object.get("rows", 0).asInt();
    result.columns = object.get("columns", 0).asInt();
    Json::Value array = object.get("sizeableRows", 0);
    Json::Value::iterator end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.sizeableRows.push_back((*i).asInt());
    }
    array = object.get("sizeableColumns", 0);
    end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.sizeableColumns.push_back((*i).asInt());
    }
    array = object.get("regions", 0);
    end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.regions.push_back(parseTextureRegion(*i, error));
    }
    array = object.get("tiled", Json::Value(Json::arrayValue));
    end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.tiledRegions.push_back((*i).asInt());
    }
    return result;
}

NinePatchDescriptor JSONHelper::parseNinePatch(const std::string& json, bool &error)
{
    error = false;
    NinePatchDescriptor result;
    result.rows = 0;
    result.columns = 0;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return result;
    }
    Json::Value::Members members = root.getMemberNames();
    if (std::find(members.begin(), members.end(), "NinePatch") == members.end())
    {
        Log::error() << "Invalid NinePatch JSON";
        error = true;
        return result;
    }

    return parseNinePatch(*root.get("NinePatch", 0).begin(), error);
}

NinePatchDescriptor JSONHelper::parseNinePatch(std::istream& json, bool &error)
{
    error = false;
    NinePatchDescriptor result;
    result.rows = 0;
    result.columns = 0;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return result;
    }
    Json::Value::Members members = root.getMemberNames();
    if (std::find(members.begin(), members.end(), "NinePatch") == members.end())
    {
        Log::error() << "Invalid NinePatch JSON";
        error = true;
        return result;
    }

    return parseNinePatch(*root.get("NinePatch", 0).begin(), error);
}

std::map<std::wstring, ResourceDescriptor> JSONHelper::parseResources(const Json::Value& object, bool &error)
{
    std::map<std::wstring, ResourceDescriptor> result;
    error = false;
	if (object.isMember("NinePatch"))
    {
        Json::Value ninepatches = object.get("NinePatch", Json::Value());
        Json::Value::iterator end = ninepatches.end();
        for (Json::Value::iterator it = ninepatches.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: " << fromUTF8(it.memberName());
                return std::map<std::wstring, ResourceDescriptor>();
            }

            NinePatchDescriptor ninepatch = parseNinePatch(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::NINEPATCH;
            desc.resource = ResourceDescriptor::Resource(ninepatch);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    if (object.isMember("Sprite"))
    {
        Json::Value sprites = object.get("Sprite", Json::Value());
        Json::Value::iterator end = sprites.end();
        for (Json::Value::iterator it = sprites.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: \"" << fromUTF8(it.memberName()) << "\"";
                return std::map<std::wstring, ResourceDescriptor>();
            }

            TextureRegionDescriptor sprite = parseTextureRegion(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::SPRITE;
            desc.resource = ResourceDescriptor::Resource(sprite);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    if (object.isMember("Font"))
    {
        Json::Value sprites = object.get("Font", Json::Value());
        Json::Value::iterator end = sprites.end();
        for (Json::Value::iterator it = sprites.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: \"" << fromUTF8(it.memberName()) << "\"";
                return std::map<std::wstring, ResourceDescriptor>();
            }

            FontDescriptor font = parseFont(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::FONT;
            desc.resource = ResourceDescriptor::Resource(font);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    if (object.isMember("Sound"))
    {
        Json::Value sounds = object.get("Sound", Json::Value());
        Json::Value::iterator end = sounds.end();
        for (Json::Value::iterator it = sounds.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: \"" << fromUTF8(it.memberName()) << "\"";
                return std::map<std::wstring, ResourceDescriptor>();
            }

            std::wstring sound = parseSound(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::SOUND;
            desc.resource = ResourceDescriptor::Resource(sound);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    return result;
}


Skin JSONHelper::parseSkin(const Json::Value& root, bool &error)
{
    Skin skin;
    std::map<std::wstring, ResourceDescriptor> resources = parseResources(root, error);
    Json::Value::Members members = root.getMemberNames();
    if (std::find(members.begin(), members.end(), "ScrollBarStyle") != members.end())
    {
        Json::Value style = root.get("ScrollBarStyle", Json::Value());
        Json::Value::Members styleMembers = style.getMemberNames();
        if ((std::find(styleMembers.begin(), styleMembers.end(), "up-button") == styleMembers.end())
                || (std::find(styleMembers.begin(), styleMembers.end(), "down-button") == styleMembers.end())
                || (std::find(styleMembers.begin(), styleMembers.end(), "scroll") == styleMembers.end()))
        {
            error = true;
            Log::error() << "Invalid JSON ScrollBarStyle.";
        }
        else
        {
            skin.scrollStyle.downButton = parseButtonStyle(style.get("down-button", Json::Value()), resources, error);
            skin.scrollStyle.upButton = parseButtonStyle(style.get("up-button", Json::Value()), resources, error);
            skin.scrollStyle.scroll = parseButtonStyle(style.get("scroll", Json::Value()), resources, error);
        }
    }
    if (std::find(members.begin(), members.end(), "ButtonStyle") != members.end())
    {
        Json::Value style = root.get("ButtonStyle", Json::Value());
        skin.buttonStyle = parseButtonStyle(style, resources, error);
    }
    if (std::find(members.begin(), members.end(), "LineEditStyle") != members.end())
    {
        Json::Value style = root.get("LineEditStyle", Json::Value());
        skin.lineEditStyle = parseLineEditStyle(style, resources, error);
    }
    if (std::find(members.begin(), members.end(), "CheckBoxStyle") != members.end())
    {
        Json::Value style = root.get("CheckBoxStyle", Json::Value());
        skin.checkBoxStyle = parseCheckBoxStyle(style, resources, error);
    }
    if (std::find(members.begin(), members.end(), "RadioButtonStyle") != members.end())
    {
        Json::Value style = root.get("RadioButtonStyle", Json::Value());
        skin.radioButtonStyle = parseRadioButtonStyle(style, resources, error);
    }
    return skin;
}

Skin JSONHelper::parseSkin(const std::string& json, bool &error)
{
    error = false;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return Skin();
    }
    Skin s = parseSkin(root, error);
    if (error)
        return Skin();
    return s;
}

Skin JSONHelper::parseSkin(std::istream& json, bool &error)
{
    error = false;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return Skin();
    }
    Skin s = parseSkin(root, error);
    if (error)
        return Skin();
    return s;
}

ButtonStyle JSONHelper::parseButtonStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error)
{
    error = false;
    ButtonStyle style;
    Json::Value::Members members = object.getMemberNames();

    style.normal  = getResource(object.get("normal", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "hovered") != members.end())
        style.hovered = getResource(object.get("hovered", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "pressed") != members.end())
        style.pressed = getResource(object.get("pressed", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "font") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("font", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::FONT)
            error = true;
        else
            style.font = boost::get<FontDescriptor>(d.resource);
    }

    if (std::find(members.begin(), members.end(), "color") != members.end())
        style.color = parseColor(object.get("color", "#").asString(), error);

    if (std::find(members.begin(), members.end(), "content-rect") != members.end())
        style.contentRect = parseRect(object.get("content-rect", Json::Value()), error);

    if (std::find(members.begin(), members.end(), "enter-sound") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("enter-sound", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::SOUND)
            error = true;
        else
            style.enterSound = boost::get<std::wstring>(d.resource);
    }
    if (std::find(members.begin(), members.end(), "leave-sound") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("leave-sound", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::SOUND)
            error = true;
        else
            style.leaveSound = boost::get<std::wstring>(d.resource);
    }
    if (std::find(members.begin(), members.end(), "click-sound") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("click-sound", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::SOUND)
            error = true;
        else
            style.clickSound = boost::get<std::wstring>(d.resource);
    }

    if (error)
    {
        Log::error() << "Invalid JSON ButtonStyle";
        return ButtonStyle();
    }
    return style;
}

CheckBoxStyle JSONHelper::parseCheckBoxStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error)
{
    error = false;
    CheckBoxStyle style;
    Json::Value::Members members = object.getMemberNames();

    style.normal  = getResource(object.get("normal", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "hovered") != members.end())
        style.hovered = getResource(object.get("hovered", "").asString(), resources, error);

    style.checkedNormal = getResource(object.get("checked-normal", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "checked-hovered") != members.end())
        style.checkedHovered = getResource(object.get("checked-hovered", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "font") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("font", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::FONT)
            error = true;
        else
            style.font = boost::get<FontDescriptor>(d.resource);
    }

    if (std::find(members.begin(), members.end(), "color") != members.end())
        style.color = parseColor(object.get("color", "#").asString(), error);

    if (error)
    {
        Log::error() << "Invalid JSON CheckBoxStyle";
        return CheckBoxStyle();
    }
    return style;
}

RadioButtonStyle JSONHelper::parseRadioButtonStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error)
{
    error = false;
    RadioButtonStyle style;
    Json::Value::Members members = object.getMemberNames();

    style.normal  = getResource(object.get("normal", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "hovered") != members.end())
        style.hovered = getResource(object.get("hovered", "").asString(), resources, error);

    style.selectedNormal = getResource(object.get("selected-normal", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "selected-hovered") != members.end())
        style.selectedHovered = getResource(object.get("selected-hovered", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "font") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("font", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::FONT)
            error = true;
        else
            style.font = boost::get<FontDescriptor>(d.resource);
    }

    if (std::find(members.begin(), members.end(), "color") != members.end())
        style.color = parseColor(object.get("color", "#").asString(), error);

    if (error)
    {
        Log::error() << "Invalid JSON RadioButtonStyle";
        return RadioButtonStyle();
    }
    return style;
}

LineEditStyle JSONHelper::parseLineEditStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error)
{
    error = false;
    LineEditStyle style;
    Json::Value::Members members = object.getMemberNames();

    style.background = getResource(object.get("background", "").asString(), resources, error);

    if (std::find(members.begin(), members.end(), "font") != members.end())
    {
        ResourceDescriptor d = getResource(object.get("font", "").asString(), resources, error);

        if (d.type != ResourceDescriptor::FONT)
            error = true;
        else
            style.font = boost::get<FontDescriptor>(d.resource);
    }

    if (std::find(members.begin(), members.end(), "color") != members.end())
        style.color = parseColor(object.get("color", "#").asString(), error);

    if (std::find(members.begin(), members.end(), "content-rect") != members.end())
        style.contentRect = parseRect(object.get("content-rect", Json::Value()), error);

    if (error)
    {
        Log::error() << "Invalid JSON LineEditStyle";
        return LineEditStyle();
    }
    return style;
}

int JSONHelper::parseColor(const std::string& string, bool &error)
{
    if (error)
        return 0;
    std::istringstream ss(string);
    uint32_t color;
    char c;
    ss >> c >> std::hex >> color;
    if (ss.fail())
    {
        error = true;
        Log::error() << "Invalid JSON color.";
        color = 0;
    }
    return color;
}

ResourceDescriptor JSONHelper::getResource(const std::string& name, const std::map<std::wstring, ResourceDescriptor>& resources, bool &error)
{
    if (error)
        return ResourceDescriptor();
    if (name.empty())
    {
        error = true;
        return ResourceDescriptor();
    }
    std::wstring n = fromUTF8(name.c_str());
    if (resources.find(n) == resources.end())
    {
        error = true;
        Log::error() << "No such JSON resource: \"" << n << "\"";
        return ResourceDescriptor();
    }
    return resources.at(n);
}

Rect JSONHelper::parseRect(const Json::Value& object, bool &error)
{
    if (error)
        return Rect();

    Rect rect;
    rect.x = object.get("x", 0).asDouble();
    rect.y = object.get("y", 0).asDouble();
    rect.width = object.get("width", -1).asDouble();
    rect.height = object.get("height", -1).asDouble();
    return rect;
}

Vector JSONHelper::parseVector(const Json::Value& object, bool &error)
{
    if (error)
        return Vector();

    Vector v;
    v.x = object.get("x", 0).asDouble();
    v.y = object.get("y", 0).asDouble();
    return v;
}

}

