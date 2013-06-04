/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#include "Types.h"
#include <boost/variant.hpp>

//TODO: Descriptor for sound
namespace Rangers
{
class Texture;
struct ButtonStyle;
struct ScrollBarStyle;
struct FontDescriptor;
struct TextureRegionDescriptor;

struct RANGERS_ENGINE_API ResourceDescriptor
{
    ResourceDescriptor();
    ResourceDescriptor(const NinePatchDescriptor& ninePatch);
    ResourceDescriptor(const FontDescriptor& font);
    ResourceDescriptor(const TextureRegionDescriptor& texture);
    ResourceDescriptor(const std::wstring& sound);

    enum Type {NONE, NINEPATCH, SPRITE, FONT, SOUND};
    typedef boost::variant<TextureRegionDescriptor, NinePatchDescriptor, FontDescriptor, std::wstring> Resource;

    Type type;
    Resource resource;

    TextureRegionDescriptor texture() const;
    NinePatchDescriptor ninePatch() const;
    FontDescriptor font() const;
    std::wstring sound() const;
};

struct RANGERS_ENGINE_API ButtonStyle
{
    ResourceDescriptor normal;
    ResourceDescriptor hovered;
    ResourceDescriptor pressed;
    int color;
    FontDescriptor font;
    Rect contentRect;
    std::wstring clickSound;
    std::wstring leaveSound;
    std::wstring enterSound;
};

struct RANGERS_ENGINE_API CheckBoxStyle
{
    ResourceDescriptor normal;
    ResourceDescriptor hovered;
    ResourceDescriptor checkedNormal;
    ResourceDescriptor checkedHovered;
    int color;
    FontDescriptor font;
};

struct RANGERS_ENGINE_API RadioButtonStyle
{
    ResourceDescriptor normal;
    ResourceDescriptor hovered;
    ResourceDescriptor checkedNormal;
    ResourceDescriptor checkedHovered;
    int color;
    FontDescriptor font;
};

struct RANGERS_ENGINE_API LineEditStyle
{
    ResourceDescriptor background;
    int color;
    FontDescriptor font;
    Rect contentRect;
};

struct RANGERS_ENGINE_API ScrollBarStyle
{
    ButtonStyle upButton;
    ButtonStyle scroll;
    ButtonStyle downButton;
};

struct RANGERS_ENGINE_API Skin
{
    ButtonStyle buttonStyle;
    ScrollBarStyle scrollStyle;
    LineEditStyle lineEditStyle;
    CheckBoxStyle checkBoxStyle;
};

}

#endif
