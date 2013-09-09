/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_SKINMANAGER_H
#define WORLD_SKINMANAGER_H

#include <OpenSR/Styles.h>
#include <OpenSR/Types.h>

namespace Rangers
{
namespace World
{
struct InfoWidgetStyle
{
    InfoWidgetStyle();

    ResourceDescriptor background;
    ResourceDescriptor font;
    ResourceDescriptor captionFont;
    int color;
    int captionColor;
    int labelColor;
    Vector iconPosition;
    Vector raceIconPosition;
    int iconSize;
    Rect contentRect;
    Rect captionContentRect;
};

class SkinManager
{
public:
    SkinManager();

    void loadStyles();

    InfoWidgetStyle infoWidgetStyle() const;

private:
    InfoWidgetStyle m_infoWidgetStyle;
};
}
}

#endif // WORLD_SKINMANAGER_H
