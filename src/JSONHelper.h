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

#include <jsoncpp/json.h>
#include "Types.h"

namespace Rangers
{
class JSONHelper
{
public:
    static TextureRegion parseTextureRegion(const Json::Value& object);
    static NinePatchDescriptor parseNinePatch(const Json::Value& object);
    static NinePatchDescriptor parseNinePatch(const std::string& json);
};
}

#endif // RANGERS_JSONHELPER_H
