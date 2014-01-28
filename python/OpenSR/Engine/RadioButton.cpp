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
#include <OpenSR/RadioButton.h>
#include <OpenSR/Styles.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct RadioButtonWrap : RadioButton, boost::python::wrapper<RadioButton>
{
    RadioButtonWrap(boost::shared_ptr<RadioButtonStyle> style, const std::wstring &text)
        : RadioButton(style, text)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(RadioButton)
};

void exportRadioButton()
{
    using namespace boost::python;

    class_<RadioButtonWrap, bases<Button>, boost::shared_ptr<RadioButtonWrap>, boost::noncopyable> c("RadioButton", init<boost::shared_ptr<RadioButtonStyle>, const std::wstring&>());
    c.add_property("selected", &RadioButton::isSelected);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(RadioButton, RadioButtonWrap, c);
    register_ptr_to_python<boost::shared_ptr<RadioButton> >();
}
}
}