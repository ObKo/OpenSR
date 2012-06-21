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

#ifndef RANGERS_JSONHELPER_H
#define RANGERS_JSONHELPER_H

#include <json/json.h>
#include <map>
#include "Types.h"
#include "Styles.h"

namespace Rangers
{
class JSONHelper
{
public:
    static TextureRegion parseTextureRegion(const Json::Value& object, bool &error);
    static NinePatchDescriptor parseNinePatch(const Json::Value& object, bool &error);
    static NinePatchDescriptor parseNinePatch(const std::string& json, bool &error);
    static FontDescriptor parseFont(const Json::Value& object, bool &error);
    static std::map<std::wstring, ResourceDescriptor> parseResources(const Json::Value& object, bool &error);
    static ButtonStyle parseButtonStyle(const Json::Value& object, const std::map<std::wstring, ResourceDescriptor>& resources, bool &error);
    static LineEditStyle parseLineEditStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error);
    static CheckBoxStyle parseCheckBoxStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error);
    static std::wstring parseSound(const Json::Value& object, bool &error);
    static Skin parseSkin(const std::string& json, bool &error);

private:
    static ResourceDescriptor getResource(const std::string& name, const std::map<std::wstring, ResourceDescriptor>& resources, bool &error);
    static int parseColor(const std::string& string, bool &error);
    static Rect parseRect(const Json::Value& object, bool &error);
};
}

#endif // RANGERS_JSONHELPER_H
