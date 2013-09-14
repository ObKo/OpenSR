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

#ifndef WORLD_INFOWIDGET_H
#define WORLD_INFOWIDGET_H

#include "OpenSR/World/world-global.h"

#include <OpenSR/Widget.h>
#include <OpenSR/Types.h>

namespace Rangers
{
class Sprite;
class Label;
class Font;
namespace World
{
struct InfoWidgetStyle;
class Planet;
class PlanetarySystem;
class RANGERS_WORLD_API InfoWidget: public Widget
{
public:
    InfoWidget(const InfoWidgetStyle& style);

    void clear();

    void showPlanet(boost::shared_ptr<Planet> planet);
    void showSystem(boost::shared_ptr<PlanetarySystem> system);

    virtual void processMain();
    virtual void draw() const;

    virtual bool containsPoint(const Vector &p) const;

private:
    enum InfoType {INFO_NONE, INFO_SYSTEM, INFO_PLANET};

    boost::shared_ptr<Font> m_font;
    boost::shared_ptr<Font> m_captionFont;
    boost::shared_ptr<Label> m_caption;
    Rect m_contentRect;
    Rect m_captionContentRect;
    boost::shared_ptr<Sprite> m_bgSprite;
    boost::shared_ptr<Sprite> m_iconSprite;
    boost::shared_ptr<Sprite> m_raceIconSprite;
    int m_color;
    int m_captionColor;
    int m_labelColor;
    std::vector<boost::shared_ptr<Object> > m_infoWidget;
    InfoType m_type;
    int m_iconSize;
    Vector m_iconPosition;
    Vector m_raceIconPosition;
};
}
}

#endif
