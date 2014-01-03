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

#include <boost/python.hpp>
#include <OpenSR/LabelWidget.h>

#include <OpenSR/Label.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct LabelWidgetWrap : LabelWidget, boost::python::wrapper<LabelWidget>
{
    LabelWidgetWrap(boost::shared_ptr<Label> label = boost::shared_ptr<Label>()): LabelWidget(label)
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(LabelWidget)
};

void exportLabelWidget()
{
    using namespace boost::python;
    class_<LabelWidgetWrap, bases<Widget>, boost::shared_ptr<LabelWidgetWrap>, boost::noncopyable> c("LabelWidget", init<boost::shared_ptr<Label> >());
    c.def(init<>())
    .add_property("label", &LabelWidget::label, &LabelWidget::setLabel)
    .add_property("horizAlign", &LabelWidget::horizAlign, &LabelWidget::setHorizAlign)
    .add_property("vertAlign", &LabelWidget::vertAlign, &LabelWidget::setVertAlign);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(LabelWidget, LabelWidgetWrap, c);
    register_ptr_to_python<boost::shared_ptr<LabelWidget> >();

    scope s = c;

    enum_<LabelWidget::HorizontalAlignment>("HorizontalAlignment")
    .value("LEFT", LabelWidget::HALIGN_LEFT)
    .value("CENTER", LabelWidget::HALIGN_CENTER)
    .value("RIGHT", LabelWidget::HALIGN_RIGHT);

    enum_<LabelWidget::VerticalAlignment>("VerticalAlignment")
    .value("TOP", LabelWidget::VALIGN_TOP)
    .value("CENTER", LabelWidget::VALIGN_CENTER)
    .value("BOTTOM", LabelWidget::VALIGN_BOTTOM);
}
}
}
