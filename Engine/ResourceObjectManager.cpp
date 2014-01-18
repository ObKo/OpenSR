/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/ResourceObjectManager.h"

#include <boost/algorithm/string.hpp>

#include <json/json.h>
#include <json/value.h>
#include <OpenSR/libRanger.h>
#include "OpenSR/Log.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Styles.h"

namespace Rangers
{
class ResourceObjectManager::DirectoryFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        for (const std::string& s : object.getMemberNames())
        {
            if (s == "@type")
                continue;

            manager.loadObject(currentPath + "/" + s, object.get(s, Json::Value()));
        }
        manager.m_directories.push_back(currentPath);
        return boost::shared_ptr< ResourceObject >(new ResourceObject());
    }
};

namespace
{
class TextureFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        TextureRegionDescriptor *d = new TextureRegionDescriptor();

        d->texture = fromUTF8(object.get("texture", "").asString().c_str());
        d->x = object.get("x", 0).asInt();
        d->y = object.get("y", 0).asInt();
        d->width = object.get("width", -1).asInt();
        d->height = object.get("height", -1).asInt();

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class FontFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        FontDescriptor *d = new FontDescriptor();

        d->path = fromUTF8(object.get("file", "").asString().c_str());
        d->size = object.get("size", 0).asInt();
        d->antialiasing = object.get("antialiasing", true).asBool();

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class SoundFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        SoundDescriptor *d = new SoundDescriptor();
        d->path = fromUTF8(object.get("file", "").asString().c_str());
        return boost::shared_ptr< ResourceObject >(d);
    }
};

class NinePatchFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        NinePatchDescriptor *d = new NinePatchDescriptor();

        d->rows = object.get("rows", 0).asInt();
        d->columns = object.get("columns", 0).asInt();

        Json::Value array = object.get("sizeableRows", 0);
        for (const Json::Value& v : array)
            d->sizeableRows.push_back(v.asInt());

        array = object.get("sizeableColumns", 0);
        for (const Json::Value& v : array)
            d->sizeableColumns.push_back(v.asInt());

        array = object.get("regions", 0);
        for (const Json::Value& v : array)
            d->regions.push_back(boost::dynamic_pointer_cast<TextureRegionDescriptor>(manager.loadObject(v, "texture")));

        array = object.get("tiled", Json::Value(Json::arrayValue));
        for (const Json::Value& v : array)
            d->tiledRegions.push_back(v.asInt());

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class ButtonStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        ButtonStyle *d = new ButtonStyle();

        d->normal = manager.loadObject(object.get("normal", Json::Value()), "texture");
        d->hovered = manager.loadObject(object.get("hovered", Json::Value()), "texture");
        d->pressed = manager.loadObject(object.get("pressed", Json::Value()), "texture");

        d->font = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("font", Json::Value()), "font"));

        boost::shared_ptr<SoundDescriptor> sound;
        sound = boost::dynamic_pointer_cast<SoundDescriptor>(manager.loadObject(object.get("enter-sound", Json::Value()), "sound"));
        if (sound)
            d->enterSound = sound->path;
        sound = boost::dynamic_pointer_cast<SoundDescriptor>(manager.loadObject(object.get("leave-sound", Json::Value()), "sound"));
        if (sound)
            d->leaveSound = sound->path;
        sound = boost::dynamic_pointer_cast<SoundDescriptor>(manager.loadObject(object.get("click-sound", Json::Value()), "sound"));
        if (sound)
            d->clickSound = sound->path;

        d->color = parseColor(object.get("color", "#FFFFFF"));
        d->contentRect = parseRect(object.get("content-rect", Json::Value()));

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class CheckBoxStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        CheckBoxStyle *d = new CheckBoxStyle();

        d->normal = manager.loadObject(object.get("normal", Json::Value()), "texture");
        d->hovered = manager.loadObject(object.get("hovered", Json::Value()), "texture");
        d->checkedNormal = manager.loadObject(object.get("checked-normal", Json::Value()), "texture");
        d->checkedHovered = manager.loadObject(object.get("checked-hovered", Json::Value()), "texture");
        d->font = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("font", Json::Value()), "font"));
        d->color = parseColor(object.get("color", "#FFFFFF"));

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class ScrollBarStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        ScrollBarStyle *d = new ScrollBarStyle();

        d->scroll = boost::dynamic_pointer_cast<ButtonStyle>(manager.loadObject(object.get("scroll", Json::Value()), "button-style"));
        d->upButton = boost::dynamic_pointer_cast<ButtonStyle>(manager.loadObject(object.get("up-button", Json::Value()), "button-style"));
        d->downButton = boost::dynamic_pointer_cast<ButtonStyle>(manager.loadObject(object.get("down-button", Json::Value()), "button-style"));

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class RadioButtonStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        RadioButtonStyle *d = new RadioButtonStyle();

        d->normal = manager.loadObject(object.get("normal", Json::Value()), "texture");
        d->hovered = manager.loadObject(object.get("hovered", Json::Value()), "texture");
        d->selectedNormal = manager.loadObject(object.get("selected-normal", Json::Value()), "texture");
        d->selectedHovered = manager.loadObject(object.get("selected-hovered", Json::Value()), "texture");
        d->font = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("font", Json::Value()), "font"));
        d->color = parseColor(object.get("color", "#FFFFFF"));

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class LineEditStyleFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        LineEditStyle *d = new LineEditStyle();

        d->background = manager.loadObject(object.get("background", Json::Value()), "texture");
        d->font = boost::dynamic_pointer_cast<FontDescriptor>(manager.loadObject(object.get("font", Json::Value()), "font"));
        d->color = parseColor(object.get("color", "#FFFFFF"));
        d->contentRect = parseRect(object.get("content-rect", Json::Value()));

        return boost::shared_ptr< ResourceObject >(d);
    }
};

