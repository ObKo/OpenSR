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
#include <OpenSR/RadioButtonGroup.h>
#include <OpenSR/RadioButton.h>
#include <OpenSR/Styles.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct RadioButtonGroupWrap : RadioButtonGroup, boost::python::wrapper<RadioButtonGroup>
{
    RadioButtonGroupWrap(Widget *parent = 0)
        : RadioButtonGroup(parent)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(RadioButtonGroup)
};

void exportRadioButtonGroup()
{
    using namespace boost::python;

    class_<RadioButtonGroupWrap, bases<WidgetNode>, boost::shared_ptr<RadioButtonGroupWrap>, boost::noncopyable> c("RadioButtonGroup", init<Widget*>());
    c.def(init<>());
    RANGERS_PYTHON_WRAP_WIDGET_DEF(RadioButtonGroup, RadioButtonGroupWrap, c);
    c.def("addRadioButton", &RadioButtonGroup::addRadioButton)
    .def("removeRadioButton", &RadioButtonGroup::removeRadioButton)
    .def("selectRadioButton", &RadioButtonGroup::selectRadioButton);
    register_ptr_to_python<boost::shared_ptr<RadioButtonGroup> >();
}
}
}