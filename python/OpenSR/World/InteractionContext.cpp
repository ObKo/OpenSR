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
#include <OpenSR/World/python/WorldObjectWrap.h>

#include <OpenSR/World/InteractionContext.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct InteractionContextWrap: InteractionContext, boost::python::wrapper<InteractionContext>
{
    InteractionContextWrap(uint64_t id = 0): InteractionContext(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(InteractionContext)
};

void exportInteractionContext()
{
    using namespace boost::python;

    class_<InteractionContextWrap, bases<WorldObject>, boost::shared_ptr<InteractionContextWrap>, boost::noncopyable> c("InteractionContext", init<>());
    c.add_property("relation", &InteractionContext::relation, &InteractionContext::setRelation)
    .add_property("race", &InteractionContext::race, &InteractionContext::setRace);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(InteractionContext, InteractionContextWrap, c);
    register_ptr_to_python<boost::shared_ptr<InteractionContext> >();
}
}
}
}