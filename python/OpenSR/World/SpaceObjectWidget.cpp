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
#include <OpenSR/python/WidgetWrap.h>

#include <OpenSR/World/SpaceObjectWidget.h>
#include <OpenSR/World/SpaceObject.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct SpaceObjectWidgetWrap : SpaceObjectWidget, boost::python::wrapper<SpaceObjectWidget>
{
    SpaceObjectWidgetWrap(boost::shared_ptr<SpaceObject> object = boost::shared_ptr<SpaceObject>())
        : SpaceObjectWidget(object)
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(SpaceObjectWidget)
};

void exportSpaceObjectWidget()
{
    using namespace boost::python;

    class_<SpaceObjectWidgetWrap, bases<Widget>, boost::shared_ptr<SpaceObjectWidgetWrap>, boost::noncopyable> c("SpaceObjectWidget", init<boost::shared_ptr<SpaceObject> >());
    c.def(init<>())
    .def("object", &SpaceObjectWidget::object)
    .def("setObject", &SpaceObjectWidget::setObject);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(SpaceObjectWidget, SpaceObjectWidgetWrap, c);
    register_ptr_to_python<boost::shared_ptr<SpaceObjectWidget> >();
}
}
}
}