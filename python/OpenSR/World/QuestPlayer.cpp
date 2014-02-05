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

#include <boost/python.hpp>
#include <OpenSR/python/Wrappers.h>

#include <OpenSR/World/QuestPlayer.h>

namespace Rangers
{
namespace World
{
namespace Python
{
namespace
{
boost::python::list visibleParameters(const QuestPlayer& self)
{
    std::vector<std::wstring> v = self.visibleParameters();
    return Rangers::Python::vectorToPythonList(v);
}

void setImages(QuestPlayer& self, const boost::python::dict &locImages, const boost::python::dict &transImages)
{
    self.setImages(Rangers::Python::pythonDictToMap<uint32_t, uint32_t>(locImages), Rangers::Python::pythonDictToMap<uint32_t, uint32_t>(transImages));
}

boost::python::list visibleTransitions(const QuestPlayer& self)
{
    std::vector<std::tuple<uint32_t, std::wstring, bool> > v = self.visibleTransitions();
    boost::python::list l;
    for (const std::tuple<uint32_t, std::wstring, bool>& t : v)
        l.append(boost::python::make_tuple(std::get<0>(t), std::get<1>(t), std::get<2>(t)));
    return l;
}
}
void exportQuestPlayer()
{
    using namespace boost::python;

    class_<QuestPlayer, boost::shared_ptr<QuestPlayer> > c("QuestPlayer", init<>());
    c.add_property("currentText", &QuestPlayer::currentText)
    .add_property("questCompleted", &QuestPlayer::questCompleted)
    .add_property("questFailed", &QuestPlayer::questFailed)
    .add_property("death", &QuestPlayer::death)
    .add_property("visibleParameters", &visibleParameters)
    .add_property("visibleTransitions", &visibleTransitions)
    .add_property("currentLocation", &QuestPlayer::currentLocation)
    .add_property("currentTransition", &QuestPlayer::currentTransition)
    .add_property("isTransition", &QuestPlayer::isTransition)
    .add_property("currentImage", &QuestPlayer::currentImage)
    .def("setImages", &setImages)
    .def("transit", &QuestPlayer::transit)
    .def("loadQuest", &QuestPlayer::loadQuest)
    .def("resetQuest", &QuestPlayer::resetQuest);
}
}
}
}