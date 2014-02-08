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

#include <OpenSR/python/Wrappers.h>
#include <OpenSR/libRanger.h>
#include <OpenSR/Log.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
namespace
{
boost::python::list getChildren(DATRecord& self)
{
    return vectorToPythonList(self.children());
}

DATRecord& getItem(DATRecord& self, boost::python::object key)
{
    boost::python::extract<std::wstring> s(key);
    boost::python::extract<int> i(key);
    if (s.check())
    {
        DATRecord::iterator i = self.find(s);
        if (i != self.end())
            return *i;
        else
            throw std::out_of_range("DATRecord::getItem");
    }
    else if (i.check())
    {
        return self[i];
    }
    else
    {
        //FIXME: Ugly way to throw TypeError
        return self[s];
    }
}

void setItem(DATRecord& self, boost::python::object key, boost::python::object value)
{
    boost::python::extract<std::wstring> s(key);
    boost::python::extract<int> i(key);
    boost::python::extract<std::wstring> vs(value);
    boost::python::extract<DATRecord&> r(value);
    if (s.check())
    {
        if (r.check())
        {
            DATRecord &rec = r;
            if (rec.name != s())
            {
                Log::warning() << "[Python][DATRecord] __setitem__: inconsistent key and name, assuming key";
                rec.name = s;
            }
            self.add(r);
        }
        else if (vs.check())
        {
            DATRecord dummy;
            dummy.type = DATRecord::VALUE;
            dummy.name = s;
            dummy.value = vs;
            self.add(dummy);
        }
        else
        {
            //FIXME: Ugly way to throw TypeError
            self.add(r);
        }
    }
    else if (i.check())
    {
        int index = i;

        if (r.check())
        {
            if (index < self.size())
                self[index] = r;
            else
                self.add(r);
        }
        else if (vs.check())
        {
            DATRecord dummy;
            dummy.type = DATRecord::VALUE;
            dummy.value = vs;
            if (index < self.size())
                self[index] = dummy;
            else
                self.add(dummy);
        }
        else
        {
            //FIXME: Ugly way to throw TypeError
            self.add(r);
        }
    }
    else
    {
        //FIXME: Ugly way to throw TypeError
        DATRecord dummy;
        dummy.name = s;
    }
}
}
void exportRanger()
{
    using namespace boost::python;
    {
        scope s = class_<DATRecord, boost::shared_ptr<DATRecord>>("DATRecord", init<DATRecord::Type, std::wstring, std::wstring>())
                  .def(init<DATRecord::Type, std::wstring>())
                  .def(init<DATRecord::Type>())
                  .def(init<>())
                  .def_readwrite("type", &DATRecord::type)
                  .def_readwrite("name", &DATRecord::name)
                  .def_readwrite("value", &DATRecord::value)
                  .add_property("children", &getChildren)
                  .def("clear", &DATRecord::clear)
                  .def("add", &DATRecord::add)
                  .def("find", (DATRecord::iterator(DATRecord::*)(const std::wstring&))&DATRecord::find)
                  .def("__len__", &DATRecord::size)
                  .def("__iter__", iterator<DATRecord>())
                  .def("__getitem__", &getItem, return_internal_reference<>())
                  .def("__setitem__", &setItem);

        enum_<DATRecord::Type>("Type")
        .value("VALUE", DATRecord::VALUE)
        .value("NODE", DATRecord::NODE)
        .value("ARRAY", DATRecord::ARRAY);
    }
    boost::python::def("textHash32", (uint32_t (*)(const std::string&))&textHash32);
}
}
}