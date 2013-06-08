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

#include <boost/python.hpp>
#include <OpenSR/LineEditWidget.h>
#include <OpenSR/Styles.h>

#include "WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct LineEditWidgetWrap : LineEditWidget, boost::python::wrapper<LineEditWidgetWrap>
{
    LineEditWidgetWrap(float w, float h = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>(), Widget* parent = 0)
        : LineEditWidget(w, h, font, parent)
    {
    }

    LineEditWidgetWrap(Widget* parent = 0)
        : LineEditWidget(parent)
    {
    }

    LineEditWidgetWrap(const LineEditStyle& style, Widget* parent = 0)
        : LineEditWidget(style, parent)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(LineEditWidget)
};

void exportLineEditWidget()
{
    using namespace boost::python;

    class_<LineEditWidgetWrap, bases<Widget>, boost::noncopyable> c("LineEditWidget", init<float, float, boost::shared_ptr<Font>, Widget*>());
    c.def(init<float, float, boost::shared_ptr<Font> >())
    .def(init<float, float>())
    .def(init<float>())
    .def(init<const LineEditStyle&, Widget*>())
    .def(init<const LineEditStyle&>())
    .def(init<Widget*>())
    .def(init<>())
    .def("setText", &LineEditWidget::setText)
    .def("text", &LineEditWidget::text);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(LineEditWidget, LineEditWidgetWrap, c);
}
}
}