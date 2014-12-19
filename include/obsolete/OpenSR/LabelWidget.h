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

#ifndef RANGERS_LABELWIDGET_H
#define RANGERS_LABELWIDGET_H

#include "OpenSR/Widget.h"

namespace Rangers
{
class Label;
class LabelWidgetPrivate;
class RANGERS_ENGINE_API LabelWidget: public Widget
{
    RANGERS_DECLARE_PRIVATE(LabelWidget)
public:
    enum HorizontalAlignment {HALIGN_LEFT, HALIGN_RIGHT, HALIGN_CENTER};
    enum VerticalAlignment {VALIGN_TOP, VALIGN_BOTTOM, VALIGN_CENTER};

    LabelWidget(boost::shared_ptr<Label> label = boost::shared_ptr<Label>());

    virtual void draw() const;
    virtual void processMain();
    virtual void processLogic(int dt);

    boost::shared_ptr<Label> label() const;
    HorizontalAlignment horizAlign() const;
    VerticalAlignment vertAlign() const;

    void setLabel(boost::shared_ptr<Label> label);
    void setHorizAlign(HorizontalAlignment align);
    void setVertAlign(VerticalAlignment align);

protected:
    LabelWidget(LabelWidgetPrivate &p);

    RANGERS_DISABLE_COPY(LabelWidget)
};
}


#endif // RANGERS_LABELWIDGET_H
