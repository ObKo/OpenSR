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
#include "Wrappers.h"

namespace Rangers
{
namespace Python
{
/* ScrollArea(Widget* parent = 0);
  ScrollArea(const ScrollBarStyle& style, WidgetNode *node = 0, Widget *parent = 0); */
struct ScrollAreaWrap : WidgetWrap_<ScrollArea>
{
    ScrollAreaWrap(const ScrollBarStyle& style, boost::shared_ptr<WidgetNode> node = boost::shared_ptr<WidgetNode>(), Widget *parent = 0)
        : WidgetWrap_<ScrollArea>(style, node, parent)
    {
    }

    ScrollAreaWrap(Widget *parent = 0)
        : WidgetWrap_<ScrollArea>(parent)
    {
    }
};

void exportScrollArea()
{
    using namespace boost::python;

    class_<ScrollArea, bases<Widget>, boost::noncopyable> c("ScrollArea", init<const ScrollBarStyle&, boost::shared_ptr<WidgetNode>, Widget*>());
    c.def(init<const ScrollBarStyle&, boost::shared_ptr<WidgetNode> >())
    .def(init<const ScrollBarStyle&>())
    .def(init<Widget*>())
    .def(init<>())
    .def("setWidth", &ScrollArea::setWidth)
    .def("setHeight", &ScrollArea::setHeight)
    .def("setNode", &ScrollArea::setNode);
    ScrollAreaWrap::defWrapped(c);
}
}
}