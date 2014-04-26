/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_STYLES_H
#define RANGERS_STYLES_H

#include "OpenSR/Types.h"

#include <boost/variant.hpp>

//TODO: Descriptor for sound
namespace Rangers
{
class Texture;
struct ButtonStyle;
struct ScrollBarStyle;
struct FontDescriptor;
struct TextureRegionDescriptor;
struct ResourceObject;

struct RANGERS_ENGINE_API ButtonStyle: public ResourceObject
{
    boost::shared_ptr<ResourceObject> normal;
    boost::shared_ptr<ResourceObject> hovered;
    boost::shared_ptr<ResourceObject> pressed;
    Color color;
    boost::shared_ptr<FontDescriptor> font;
    Rect contentRect;
    std::string clickSound;
    std::string leaveSound;
    std::string enterSound;
};

struct RANGERS_ENGINE_API CheckBoxStyle: public ResourceObject
{
    boost::shared_ptr<ResourceObject> normal;
    boost::shared_ptr<ResourceObject> hovered;
    boost::shared_ptr<ResourceObject> checkedNormal;
    boost::shared_ptr<ResourceObject> checkedHovered;
    Color color;
    boost::shared_ptr<FontDescriptor> font;
};

struct RANGERS_ENGINE_API RadioButtonStyle: public ResourceObject
{
    boost::shared_ptr<ResourceObject> normal;
    boost::shared_ptr<ResourceObject> hovered;
    boost::shared_ptr<ResourceObject> selectedNormal;
    boost::shared_ptr<ResourceObject> selectedHovered;
    Color color;
    boost::shared_ptr<FontDescriptor> font;
};

struct RANGERS_ENGINE_API LineEditStyle: public ResourceObject
{
    boost::shared_ptr<ResourceObject> background;
    Color color;
    boost::shared_ptr<FontDescriptor> font;
    Rect contentRect;
};

struct RANGERS_ENGINE_API ScrollBarStyle: public ResourceObject
{
    boost::shared_ptr<ButtonStyle> upButton;
    boost::shared_ptr<ButtonStyle> scroll;
    boost::shared_ptr<ButtonStyle> downButton;
};

struct RANGERS_ENGINE_API Skin: public ResourceObject
{
    boost::shared_ptr<ButtonStyle> buttonStyle;
    boost::shared_ptr<ScrollBarStyle> scrollStyle;
    boost::shared_ptr<LineEditStyle> lineEditStyle;
    boost::shared_ptr<CheckBoxStyle> checkBoxStyle;
    boost::shared_ptr<RadioButtonStyle> radioButtonStyle;
};

}

#endif
