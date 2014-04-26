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
#include <OpenSR/TiledLine.h>
#include <OpenSR/Texture.h>

#include "OpenSR/python/ObjectWrap.h"

namespace Rangers
{
namespace Python
{
struct TiledLineWrap : TiledLine, boost::python::wrapper<TiledLine>
{
    TiledLineWrap(): TiledLine()
    {
    }

    TiledLineWrap(boost::shared_ptr<Texture> texture)
        : TiledLine(texture)
    {
    }
    TiledLineWrap(boost::shared_ptr<Texture> texture, const Vector& start, const Vector& end)
        : TiledLine(texture, start, end)
    {
    }

    TiledLineWrap(const std::string& texture)
        : TiledLine(texture)
    {
    }
    TiledLineWrap(const std::string& texture, const Vector& start, const Vector& end)
        : TiledLine(texture, start, end)
    {
    }

    RANGERS_PYTHON_WRAP_OBJECT(TiledLine)

    static void setLine(TiledLine& self, boost::python::object pos)
    {
        boost::python::tuple mt = boost::python::extract<boost::python::tuple>(pos);
        boost::python::extract<boost::python::tuple> t(mt[0]);
        boost::python::extract<Vector> v(mt[0]);
        if (v.check())
        {
            self.setLine(boost::python::extract<Vector>(mt[0]), boost::python::extract<Vector>(mt[1]));
        }
        else if (t.check())
        {
            boost::python::tuple p1 = boost::python::extract<boost::python::tuple>(mt[0]);
            boost::python::tuple p2 = boost::python::extract<boost::python::tuple>(mt[1]);
            self.setLine(Vector(boost::python::extract<float>(p1[0]), boost::python::extract<float>(p1[1])),
                         Vector(boost::python::extract<float>(p2[0]), boost::python::extract<float>(p2[1])));
        }
    }

    static void setTexture(TiledLine&  self, boost::python::object g)
    {
        boost::python::extract<boost::shared_ptr<Texture> > t(g);
        boost::python::extract<std::string> s(g);
        if (t.check())
        {
            self.setTexture(t);
        }
        else if (s.check())
        {
            self.setTexture(s);
        }
    }

    static boost::python::object line(TiledLine& self)
    {
        return boost::python::make_tuple(self.start(), self.end());
    }
};

void exportTiledLine()
{
    using namespace boost::python;

    class_<TiledLineWrap, bases<Object>, boost::shared_ptr<TiledLineWrap>, boost::noncopyable> c("TiledLine", init<boost::shared_ptr<Texture>, const Vector&, const Vector&>());
    c.def(init<boost::shared_ptr<Texture> >())
    .def(init<const std::string&, const Vector&, const Vector&>())
    .def(init<const std::string&>())
    .def(init<>())
    .add_property("texture", &TiledLine::texture, &TiledLineWrap::setTexture)
    .add_property("line", &TiledLineWrap::line, &TiledLineWrap::setLine);
    RANGERS_PYTHON_WRAP_OBJECT_DEF(TiledLine, TiledLineWrap, c);
    register_ptr_to_python<boost::shared_ptr<TiledLine> >();
}
}
}
