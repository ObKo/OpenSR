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

#include <OpenSR/Node.h>
#include <boost/python.hpp>
#include "Wrappers.h"

namespace Rangers
{
namespace Python
{
struct NodeWrap: ObjectWrap_<Node>
{
    NodeWrap(Object *parent = 0): ObjectWrap_<Node>(parent)
    {
    }
};

void exportNode()
{
    using namespace boost::python;
    class_<NodeWrap, bases<Object>, boost::noncopyable> c("Node", init<Object*>());
    c
    .def(init<>());
    NodeWrap::defWrapped(c);
    //.def("processLogic", &Node::processLogic, &Python::NodeWrap::processLogic_)
    //.def("draw", &Node::draw, &Python::NodeWrap::draw_);
}
}
}
