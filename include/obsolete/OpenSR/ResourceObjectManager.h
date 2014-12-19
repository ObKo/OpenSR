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

#ifndef RANGERS_RESOURCEOBJECTMANAGER_H
#define RANGERS_RESOURCEOBJECTMANAGER_H

#include <OpenSR/Types.h>

#include <list>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

namespace Json
{
class Value;
}

namespace Rangers
{
class RANGERS_ENGINE_API ResourceObjectManager
{
public:
    class RANGERS_ENGINE_API ResourceObjectFactory
    {
    public:
        virtual boost::shared_ptr<ResourceObject> operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager &manager) = 0;

        static Color parseColor(const Json::Value& object);
        static Rect parseRect(const Json::Value& object);
        static Vector parseVector(const Json::Value& object);
    };

    ResourceObjectManager();

    boost::shared_ptr<ResourceObject> getResourceObject(const std::string& id);

    template <class T>
    boost::shared_ptr<T> getObject(const std::string& id)
    {
        return boost::dynamic_pointer_cast<T>(getResourceObject(id));
    }

    void addJSON(const std::string& jsonFile, const std::string& prefix = std::string());
    void addFactory(const std::string& type, boost::shared_ptr<ResourceObjectFactory> factory);

    boost::shared_ptr<ResourceObject> loadObject(const std::string& currentPath, const Json::Value& object);

    //! Load object without id or type
    boost::shared_ptr<ResourceObject> loadObject(const Json::Value& object, const std::string& type = std::string());
    //! Load object without id or type from file
    boost::shared_ptr<ResourceObject> loadObject(const std::string& jsonFile, const std::string& type = std::string());

private:
    std::list<std::string> m_directories;
    std::map<std::string, boost::shared_ptr<ResourceObject> > m_objects;
    std::map<std::string, std::string> m_objectFiles;

    std::map<std::string, boost::shared_ptr<ResourceObjectFactory> > m_objectFactories;

    void registerDefaultFactories();

    class DirectoryFactory;
};
}

#endif
