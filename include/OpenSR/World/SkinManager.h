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

#ifndef WORLD_SKINMANAGER_H
#define WORLD_SKINMANAGER_H

#include "OpenSR/World/world-global.h"

#include <OpenSR/Styles.h>
#include <OpenSR/Types.h>

namespace Rangers
{
namespace World
{
struct RANGERS_WORLD_API InfoWidgetStyle: public ResourceObject
{
    InfoWidgetStyle();

    boost::shared_ptr<ResourceObject> background;
    boost::shared_ptr<FontDescriptor> font;
    boost::shared_ptr<FontDescriptor> captionFont;
    Color color;
    Color captionColor;
    Color labelColor;
    Vector iconPosition;
    Vector raceIconPosition;
    int iconSize;
    Rect contentRect;
    Rect captionContentRect;
};

class RANGERS_WORLD_API SkinManager
{
public:
    SkinManager();

    void loadStyles();

    boost::shared_ptr<InfoWidgetStyle> infoWidgetStyle() const;

private:
    boost::shared_ptr<InfoWidgetStyle> m_infoWidgetStyle;
};
}
}

#endif // WORLD_SKINMANAGER_H
