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

#ifndef WORLD_SPACEINFOWIDGET_H
#define WORLD_SPACEINFOWIDGET_H

#include <OpenSR/Widget.h>
#include <OpenSR/Types.h>

namespace Rangers
{
class Sprite;
namespace World
{
struct InfoWidgetStyle;
class Planet;
class SpaceInfoWidget: public Widget
{
public:
    SpaceInfoWidget(const InfoWidgetStyle& style, Widget* parent = 0);
    virtual ~SpaceInfoWidget();

    void clear();

    void showPlanet(boost::shared_ptr<Planet> planet);

    virtual void processMain();
    virtual void draw() const;

private:
    Rect m_contentRect;
    Sprite *m_bgSprite;
};
}
}

#endif // WORLD_SPACEINFOWIDGET_H
