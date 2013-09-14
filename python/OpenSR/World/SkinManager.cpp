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

#include <OpenSR/World/SkinManager.h>

namespace Rangers
{
namespace World
{
namespace Python
{
void exportSkinManager()
{
    using namespace boost::python;

    class_<SkinManager, boost::noncopyable> c("SkinManager", boost::python::no_init);
    c.def("loadStyles", &SkinManager::loadStyles)
    .def("infoWidgetStyle", &SkinManager::infoWidgetStyle);

    class_<InfoWidgetStyle> c2("InfoWidgetStyle", init<>());
    c2.def_readwrite("background", &InfoWidgetStyle::background)
    .def_readwrite("font", &InfoWidgetStyle::font)
    .def_readwrite("captionFont", &InfoWidgetStyle::captionFont)
    .def_readwrite("color", &InfoWidgetStyle::color)
    .def_readwrite("captionColor", &InfoWidgetStyle::captionColor)
    .def_readwrite("labelColor", &InfoWidgetStyle::labelColor)
    .def_readwrite("contentRect", &InfoWidgetStyle::contentRect);
}
}
}
}