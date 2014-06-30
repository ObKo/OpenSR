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
    .def("infoWidgetStyle", &SkinManager::infoWidgetStyle)
    .def("trajectoryStyle", &SkinManager::trajectoryStyle);

    class_<InfoWidgetStyle, boost::shared_ptr<InfoWidgetStyle>, bases<ResourceObject> > c2("InfoWidgetStyle", init<>());
    c2.def_readwrite("background", &InfoWidgetStyle::background)
    .def_readwrite("font", &InfoWidgetStyle::font)
    .def_readwrite("captionFont", &InfoWidgetStyle::captionFont)
    .def_readwrite("color", &InfoWidgetStyle::color)
    .def_readwrite("captionColor", &InfoWidgetStyle::captionColor)
    .def_readwrite("labelColor", &InfoWidgetStyle::labelColor)
    .def_readwrite("contentRect", &InfoWidgetStyle::contentRect);

    class_<InteractionWidgetStyle, boost::shared_ptr<InteractionWidgetStyle>, bases<ResourceObject> > c3("InteractionWidgetStyle", init<>());
    c3.def_readwrite("background", &InteractionWidgetStyle::background)
    .def_readwrite("font", &InteractionWidgetStyle::font)
    .def_readwrite("color", &InteractionWidgetStyle::color)
    .def_readwrite("activeColor", &InteractionWidgetStyle::activeColor)
    .def_readwrite("inactiveColor", &InteractionWidgetStyle::inactiveColor)
    .def_readwrite("scrollStyle", &InteractionWidgetStyle::scrollStyle);

    class_<TrajectoryStyle, boost::shared_ptr<TrajectoryStyle>, bases<ResourceObject> > c4("TrajectoryStyle", init<>());
    c4.def_readwrite("currentTurnLine", &TrajectoryStyle::currentTurnLine)
    .def_readwrite("turnLine", &TrajectoryStyle::turnLine)
    .def_readwrite("currentTurnPoint", &TrajectoryStyle::currentTurnPoint)
    .def_readwrite("turnPoint", &TrajectoryStyle::turnPoint);
}
}
}
}