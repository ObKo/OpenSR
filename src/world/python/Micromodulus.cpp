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
#include "Micromodulus.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct MicromodulusWrap: Micromodulus, boost::python::wrapper<Micromodulus>
{
    MicromodulusWrap(uint64_t id = 0): Micromodulus(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(Micromodulus)
};

void exportMicromodulus()
{
    using namespace boost::python;

    class_<MicromodulusWrap, bases<Item>, boost::shared_ptr<MicromodulusWrap>, boost::noncopyable> c("Micromodulus", init<uint64_t>());
    c.def(init<>());
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Micromodulus, MicromodulusWrap, c);
    register_ptr_to_python<boost::shared_ptr<Micromodulus> >();
}
}
}
}