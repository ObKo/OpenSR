/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

    class_<ButtonStyle, boost::shared_ptr<ButtonStyle>, bases<ResourceObject> >("ButtonStyle")
    .def_readwrite("normal", &ButtonStyle::normal)
    .def_readwrite("hovered", &ButtonStyle::hovered)
    .def_readwrite("pressed", &ButtonStyle::pressed)
    .def_readwrite("color", &ButtonStyle::color)
    .def_readwrite("font", &ButtonStyle::font)
    .def_readwrite("contentRect", &ButtonStyle::contentRect)
    .def_readwrite("clickSound", &ButtonStyle::clickSound)
    .def_readwrite("enterSound", &ButtonStyle::enterSound)
    .def_readwrite("leaveSound", &ButtonStyle::leaveSound);

    class_<CheckBoxStyle, boost::shared_ptr<CheckBoxStyle>, bases<ResourceObject> >("CheckBoxStyle")
    .def_readwrite("normal", &CheckBoxStyle::normal)
    .def_readwrite("hovered", &CheckBoxStyle::hovered)
    .def_readwrite("checkedNormal", &CheckBoxStyle::checkedNormal)
    .def_readwrite("checkedHovered", &CheckBoxStyle::checkedHovered)
    .def_readwrite("color", &CheckBoxStyle::color)
    .def_readwrite("font", &CheckBoxStyle::font);

    class_<RadioButtonStyle, boost::shared_ptr<RadioButtonStyle>, bases<ResourceObject> >("RadioButtonStyle")
    .def_readwrite("normal", &RadioButtonStyle::normal)
    .def_readwrite("hovered", &RadioButtonStyle::hovered)
    .def_readwrite("checkedNormal", &RadioButtonStyle::selectedNormal)
    .def_readwrite("checkedHovered", &RadioButtonStyle::selectedHovered)
    .def_readwrite("color", &RadioButtonStyle::color)
    .def_readwrite("font", &RadioButtonStyle::font);

    class_<LineEditStyle, boost::shared_ptr<LineEditStyle>, bases<ResourceObject> >("LineEditStyle")
    .def_readwrite("background", &LineEditStyle::background)
    .def_readwrite("color", &LineEditStyle::color)
    .def_readwrite("font", &LineEditStyle::font)
    .def_readwrite("contentRect", &LineEditStyle::contentRect);

    class_<ScrollBarStyle, boost::shared_ptr<ScrollBarStyle>, bases<ResourceObject> >("ScrollBarStyle")
    .def_readwrite("upButton", &ScrollBarStyle::upButton)
    .def_readwrite("scroll", &ScrollBarStyle::scroll)
    .def_readwrite("downButton", &ScrollBarStyle::downButton);

    class_<Skin, boost::shared_ptr<Skin>, bases<ResourceObject> >("Skin")
    .def_readwrite("buttonStyle", &Skin::buttonStyle)
    .def_readwrite("scrollStyle", &Skin::scrollStyle)
    .def_readwrite("lineEditStyle", &Skin::lineEditStyle)
    .def_readwrite("checkBoxStyle", &Skin::checkBoxStyle)
    .def_readwrite("radioButtonStyle", &Skin::radioButtonStyle);

    class_<SoundDescriptor, boost::shared_ptr<SoundDescriptor>, bases<ResourceObject> >("SoundDescriptor")
    .def_readwrite("path", &SoundDescriptor::path);
}
}
}
