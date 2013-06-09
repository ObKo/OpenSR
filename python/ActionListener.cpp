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

#include <OpenSR/ActionListener.h>
#include <OpenSR/Action.h>
#include <OpenSR/Log.h>
#include <boost/python.hpp>

#include "OpenSR/python/Wrappers.h"

namespace Rangers
{
void handlePythonError();
namespace Python
{
struct ActionListenerWrap: ActionListener, boost::python::wrapper<ActionListener>
{
    ActionListenerWrap()
    {
    }

    void actionPerformed(const Action &action)
    {
        GILGuard g;
        try
        {
            //{ Log::debug() << "Precall"; }
            get_override("actionPerformed")(action);
            //{ Log::debug() << "Postcall"; }
        }
        catch (const boost::python::error_already_set& e)
        {
            handlePythonError();
        }
    }
};

void exportActionListener()
{
    using namespace boost::python;
    class_<ActionListenerWrap, boost::noncopyable>("ActionListener", init<>())
    .def("actionPerformed", pure_virtual(&ActionListener::actionPerformed));
}
}
}
