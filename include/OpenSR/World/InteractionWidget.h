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

#ifndef WORLD_INTERACTION_WIDGET_H
#define WORLD_INTERACTION_WIDGET_H

#include "world-global.h"

#include <OpenSR/Widget.h>

#include <tuple>
#include <vector>
#include <string>

namespace Rangers
{
class ColorLabel;
class LabelWidget;
class ScrollArea;
namespace World
{
//TODO: Add background
class RANGERS_WORLD_API InteractionWidget: public Widget
{
public:
    InteractionWidget();
    InteractionWidget(const Rect& textRect, const Rect& selectionRect);

    virtual void draw() const;
    virtual void processMain();

    //! Interaction text
    std::wstring text() const;
    //! Selections, in (text, enable) tuples
    std::vector<std::tuple<std::wstring, bool> > selections() const;

    void setText(const std::wstring& text);
    void setSelections(const std::vector<std::tuple<std::wstring, bool> >& selections);


private:
    class InteractionWidgetListener;

    boost::shared_ptr<InteractionWidgetListener> m_actionListener;

    Rect m_textRect;
    Rect m_selectionRect;

    Rect m_normalBoundingRect;

    std::wstring m_text;
    std::vector<std::tuple<std::wstring, bool> > m_selections;

    boost::shared_ptr<LabelWidget> m_textLabel;
    std::vector<boost::shared_ptr<LabelWidget> > m_selectionsWidget;
    boost::shared_ptr<ScrollArea> m_textArea;
    boost::shared_ptr<ScrollArea> m_selectionArea;
};
}
}
#endif // WORLD_INTERACTION_WIDGET_H
