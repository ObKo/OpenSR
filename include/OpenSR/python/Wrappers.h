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

#ifndef RANGERS_PYTHON_WRAPPERS_H
#define RANGERS_PYTHON_WRAPPERS_H

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <list>
#include <vector>

#define RANGERS_PYTHON_WRAP_FUNC(class, func, ...) { \
        Rangers::Python::GILGuard g; \
        if (boost::python::override f = this->get_override(#func)) \
        { \
            try \
            { \
                return f(__VA_ARGS__); \
            } \
            catch (const boost::python::error_already_set& e) \
            { \
                handlePythonError(); \
            } \
        } \
        return class::func(__VA_ARGS__); \
    }

#define RANGERS_PYTHON_WRAP_FUNC_VOID(class, func, ...) { \
        Rangers::Python::GILGuard g; \
        if (boost::python::override f = this->get_override(#func)) \
        { \
            try \
            { \
                f(__VA_ARGS__); \
                return; \
            } \
            catch (const boost::python::error_already_set& e) \
            { \
                handlePythonError(); \
            } \
        } \
        class::func(__VA_ARGS__); \
    }

namespace Rangers
{
void handlePythonError();
namespace Python
{
template<class T>
boost::python::list vectorToPythonList(const std::vector<T>& v)
{
    boost::python::list l;
    for (const T & item : v)
    {
        l.append(item);
    }
    return l;
}

template<class T>
boost::python::list listToPythonList(const std::list<T>& v)
{
    boost::python::list l;
    for (const T & item : v)
    {
        l.append(item);
    }
    return l;
}

//TODO: Optimize?
template<class T>
std::vector<T> pythonObjectToVector(const boost::python::object &l)
{
    boost::python::stl_input_iterator<T> begin(l), end;
    return std::vector<T>(begin, end);
}

//TODO: Optimize?
template<class T>
std::list<T> pythonObjectToList(const boost::python::object &l)
{
    boost::python::stl_input_iterator<T> begin(l), end;
    return std::list<T>(begin, end);
}

class GILGuard
{
public:
    inline GILGuard()
    {
        m_state = PyGILState_Ensure();
    }
    inline ~GILGuard()
    {
        PyGILState_Release(m_state);
    }
private:
    PyGILState_STATE m_state;
};

}
}

#endif