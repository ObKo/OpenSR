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
#include <OpenSR/LineEdit.h>
#include <OpenSR/Styles.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct LineEditWrap : LineEdit, boost::python::wrapper<LineEdit>
{
    LineEditWrap(float w, float h = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>(), Widget* parent = 0)
        : LineEdit(w, h, font, parent)
    {
    }

    LineEditWrap(Widget* parent = 0)
        : LineEdit(parent)
    {
    }

    LineEditWrap(const LineEditStyle& style, Widget* parent = 0)
        : LineEdit(style, parent)
    {
    }

    RANGERS_PYTHON_WRAP_WIDGET(LineEdit)
};

void exportLineEdit()
{
    using namespace boost::python;

    class_<LineEditWrap, bases<Widget>, boost::noncopyable> c("LineEdit", init<float, float, boost::shared_ptr<Font>, Widget*>());
    c.def(init<float, float, boost::shared_ptr<Font> >())
    .def(init<float, float>())
    .def(init<float>())
    .def(init<const LineEditStyle&, Widget*>())
    .def(init<const LineEditStyle&>())
    .def(init<Widget*>())
    .def(init<>())
    .def("setText", &LineEdit::setText)
    .def("text", &LineEdit::text);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(LineEdit, LineEditWrap, c);
}
}
}