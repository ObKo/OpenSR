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

    static void setTexture(TiledPolyline& self, boost::python::object g)
    {
        boost::python::extract<boost::shared_ptr<Texture> > t(g);
        boost::python::extract<std::wstring> s(g);
        if (t.check())
        {
            self.setTexture(t);
        }
        else if (s.check())
        {
            self.setTexture(s);
        }
        else
        {
            //FIXME:
            self.setTexture(boost::python::extract<boost::shared_ptr<Texture> >(g));
        }
    }

    static boost::python::object polyline(TiledPolyline& self)
    {
        return boost::python::list(self.polyline());
    }

    static void setPolyline(TiledPolyline& self, boost::python::object p)
    {
        boost::python::list l = boost::python::extract<boost::python::list>(p);
        int len = boost::python::len(l);
        std::vector<Vector> points;
        for (int i = 0; i < len; i++)
        {
            boost::python::extract<boost::python::tuple> t(l[i]);
            if (t.check())
            {
                boost::python::tuple p = t;
                points.push_back(Vector(boost::python::extract<float>(p[0]), boost::python::extract<float>(p[1])));
            }
            else
            {
                points.push_back(boost::python::extract<Vector>(l[i]));
            }
        }
        self.setPolyline(points);
    }
};

void exportTiledPolyline()
{
    using namespace boost::python;

    class_<TiledPolylineWrap, bases<Object>, boost::shared_ptr<TiledPolylineWrap>, boost::noncopyable> c("TiledPolyline", init<boost::shared_ptr<Texture> >());
    c.def(init<const std::wstring&>())
    .def(init<>())
    .add_property("texture", &TiledPolyline::texture, &TiledPolylineWrap::setTexture)
    .add_property("polyline", &TiledPolylineWrap::polyline, &TiledPolylineWrap::setPolyline);
    RANGERS_PYTHON_WRAP_OBJECT_DEF(TiledPolyline, TiledPolylineWrap, c);
    register_ptr_to_python<boost::shared_ptr<TiledPolyline> >();
}
}
}
