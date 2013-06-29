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
#include <OpenSR/NinePatch.h>

#include "OpenSR/python/SpriteWrap.h"

namespace Rangers
{
namespace Python
{
struct NinePatchWrap : NinePatch, boost::python::wrapper<NinePatch>
{
    NinePatchWrap(Object *parent = 0)
        : NinePatch(parent)
    {
    }
    NinePatchWrap(const NinePatchDescriptor &desc, Object *parent = 0)
        : NinePatch(desc, parent)
    {
    }
    NinePatchWrap(const std::wstring& name, Object *parent = 0)
        : NinePatch(name, parent)
    {
    }
    RANGERS_PYTHON_WRAP_SPRITE(NinePatch)
};

void exportNinePatch()
{
    using namespace boost::python;

    class_<NinePatchWrap, bases<Sprite>, boost::shared_ptr<NinePatchWrap>, boost::noncopyable> c("NinePatch", init<const NinePatchDescriptor&, Object*>());
    c.def(init<const NinePatchDescriptor&>())
    .def(init<const std::wstring&, Object*>())
    .def(init<const std::wstring&>())
    .def(init<Object*>())
    .def(init<>());
    RANGERS_PYTHON_WRAP_SPRITE_DEF(NinePatch, NinePatchWrap, c);
    register_ptr_to_python<boost::shared_ptr<NinePatch> >();
}
}
}