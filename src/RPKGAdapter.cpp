/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/RPKGAdapter.h"

#include <sstream>
#include <cerrno>
#include <cstring>
#include <libRanger.h>

#include "OpenSR/Log.h"

using namespace std;

namespace Rangers
{
//FIXME: Pass std::wstring to std::ifstream constructor
void RPKGAdapter::load(const wstring& fileName)
{
    rpkgArchive.open(toLocal(fileName).c_str(), ios::binary | ios::in);
    if (!rpkgArchive.is_open())
    {
        Log::error() << "Cannot load RPKG archive " << fileName << ": " << fromLocal(strerror(errno));
        return;
    }
    std::list<RPKGEntry> l = loadRPKG(rpkgArchive);

    for (std::list<RPKGEntry>::const_iterator i = l.begin(); i != l.end(); ++i)
        files[i->name] = *i;

    Log::info() << "Loaded " << files.size() << " files from RPKG archive " << fileName;
    m_fileName = fileName;
}

RPKGAdapter::~RPKGAdapter()
{
    rpkgArchive.close();
}


list<wstring> RPKGAdapter::getFiles() const
{
    list<wstring> result;
    for (map<wstring, RPKGEntry>::const_iterator i = files.begin(); i != files.end(); ++i)
        result.push_back(i->first);

    return result;
}

std::istream* RPKGAdapter::getStream(const std::wstring& name)
{
    if (files.find(name) == files.end())
    {
        Log::error() << "No such file in RPKG archive: " << name;
        return 0;
    }
    RPKGEntry e = files[name];
    return getRPKGFileStream(e, boost::shared_ptr<std::istream>(new std::ifstream(toLocal(m_fileName).c_str(), ios::binary | ios::in)));
}
}
