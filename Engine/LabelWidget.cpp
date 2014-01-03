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

#include "OpenSR/LabelWidget.h"
#include "OpenSR/private/LabelWidget_p.h"

#include "OpenSR/Label.h"
#include "OpenSR/ColorLabel.h"

namespace Rangers
{
LabelWidgetPrivate::LabelWidgetPrivate():
    WidgetPrivate(), vAlign(LabelWidget::VALIGN_TOP), hAlign(LabelWidget::HALIGN_CENTER), newLabel(false)
{
}

LabelWidget::LabelWidget(boost::shared_ptr<Label> label):
    Widget(*(new LabelWidgetPrivate()))
{
    RANGERS_D(LabelWidget);
    setLabel(label);
}

LabelWidget::LabelWidget(LabelWidgetPrivate &p): Widget(p)
{
}

void LabelWidget::draw() const
{
    if (!prepareDraw())
        return;

    RANGERS_D(const LabelWidget);

    if (d->label)
        d->label->draw();

    endDraw();
}

void LabelWidget::processMain()
{
    Widget::processMain();

    RANGERS_D(LabelWidget);

    if (!d->label)
        return;

    if (d->newLabel)
    {
        if (d->label->needUpdate())
            d->label->processMain();
        d->newLabel = false;

        if (d->width == -1)
            d->width = d->label->width();
        if (d->height == -1)
            d->height = d->label->height();
    }

    Vector pos;
    switch (d->hAlign)
    {
    case HALIGN_LEFT:
        pos.x = 0.0f;
        break;
    case HALIGN_CENTER:
        pos.x = int(d->width - d->label->width()) / 2;
        break;
    case HALIGN_RIGHT:
        pos.x = int(d->width - d->label->width());
        break;
    }

    switch (d->vAlign)
    {
    case VALIGN_TOP:
        pos.y = 0.0f;
        break;
    case VALIGN_CENTER:
        pos.y = int(d->height - d->label->height()) / 2;
        break;
    case VALIGN_BOTTOM:
        pos.y = int(d->height - d->label->height());
        break;
    }

    d->label->setPosition(pos);
}

void LabelWidget::processLogic(int dt)
{
    RANGERS_D(LabelWidget);
    if (d->label)
    {
        d->label->processLogic(dt);
    }
}

boost::shared_ptr<Label> LabelWidget::label() const
{
    RANGERS_D(const LabelWidget);
    return d->label;
}

void LabelWidget::setLabel(boost::shared_ptr<Label> label)
{
    RANGERS_D(LabelWidget);
    if (d->label)
    {
        removeChild(d->label);
    }
    d->label = label;
    if (d->label)
    {
        d->width = -1;
        d->height = -1;
        d->newLabel = true;
        addChild(d->label);
    }
    else
    {
        d->width = 0;
        d->height = 0;
    }
    markToUpdate();
}

LabelWidget::HorizontalAlignment LabelWidget::horizAlign() const
{
    RANGERS_D(const LabelWidget);
    return d->hAlign;
}

LabelWidget::VerticalAlignment LabelWidget::vertAlign() const
{
    RANGERS_D(const LabelWidget);
    return d->vAlign;
}

void LabelWidget::setHorizAlign(LabelWidget::HorizontalAlignment align)
{
    RANGERS_D(LabelWidget);
    d->hAlign = align;
    markToUpdate();
}

void LabelWidget::setVertAlign(LabelWidget::VerticalAlignment align)
{
    RANGERS_D(LabelWidget);
    d->vAlign = align;
    markToUpdate();
}

}