class SkinFactory: public ResourceObjectManager::ResourceObjectFactory
{
public:
    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        Skin *d = new Skin();

        d->buttonStyle = boost::dynamic_pointer_cast<ButtonStyle>(manager.loadObject(object.get("button", Json::Value()), "button-style"));
        d->checkBoxStyle = boost::dynamic_pointer_cast<CheckBoxStyle>(manager.loadObject(object.get("check-box", Json::Value()), "check-box-style"));
        d->lineEditStyle = boost::dynamic_pointer_cast<LineEditStyle>(manager.loadObject(object.get("line-edit", Json::Value()), "line-edit-style"));
        d->radioButtonStyle = boost::dynamic_pointer_cast<RadioButtonStyle>(manager.loadObject(object.get("radio-button", Json::Value()), "radio-button-style"));
        d->scrollStyle = boost::dynamic_pointer_cast<ScrollBarStyle>(manager.loadObject(object.get("scroll-bar", Json::Value()), "scroll-bar-style"));

        return boost::shared_ptr< ResourceObject >(d);
    }
};
}

Color ResourceObjectManager::ResourceObjectFactory::parseColor(const Json::Value& object)
{
    if (!object.isString())
        return Color();

    return Color::fromString(object.asString());
}

Rect ResourceObjectManager::ResourceObjectFactory::parseRect(const Json::Value& object)
{
    if (!object.isObject())
        return Rect();

    Rect r;
    r.x = object.get("x", 0).asDouble();
    r.y = object.get("y", 0).asDouble();
    r.width = object.get("width", -1).asDouble();
    r.height = object.get("height", -1).asDouble();
    return r;
}

Vector ResourceObjectManager::ResourceObjectFactory::parseVector(const Json::Value& object)
{
    if (!object.isObject())
        return Vector();

    Vector v;
    v.x = object.get("x", 0).asDouble();
    v.y = object.get("y", 0).asDouble();
    return v;
}


ResourceObjectManager::ResourceObjectManager()
{
    registerDefaultFactories();
}

void ResourceObjectManager::registerDefaultFactories()
{
    addFactory("directory", boost::shared_ptr<ResourceObjectFactory>(new DirectoryFactory()));
    addFactory("texture", boost::shared_ptr<ResourceObjectFactory>(new TextureFactory()));
    addFactory("font", boost::shared_ptr<ResourceObjectFactory>(new FontFactory()));
    addFactory("sound", boost::shared_ptr<ResourceObjectFactory>(new SoundFactory()));
    addFactory("ninepatch", boost::shared_ptr<ResourceObjectFactory>(new NinePatchFactory()));
    addFactory("button-style", boost::shared_ptr<ResourceObjectFactory>(new ButtonStyleFactory()));
    addFactory("line-edit-style", boost::shared_ptr<ResourceObjectFactory>(new LineEditStyleFactory()));
    addFactory("check-box-style", boost::shared_ptr<ResourceObjectFactory>(new CheckBoxStyleFactory()));
    addFactory("radio-button-style", boost::shared_ptr<ResourceObjectFactory>(new RadioButtonStyleFactory()));
    addFactory("scroll-bar-style", boost::shared_ptr<ResourceObjectFactory>(new ScrollBarStyleFactory()));
    addFactory("skin", boost::shared_ptr<ResourceObjectFactory>(new SkinFactory()));
}

void ResourceObjectManager::addJSON(const std::wstring& jsonFile, const std::string& prefix)
{
    boost::shared_ptr<std::istream> f = ResourceManager::instance().getFileStream(jsonFile);
    if (!f)
        return;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*f, root))
    {
        Log::error() << L"[ROM] Error parsing JSON file " << jsonFile << ": " << reader.getFormatedErrorMessages();
        return;
    }
    loadObject(prefix, root);
}

