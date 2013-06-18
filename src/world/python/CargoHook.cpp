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
#include "CargoHook.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct CargoHookWrap: CargoHook, boost::python::wrapper<CargoHook>
{
    CargoHookWrap(uint64_t id = 0): CargoHook(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(CargoHook)
};

void exportCargoHook()
{
    using namespace boost::python;

    class_<CargoHookWrap, bases<Equipment>, boost::noncopyable> c("CargoHook", init<uint64_t>());
    c.def(init<>());
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(CargoHook, CargoHookWrap, c);
}
}
}
}