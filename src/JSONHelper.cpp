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

#include "JSONHelper.h"
#include "Log.h"
#include "Texture.h"
#include <boost/shared_ptr.hpp>
#include "ResourceManager.h"
#include <libRanger.h>
#include <vector>

//TODO: Need error handling.
namespace Rangers
{
TextureRegion JSONHelper::parseTextureRegion(const Json::Value& object, bool &error)
{
    error = false;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return TextureRegion(boost::shared_ptr<Texture>(), 0, 0, 0, 0);
    }
    std::wstring textureName = fromUTF8(object.get("texture", "").asString().c_str());
    boost::shared_ptr<Texture> texture;

    if (!textureName.empty())
        texture = ResourceManager::instance()->loadTexture(textureName);

    if (!texture)
        return TextureRegion(boost::shared_ptr<Texture>(), 0, 0, 0, 0);

    int x, y, width, height;

    x = object.get("x", 0).asInt();
    y = object.get("y", 0).asInt();
    width = object.get("width", texture->width()).asInt();
    height = object.get("height", texture->height()).asInt();

    return TextureRegion(texture, x, y, width, height);
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

std::map<std::wstring, ResourceDescriptor> JSONHelper::parseResources(const Json::Value& object, bool &error)
{
    std::map<std::wstring, ResourceDescriptor> result;
    error = false;
    Json::Value::Members members = object.getMemberNames();
    if (std::find(members.begin(), members.end(), "NinePatch") != members.end())
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
            desc.ninepatch = ninepatch;
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    if (std::find(members.begin(), members.end(), "Sprite") != members.end())
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

            TextureRegion sprite = parseTextureRegion(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::SPRITE;
            desc.region = sprite;
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    return result;
}

void JSONHelper::parseSkin(const std::string& json, bool &error)
{
    error = false;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return;
    }
    parseResources(root, error);
}
}
