/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#include "RPKGAdapter.h"
#include "libRanger.h"
#include "Log.h"
#include <sstream>
#include <cerrno>

using namespace Rangers;
using namespace std;

void RPKGAdapter::load(const wstring& fileName)
{
    rpkgArchive.open(toLocal(fileName).c_str(), ios::binary | ios::in);
    if (!rpkgArchive.is_open())
    {
        logger() << LERROR << "Cannot RPKG archive " << fileName << ": " << fromLocal(strerror(errno)) << LEND;
        return;
    }
    std::list<RPKGEntry> l = loadRPKG(rpkgArchive);

    for (std::list<RPKGEntry>::const_iterator i = l.begin(); i != l.end(); i++)
        files[i->name] = *i;

    logger() << LINFO << "Loaded " << files.size() << " files from RPKG archive " << fileName << LEND;
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

char* RPKGAdapter::loadData(const wstring& name, size_t& size)
{
    if (files.find(name) == files.end())
    {
        logger() << LERROR << "No such file " << name << LEND;
        return 0;
    }
    return extractFile(files[name], rpkgArchive, size);
}
