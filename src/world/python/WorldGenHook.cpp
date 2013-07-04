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
#include <OpenSR/python/Wrappers.h>

#include "WorldGenHook.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct WorldGenHookWrap: WorldGenHook, boost::python::wrapper<WorldGenHook>
{
    WorldGenHookWrap(): WorldGenHook()
    {
    }

    virtual void generate()
    {
        RANGERS_PYTHON_WRAP_FUNC_VOID(WorldGenHook, generate);
    }
    void generate_()
    {
        WorldGenHook::generate();
    }
};

void exportWorldGenHook()
{
    using namespace boost::python;

    class_<WorldGenHookWrap, boost::shared_ptr<WorldGenHookWrap>, boost::noncopyable> c("WorldGenHook", init<>());
    c.def("generate", &WorldGenHook::generate, &WorldGenHookWrap::generate_);
    register_ptr_to_python<boost::shared_ptr<WorldGenHook> >();
}
}
}
}