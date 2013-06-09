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
#include <OpenSR/Label.h>
#include <OpenSR/Font.h>

#include "OpenSR/python/SpriteWrap.h"

namespace Rangers
{
namespace Python
{
struct LabelWrap : Label, boost::python::wrapper<Label>
{
    LabelWrap(Object *parent = 0)
        : Label(parent)
    {
    }
    LabelWrap(const std::wstring& text, Object *parent = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>(), SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP)
        : Label(text, parent, font, xpos, ypos)
    {
    }
    RANGERS_PYTHON_WRAP_SPRITE(Label)
};

void exportLabel()
{
    using namespace boost::python;

    class_<LabelWrap, bases<Sprite>, boost::noncopyable> c("Label", init<const std::wstring&, Object*>());
    c.def(init<const std::wstring&>())
    .def(init<Object*>())
    .def(init<>())
    .def("setText", (void (Label::*)(const std::wstring&))&Label::setText)
    .def("setFont", &Label::setFont)
    .def("font", &Label::font)
    .def("text", &Label::text)
    .def("setWordWrap", &Label::setWordWrap)
    .def("setFixedSize", &Label::setFixedSize);
    RANGERS_PYTHON_WRAP_SPRITE_DEF(Label, LabelWrap, c);
}
}
}