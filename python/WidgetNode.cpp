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

#include "WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct WidgetNodeWrap : WidgetNode, boost::python::wrapper<WidgetNode>
{
    WidgetNodeWrap(Widget *parent = 0)
        : WidgetNode(parent)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(WidgetNode)
};

void exportWidgetNode()
{
    using namespace boost::python;

    class_<WidgetNodeWrap, bases<Widget>, boost::noncopyable> c("WidgetNode", init<Widget*>());
    c.def(init<>());
    RANGERS_PYTHON_WRAP_WIDGET_DEF(WidgetNode, WidgetNodeWrap, c);
}
}
}