boost::shared_ptr< ResourceObject > ResourceObjectManager::getResourceObject(const std::string& id)
{
    if (id.empty())
        return boost::shared_ptr< ResourceObject >();

    // Check that object already loaded.
    auto oi = m_objects.find(id);
    if (oi != m_objects.end())
        return oi->second;

    // Check that object wasn't loaded, but there is JSON file for it
    auto fi = m_objectFiles.find(id);
    if (fi != m_objectFiles.end())
    {
        boost::shared_ptr<std::istream> f = ResourceManager::instance().getFileStream(fi->second);
        if (!f)
            return boost::shared_ptr< ResourceObject >();
        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(*f, root))
        {
            Log::error() << L"[ROM] Error parsing JSON file " << fi->second << ": " << reader.getFormatedErrorMessages();
            return boost::shared_ptr< ResourceObject >();
        }
        m_objectFiles.erase(fi->first);
        return loadObject(id, root);
    }

    // OK, no object, no file, so check that directory that may contains object was loaded.

    std::string dir = directory(id);
    //Remove trailing slash
    if (!dir.empty())
        dir = dir.substr(0, dir.length() - 1);

    if (dir.empty())
    {
        Log::error() << "[ROM] Cannot find object " << id;
        return boost::shared_ptr< ResourceObject >();
    }

    auto di = std::find(m_directories.begin(), m_directories.end(), dir);
    if (di != m_directories.end())
    {
        // Directory loaded, but there is no object
        Log::error() << "[ROM] Cannot find object " << id;
        return boost::shared_ptr< ResourceObject >();
    }

    // Load dir recursively
    if (!getResourceObject(dir))
        return boost::shared_ptr< ResourceObject >();

    return getResourceObject(id);
}

boost::shared_ptr< ResourceObject > ResourceObjectManager::loadObject(const std::string& currentPath, const Json::Value& object)
{
    if (object.isNull())
        return boost::shared_ptr< ResourceObject >();

    if (object.isString())
    {
        std::string path = object.asString();
        if (path[0] == ':')
        {
            m_objectFiles[currentPath] = fromUTF8(path.substr(1).c_str());
            return boost::shared_ptr< ResourceObject >();
        }
        else
        {
            return getResourceObject(path);
        }
    }
    if (!object.isMember("@type"))
    {
        Log::error() << "[ROM] Invalid JSON object (no @type field)";
        return boost::shared_ptr< ResourceObject >();
    }
    std::string type = object.get("@type", "").asString();

    auto fi = m_objectFactories.find(type);
    if (fi == m_objectFactories.end())
    {
        Log::warning() << "[ROM] No object loader found for type " << type;
        return boost::shared_ptr< ResourceObject >();
    }
    boost::shared_ptr< ResourceObject > o = (*(fi->second))(currentPath, object, *this);
    if (o)
        m_objects[currentPath] = o;
    return o;
}

boost::shared_ptr< ResourceObject > ResourceObjectManager::loadObject(const Json::Value& object, const std::string& type)
{
    if (object.isNull())
        return boost::shared_ptr< ResourceObject >();

    if (object.isString())
    {
        std::string path = object.asString();
        return getResourceObject(path);
    }
    if (type.empty() && !object.isMember("@type"))
    {
        Log::error() << "[ROM] Invalid JSON object (no @type field)";
        return boost::shared_ptr< ResourceObject >();
    }
    std::string t = type;
    if (object.isMember("@type"))
        t = object.get("@type", "").asString();

    auto fi = m_objectFactories.find(t);
    if (fi == m_objectFactories.end())
    {
        Log::warning() << "[ROM] No object loader found for type " << t;
        return boost::shared_ptr< ResourceObject >();
    }
    return (*(fi->second))("", object, *this);
}

boost::shared_ptr< ResourceObject > ResourceObjectManager::loadObject(const std::wstring& jsonFile, const std::string& type)
{
    boost::shared_ptr<std::istream> f = ResourceManager::instance().getFileStream(jsonFile);
    if (!f)
        return boost::shared_ptr< ResourceObject >();
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*f, root))
    {
        Log::error() << L"[ROM] Error parsing JSON file " << jsonFile << ": " << reader.getFormatedErrorMessages();
        return boost::shared_ptr< ResourceObject >();
    }
    return loadObject(root, type);
}


void ResourceObjectManager::addFactory(const std::string& type, boost::shared_ptr< ResourceObjectFactory > factory)
{
    if (type.empty())
        return;
    m_objectFactories[type] = factory;
}

}
