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
#include <OpenSR/TiledPolyline.h>
#include <OpenSR/Texture.h>

#include "OpenSR/python/ObjectWrap.h"

namespace Rangers
{
namespace Python
{
struct TiledPolylineWrap : TiledPolyline, boost::python::wrapper<TiledPolyline>
{
    TiledPolylineWrap(): TiledPolyline()
    {
    }

    TiledPolylineWrap(boost::shared_ptr<Texture> texture)
        : TiledPolyline(texture)
    {
    }


    TiledPolylineWrap(const std::wstring& texture)
        : TiledPolyline(texture)
    {
    }

    RANGERS_PYTHON_WRAP_OBJECT(TiledPolyline)
};

void exportTiledPolyline()
{
    using namespace boost::python;

    class_<TiledPolylineWrap, bases<Object>, boost::shared_ptr<TiledPolylineWrap>, boost::noncopyable> c("TiledPolyline", init<boost::shared_ptr<Texture> >());
    c.def(init<const std::wstring&>())
    .def(init<>())
    .def("texture", &TiledPolyline::texture)
    .def("setTexture", (void (TiledPolyline::*)(boost::shared_ptr<Texture>))&TiledPolyline::setTexture)
    .def("setTexture", (void (TiledPolyline::*)(const std::wstring&))&TiledPolyline::setTexture)
    .def("polyline", &TiledPolyline::polyline)
    .def("setPolyline", &TiledPolyline::setPolyline);
    RANGERS_PYTHON_WRAP_OBJECT_DEF(TiledPolyline, TiledPolylineWrap, c);
    register_ptr_to_python<boost::shared_ptr<TiledPolyline> >();
}
}
}
