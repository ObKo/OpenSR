/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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
#include <OpenSR/python/WidgetWrap.h>

#include <OpenSR/World/InteractionWidget.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct InteractionWidgetWrap : InteractionWidget, boost::python::wrapper<InteractionWidget>
{
    InteractionWidgetWrap(): InteractionWidget()
    {
    }

    InteractionWidgetWrap(const Rect& textRect, const Rect& selectionRect): InteractionWidget(textRect, selectionRect)
    {
    }

    static boost::python::list selections(const InteractionWidget& self)
    {
        boost::python::list result;
        std::vector<std::tuple<std::wstring, bool> > selections = self.selections();
        for (const std::tuple<std::wstring, bool>& t : selections)
        {
            result.append(boost::python::tuple(t));
        }
        return result;
    }

    static void setSelections(InteractionWidget& self, const boost::python::object& value)
    {
        boost::python::list l = boost::python::extract<boost::python::list>(value);
        std::vector<std::tuple<std::wstring, bool> > result;

        boost::python::stl_input_iterator<boost::python::object> begin(l), end;
        for (boost::python::stl_input_iterator<boost::python::object> i  = begin; i != end; ++i)
        {
            boost::python::tuple t = boost::python::extract<boost::python::tuple>(*i);
            result.push_back(std::tuple<std::wstring, bool>(boost::python::extract<std::wstring>(t[0]),
                             boost::python::extract<bool>(t[1])));
        }
        self.setSelections(result);
    }

    RANGERS_PYTHON_WRAP_WIDGET(InteractionWidget)
};

void exportInteractionWidget()
{
    using namespace boost::python;

    class_<InteractionWidgetWrap, bases<Widget>, boost::shared_ptr<InteractionWidgetWrap>, boost::noncopyable> c("InteractionWidget", init<const Rect&, const Rect&>());
    c.def(init<>())
    .add_property("text", &InteractionWidget::text, &InteractionWidget::setText)
    .add_property("selections", &InteractionWidgetWrap::selections, &InteractionWidgetWrap::setSelections);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(InteractionWidget, InteractionWidgetWrap, c);
    register_ptr_to_python<boost::shared_ptr<InteractionWidget> >();
}
}
}
}