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
#include "DefenceGenerator.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct DefenceGeneratorWrap: DefenceGenerator, boost::python::wrapper<DefenceGenerator>
{
    DefenceGeneratorWrap(uint64_t id = 0): DefenceGenerator(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(DefenceGenerator)
};

void exportDefenceGenerator()
{
    using namespace boost::python;

    class_<DefenceGeneratorWrap, bases<Equipment>, boost::shared_ptr<DefenceGeneratorWrap>, boost::noncopyable> c("DefenceGenerator", init<uint64_t>());
    c.def(init<>());
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(DefenceGenerator, DefenceGeneratorWrap, c);
    register_ptr_to_python<boost::shared_ptr<DefenceGenerator> >();
}
}
}
}