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
#include <OpenSR/Widget.h>
#include <OpenSR/Action.h>
#include <OpenSR/ActionListener.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct WidgetWrap : Widget, boost::python::wrapper<Widget>
{
    WidgetWrap(Widget *parent = 0): Widget(parent)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(Widget)
};

void exportWidget()
{
    using namespace boost::python;
    class_<WidgetWrap, bases<Object>, boost::noncopyable> c("Widget", init<Widget*>());
    c.def(init<>())
    .def("width", &Widget::width)
    .def("height", &Widget::height)
    .def("isFocused", &Widget::isFocused)
    .def("isVisible", &Widget::isVisible)
    .def("setWidth", &Widget::setWidth)
    .def("setHeight", &Widget::setHeight)
    .def("setGeometry", &Widget::setGeometry)
    .def("setVisible", &Widget::setVisible)
    .def("addWidget", &Widget::addWidget)
    .def("removeWidget", &Widget::removeWidget)
    .def("childWidgets", &Widget::childWidgets)
    .def("addListener", &Widget::addListener)
    .def("removeListener", &Widget::removeListener)
    .def("action", &Widget::action);
    RANGERS_PYTHON_WRAP_OBJECT_DEF(Widget, WidgetWrap, c);
}
}
}
