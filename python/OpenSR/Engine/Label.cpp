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
    LabelWrap()
        : Label()
    {
    }
    LabelWrap(const std::wstring& text, boost::shared_ptr<Font> font = boost::shared_ptr<Font>())
        : Label(text, font)
    {
    }
    RANGERS_PYTHON_WRAP_SPRITE(Label)
};

void exportLabel()
{
    using namespace boost::python;

    class_<LabelWrap, bases<Sprite>, boost::shared_ptr<LabelWrap>, boost::noncopyable> c("Label", init<const std::wstring&>());
    c.def(init<>())
    .add_property("text", &Label::text, (void (Label::*)(const std::wstring&))&Label::setText)
    .add_property("font", &Label::font, &Label::setFont)
    .add_property("wordWrap", &Label::wordWrap, &Label::setWordWrap)
    .def("setFixedSize", &Label::setFixedSize);
    RANGERS_PYTHON_WRAP_SPRITE_DEF(Label, LabelWrap, c);
    register_ptr_to_python<boost::shared_ptr<Label> >();
}
}
}