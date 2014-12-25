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

#include "OpenSR/PythonBindings.h"

#include <Python.h>
#include <boost/python.hpp>

#include <OpenSR/libRanger.h>

#include "OpenSR/ResourceManager.h"
#include "OpenSR/Log.h"

namespace
{
boost::python::object main_module;
boost::python::object main_namespace;
}

namespace Rangers
{
void handlePythonError()
{
    PyObject *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

    if (!ptype || !pvalue)
    {
        Log::error() << "[Python] Unknown error!";
        return;
    }

    PyObject *errorTextObject = PyObject_Str(pvalue);

    boost::python::handle<> hType(ptype);
    boost::python::object extype(hType);

    std::string tracebackString;
    //long lineno = 0;
    if (ptraceback)
    {
        try
        {
            boost::python::handle<> hTraceback(ptraceback);
            boost::python::object traceback(hTraceback);
            //lineno =  boost::python::extract<long> (traceback.attr("tb_lineno"));
            boost::python::object tb(boost::python::import("traceback"));
            boost::python::object fmt_tb(tb.attr("format_tb"));
            boost::python::object tb_list(fmt_tb(hTraceback));
            boost::python::object tb_str(boost::python::str("\n").join(tb_list));
            tracebackString = boost::python::extract<std::string>(tb_str);
        }
        catch (const boost::python::error_already_set& e)
        {
        }
    }

    std::string strErrorMessage = boost::python::extract<std::string>(errorTextObject);
    std::string exceptionType = boost::python::extract<std::string>(extype.attr("__name__"));
    Log::error() << "[Python] " << exceptionType << ": " << strErrorMessage;

    if (!tracebackString.empty())
    {
        std::vector<std::string> lines = split(tracebackString, L'\n');
        std::vector<std::string>::const_iterator end = lines.end();
        for (std::vector<std::string>::const_iterator i = lines.begin(); i != end; ++i)
        {
            if (i->empty())
                continue;
            Log::error() << "[Python] " << (*i);
        }
    }
}

void execPythonScript(const char *data, size_t size, const std::string& name, const boost::python::object& dict)
{
    if (!data || !size)
        return;

    boost::python::object execNamespace;
    if (dict.is_none())
        execNamespace = main_namespace;
    else
        execNamespace = dict;

    PyGILState_STATE s = PyGILState_Ensure();
    try
    {
        boost::python::exec(boost::python::str(data, size), execNamespace);
    }
    catch (const boost::python::error_already_set& e)
    {
        handlePythonError();
    }
    PyGILState_Release(s);
}

void execPythonLine(const std::string& line, const std::string& name, const boost::python::object& dict)
{
    boost::python::object execNamespace;
    if (dict.is_none())
        execNamespace = main_namespace;
    else
        execNamespace = dict;

    PyGILState_STATE s = PyGILState_Ensure();
    try
    {
        boost::python::exec(line.c_str(), execNamespace);
    }
    catch (const boost::python::error_already_set& e)
    {
        handlePythonError();
    }
    PyGILState_Release(s);
}

void execPythonScript(const std::string& fileName, const boost::python::object& dict)
{
    size_t scriptSize;
    boost::shared_array<char> script = ResourceManager::instance().loadData(fileName, scriptSize);
    if (!script)
        return;
    execPythonScript(script.get(), scriptSize, fileName, dict);
}

void execPythonModule(const std::string& fileName, boost::python::object& moduleObject)
{
    size_t scriptSize;
    boost::shared_array<char> script = ResourceManager::instance().loadData(fileName, scriptSize);
    if (!script)
        return;


    moduleObject.attr("__dict__")["__builtins__"] = main_namespace["__builtins__"];
    execPythonScript(script.get(), scriptSize, fileName, moduleObject.attr("__dict__"));
}

void initPython()
{
    Py_InitializeEx(1);
    main_module =  boost::python::import("__main__");
    main_namespace = main_module.attr("__dict__");
}

void deinitPython()
{
    //Py_Finalize();
}

}