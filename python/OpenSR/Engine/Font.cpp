/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include <OpenSR/Font.h>
#include <OpenSR/Texture.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportFont()
{
    using namespace boost::python;
    class_<Font, boost::shared_ptr<Font> >("Font", boost::python::no_init)
    .def("renderText", (boost::shared_ptr<Texture> (Font::*)(const std::string&, int) const)&Font::renderText)
    .def("renderColoredText", (boost::shared_ptr<Texture> (Font::*)(const std::string&, int, int, int) const)&Font::renderColoredText)
    .add_property("size", &Font::size)
    .def("calculateStringWidth", (int (Font::*)(const std::string&) const)&Font::calculateStringWidth)
    .def("maxChars", (int (Font::*)(const std::string&, int) const)&Font::maxChars);
}
}
}
