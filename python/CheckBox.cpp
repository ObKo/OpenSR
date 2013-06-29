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
#include <OpenSR/CheckBox.h>
#include <OpenSR/Styles.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct CheckBoxWrap : CheckBox, boost::python::wrapper<CheckBox>
{
    CheckBoxWrap(const CheckBoxStyle& style, const std::wstring &text, Widget *parent = 0)
        : CheckBox(style, text, parent)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(CheckBox)
};

void exportCheckBox()
{
    using namespace boost::python;

    class_<CheckBoxWrap, bases<Widget>, boost::shared_ptr<CheckBoxWrap>, boost::noncopyable> c("CheckBox", init<const CheckBoxStyle&, const std::wstring&, Widget*>());
    c.def(init<const CheckBoxStyle&, const std::wstring&>())
    .def("setText", &CheckBox::setText)
    .def("text", &CheckBox::text)
    .def("setChecked", &CheckBox::setChecked)
    .def("checked", &CheckBox::checked);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(CheckBox, CheckBoxWrap, c);
    register_ptr_to_python<boost::shared_ptr<CheckBox> >();
}
}
}