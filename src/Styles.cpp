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

#include "OpenSR/Styles.h"

namespace Rangers
{
ResourceDescriptor::ResourceDescriptor(): type(NONE)
{
}

ResourceDescriptor::ResourceDescriptor(const NinePatchDescriptor& ninePatch):
    type(NINEPATCH), resource(ninePatch)
{
}

ResourceDescriptor::ResourceDescriptor(const FontDescriptor& font):
    type(FONT), resource(font)
{
}

ResourceDescriptor::ResourceDescriptor(const TextureRegionDescriptor& texture):
    type(SPRITE), resource(texture)
{
}

ResourceDescriptor::ResourceDescriptor(const std::wstring& sound):
    type(SOUND), resource(sound)
{
}

TextureRegionDescriptor ResourceDescriptor::texture() const
{
    return boost::get<TextureRegionDescriptor>(resource);
}

NinePatchDescriptor ResourceDescriptor::ninePatch() const
{
    return boost::get<NinePatchDescriptor>(resource);
}

FontDescriptor ResourceDescriptor::font() const
{
    return boost::get<FontDescriptor>(resource);
}

std::wstring ResourceDescriptor::sound() const
{
    return boost::get<std::wstring>(resource);
}
}
