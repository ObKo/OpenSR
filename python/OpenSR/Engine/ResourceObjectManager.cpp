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

#include <OpenSR/ResourceObjectManager.h>
#include <json/value.h>

namespace Rangers
{
void handlePythonError();
namespace Python
{
struct ResourceObjectFactoryWrap: ResourceObjectManager::ResourceObjectFactory, boost::python::wrapper<ResourceObjectManager::ResourceObjectFactory>
{
    ResourceObjectFactoryWrap()
    {
    }

    virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager)
    {
        Rangers::Python::GILGuard g;
        try
        {
            return this->get_override("__call__")(currentPath, object, manager);
        }
        catch (const boost::python::error_already_set& e)
        {
            handlePythonError();
        }
        return boost::shared_ptr<ResourceObject>();
    }
};

void exportResourceObjectManager()
{
    using namespace boost::python;

    class_<ResourceObjectManager, boost::noncopyable>("ResourceObjectManager", init<>())
    .def("getResourceObject", &ResourceObjectManager::getResourceObject)
    .def("addJSON", (void (ResourceObjectManager::*)(const std::wstring&, const std::string&))&ResourceObjectManager::addJSON)
    .def("addJSON", (void (ResourceObjectManager::*)(const std::wstring&))&ResourceObjectManager::addJSON)
    .def("addFactory", &ResourceObjectManager::addFactory)
    .def("loadObject", (boost::shared_ptr<ResourceObject> (ResourceObjectManager::*)(const std::wstring&, const std::string&))&ResourceObjectManager::loadObject);

    class_<ResourceObjectFactoryWrap, boost::shared_ptr<ResourceObjectFactoryWrap>, boost::noncopyable>("ResourceObjectFactory", init<>())
    .def("__call__", pure_virtual(&ResourceObjectManager::ResourceObjectFactory::operator()))
    .def("parseColor", &ResourceObjectManager::ResourceObjectFactory::parseColor)
    .staticmethod("parseColor")
    .def("parseRect", &ResourceObjectManager::ResourceObjectFactory::parseRect)
    .staticmethod("parseRect")
    .def("parseVector", &ResourceObjectManager::ResourceObjectFactory::parseVector)
    .staticmethod("parseVector");
}
}
}
