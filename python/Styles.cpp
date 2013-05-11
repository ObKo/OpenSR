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

#include <OpenSR/Styles.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportStyles()
{
    using namespace boost::python;
    enum_<ResourceDescriptor::Type>("ResourceDescriptorType")
    .value("NONE",  ResourceDescriptor::NONE)
    .value("NINEPATCH", ResourceDescriptor::NINEPATCH)
    .value("SPRITE", ResourceDescriptor::SPRITE)
    .value("FONT", ResourceDescriptor::FONT)
    .value("SOUND", ResourceDescriptor::SOUND);

    class_<ResourceDescriptor>("ResourceDescriptor", init<>())
    .def(init<const NinePatchDescriptor&>())
    .def(init<const TextureRegionDescriptor&>())
    .def(init<const FontDescriptor&>())
    .def(init<const std::wstring&>())
    .def_readwrite("type", &ResourceDescriptor::type)
    .def("texture", &ResourceDescriptor::texture)
    .def("ninePatch", &ResourceDescriptor::ninePatch)
    .def("sound", &ResourceDescriptor::sound)
    .def("font", &ResourceDescriptor::font);

    class_<ButtonStyle>("ButtonStyle")
    .def_readwrite("normal", &ButtonStyle::normal)
    .def_readwrite("hovered", &ButtonStyle::hovered)
    .def_readwrite("pressed", &ButtonStyle::pressed)
    .def_readwrite("color", &ButtonStyle::color)
    .def_readwrite("font", &ButtonStyle::font)
    .def_readwrite("contentRect", &ButtonStyle::contentRect)
    .def_readwrite("clickSound", &ButtonStyle::clickSound)
    .def_readwrite("enterSound", &ButtonStyle::enterSound)
    .def_readwrite("leaveSound", &ButtonStyle::leaveSound);

    class_<CheckBoxStyle>("CheckBoxStyle")
    .def_readwrite("normal", &CheckBoxStyle::normal)
    .def_readwrite("hovered", &CheckBoxStyle::hovered)
    .def_readwrite("checkedNormal", &CheckBoxStyle::checkedNormal)
    .def_readwrite("checkedHovered", &CheckBoxStyle::checkedHovered)
    .def_readwrite("color", &CheckBoxStyle::color)
    .def_readwrite("font", &CheckBoxStyle::font);

    class_<RadioButtonStyle>("RadioButtonStyle")
    .def_readwrite("normal", &RadioButtonStyle::normal)
    .def_readwrite("hovered", &RadioButtonStyle::hovered)
    .def_readwrite("checkedNormal", &RadioButtonStyle::checkedNormal)
    .def_readwrite("checkedHovered", &RadioButtonStyle::checkedHovered)
    .def_readwrite("color", &RadioButtonStyle::color)
    .def_readwrite("font", &RadioButtonStyle::font);

    class_<LineEditStyle>("LineEditStyle")
    .def_readwrite("background", &LineEditStyle::background)
    .def_readwrite("color", &LineEditStyle::color)
    .def_readwrite("font", &LineEditStyle::font)
    .def_readwrite("contentRect", &LineEditStyle::contentRect);

    class_<ScrollBarStyle>("ScrollBarStyle")
    .def_readwrite("upButton", &ScrollBarStyle::upButton)
    .def_readwrite("scroll", &ScrollBarStyle::scroll)
    .def_readwrite("downButton", &ScrollBarStyle::downButton);

    class_<Skin>("Skin")
    .def_readwrite("buttonStyle", &Skin::buttonStyle)
    .def_readwrite("scrollStyle", &Skin::scrollStyle)
    .def_readwrite("lineEditStyle", &Skin::lineEditStyle)
    .def_readwrite("checkBoxStyle", &Skin::checkBoxStyle);
}
}
}
