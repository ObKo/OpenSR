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

#ifndef WORLD_INFOWIDGETPROVIDER_H
#define WORLD_INFOWIDGETPROVIDER_H

#include <OpenSR/Types.h>
#include <list>
#include <utility>

namespace Rangers
{
class Widget;
struct TextureRegion;
class LabelWidget;
class SpriteWidget;
namespace World
{
class InfoWidgetStyle;
class WorldObject;

//! InfoWidget content
struct RANGERS_WORLD_API InfoWidgetContent
{
    InfoWidgetContent();
    virtual ~InfoWidgetContent() {};

    std::wstring caption;
    TextureRegion icon;
    uint32_t race;

    std::vector<boost::shared_ptr<Widget> > widgets;

    bool hasIcon;
    bool hasRace;
};

struct RANGERS_WORLD_API KeyValueContent: public InfoWidgetContent
{
    std::list<std::pair<boost::shared_ptr<LabelWidget>, boost::shared_ptr<LabelWidget> > > keyValueWidgets;
};

struct RANGERS_WORLD_API SystemContent: public InfoWidgetContent
{
    struct SystemObjectLine
    {
        boost::shared_ptr<LabelWidget> name;
        boost::shared_ptr<SpriteWidget> icon;
        boost::shared_ptr<SpriteWidget> raceIcon;
    };
    std::list<SystemObjectLine> objectWidgets;
};

//! Content provider for InfoWidget
class RANGERS_WORLD_API InfoWidgetProvider
{
public:
    //! Create content for InfoWidget.
    virtual boost::shared_ptr<InfoWidgetContent> createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const = 0;
    //! Update conent, e.g. widget positions.
    virtual void updateContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetContent> content, Rect &contentRect) const = 0;
};

class KeyValueInfoWidgetProvider: public InfoWidgetProvider
{
public:
    virtual void updateContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetContent> content, Rect &contentRect) const;

protected:
    void generateWidgets(boost::shared_ptr<KeyValueContent> content, boost::shared_ptr<InfoWidgetStyle> style, const std::list<std::pair<std::wstring, std::wstring> >& keyValue) const;
};

class AsteroidInfoWidgetProvider: public KeyValueInfoWidgetProvider
{
public:
    virtual boost::shared_ptr<InfoWidgetContent> createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const;
};

class PlanetInfoWidgetProvider: public KeyValueInfoWidgetProvider
{
public:
    virtual boost::shared_ptr<InfoWidgetContent> createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const;
};

class SystemInfoWidgetProvider: public InfoWidgetProvider
{
public:
    virtual boost::shared_ptr<InfoWidgetContent> createContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetStyle> style) const;
    virtual void updateContent(boost::shared_ptr<WorldObject> object, boost::shared_ptr<InfoWidgetContent> content, Rect &contentRect) const;
};
}
}

#endif
