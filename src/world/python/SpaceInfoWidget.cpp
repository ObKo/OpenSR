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
#include "SpaceInfoWidget.h"

#include "WorldStyleManager.h"

#include <OpenSR/python/WidgetWrap.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct SpaceInfoWidgetWrap : SpaceInfoWidget, boost::python::wrapper<SpaceInfoWidget>
{
    SpaceInfoWidgetWrap(const InfoWidgetStyle &style, Widget *parent = 0)
        : SpaceInfoWidget(style, parent)
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(SpaceInfoWidget)
};

void exportSpaceInfoWidget()
{
    using namespace boost::python;

    class_<SpaceInfoWidgetWrap, bases<Widget>, boost::noncopyable> c("SpaceInfoWidget", init<const InfoWidgetStyle&, Widget*>());
    c.def(init<const InfoWidgetStyle&>())
    .def("clear", &SpaceInfoWidget::clear)
    .def("showPlanet", &SpaceInfoWidget::showPlanet)
    .def("showSystem", &SpaceInfoWidget::showSystem);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(SpaceInfoWidget, SpaceInfoWidgetWrap, c);
}
}
}
}