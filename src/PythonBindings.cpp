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

#include "Python.h"
#include <boost/python.hpp>
#include "ResourceManager.h"
#include "Log.h"
#include <libRanger.h>

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

    std::wstring tracebackString;
    long lineno = 0;
    if (ptraceback)
    {
        boost::python::handle<> hTraceback(ptraceback);
        boost::python::object traceback(hTraceback);
        lineno =  boost::python::extract<long> (traceback.attr("tb_lineno"));
        boost::python::object tb(boost::python::import("traceback"));
        boost::python::object fmt_tb(tb.attr("format_tb"));
        boost::python::object tb_list(fmt_tb(hTraceback));
        boost::python::object tb_str(boost::python::str("\n").join(tb_list));
        tracebackString = boost::python::extract<std::wstring>(tb_str);
    }

    std::wstring strErrorMessage = boost::python::extract<std::wstring>(errorTextObject);
    std::wstring exceptionType = boost::python::extract<std::wstring>(extype.attr("__name__"));
    Log::error() << "[Python] " << exceptionType << ": " << strErrorMessage;

    if (!tracebackString.empty())
    {
        std::vector<std::wstring> lines = split(tracebackString, L'\n');
        std::vector<std::wstring>::const_iterator end = lines.end();
        for (std::vector<std::wstring>::const_iterator i = lines.begin(); i != end; ++i)
        {
            if (i->empty())
                continue;
            Log::error() << "[Python] " << (*i);
        }
    }
}

void execPythonScript(const char *data, size_t size, const std::wstring& name)
{
    if (!data || !size)
        return;

    try
    {
        boost::python::exec(boost::python::str(data, size), main_namespace);
    }
    catch (const boost::python::error_already_set& e)
    {
        handlePythonError();
    }
}

void execPythonLine(const std::wstring& line, const std::wstring& name)
{
    try
    {
        boost::python::exec(toUTF8(line).c_str(), main_namespace);
    }
    catch (const boost::python::error_already_set& e)
    {
        handlePythonError();
    }
}

void execPythonScript(const std::wstring& fileName)
{
    size_t scriptSize;
    boost::shared_array<char> script = ResourceManager::instance().loadData(fileName, scriptSize);
    if (!script)
        return;
    execPythonScript(script.get(), scriptSize, fileName);
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