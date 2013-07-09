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
#include "SystemPlanetWidget.h"

#include "Planet.h"

#include <OpenSR/python/WidgetWrap.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct SystemPlanetWidgetWrap : SystemPlanetWidget, boost::python::wrapper<SystemPlanetWidget>
{
    SystemPlanetWidgetWrap(boost::shared_ptr<Planet> planet = boost::shared_ptr<Planet>())
        : SystemPlanetWidget(planet)
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(SystemPlanetWidget)
};

void exportSystemPlanetWidget()
{
    using namespace boost::python;

    class_<SystemPlanetWidgetWrap, bases<Widget>, boost::shared_ptr<SystemPlanetWidgetWrap>, boost::noncopyable> c("SystemPlanetWidget", init<boost::shared_ptr<Planet> >());
    c.def(init<>())
    .def("planet", &SystemPlanetWidget::planet);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(SystemPlanetWidget, SystemPlanetWidgetWrap, c);
    register_ptr_to_python<boost::shared_ptr<SystemPlanetWidget> >();
}
}
}
}