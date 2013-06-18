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
#include "Equipment.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct EquipmentWrap: Equipment, boost::python::wrapper<Equipment>
{
    EquipmentWrap(uint64_t id = 0): Equipment(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(Equipment)
};

void exportEquipment()
{
    using namespace boost::python;

    class_<EquipmentWrap, bases<Item>, boost::noncopyable> c("Equipment", init<uint64_t>());
    c.def(init<>())
    .def("race", &Equipment::race)
    .def("setRace", &Equipment::setRace);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Equipment, EquipmentWrap, c);
}
}
}
}