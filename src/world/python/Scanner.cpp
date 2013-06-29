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
#include "Scanner.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct ScannerWrap: Scanner, boost::python::wrapper<Scanner>
{
    ScannerWrap(uint64_t id = 0): Scanner(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(Scanner)
};

void exportScanner()
{
    using namespace boost::python;

    class_<ScannerWrap, bases<Equipment>, boost::shared_ptr<ScannerWrap>, boost::noncopyable> c("Scanner", init<uint64_t>());
    c.def(init<>());
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Scanner, ScannerWrap, c);
    register_ptr_to_python<boost::shared_ptr<Scanner> >();
}
}
}
}