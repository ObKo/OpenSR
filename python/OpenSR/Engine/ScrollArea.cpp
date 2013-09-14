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
#include <OpenSR/WidgetNode.h>
#include <OpenSR/ScrollArea.h>
#include <OpenSR/Styles.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct ScrollAreaWrap : ScrollArea, boost::python::wrapper<ScrollArea>
{
    ScrollAreaWrap(const ScrollBarStyle& style, boost::shared_ptr<WidgetNode> node = boost::shared_ptr<WidgetNode>())
        : ScrollArea(style, node)
    {
    }

    ScrollAreaWrap()
        : ScrollArea()
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(ScrollArea)
};

void exportScrollArea()
{
    using namespace boost::python;

    class_<ScrollAreaWrap, bases<Widget>, boost::shared_ptr<ScrollAreaWrap>, boost::noncopyable> c("ScrollArea", init<const ScrollBarStyle&, boost::shared_ptr<WidgetNode> >());
    c.def(init<const ScrollBarStyle&>())
    .def(init<>())
    .def("setWidth", &ScrollArea::setWidth)
    .def("setHeight", &ScrollArea::setHeight)
    .def("setNode", &ScrollArea::setNode);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(ScrollArea, ScrollAreaWrap, c);
    register_ptr_to_python<boost::shared_ptr<ScrollArea> >();
}
}
}