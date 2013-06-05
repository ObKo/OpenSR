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

#ifndef RANGERS_FSADAPTER_H
#define RANGERS_FSADAPTER_H

#include "OpenSR/ResourceAdapter.h"

namespace Rangers
{
class FSAdapter: public ResourceAdapter
{
public:
    void load(const std::wstring& path);
    virtual ~FSAdapter();

    std::list<std::wstring> getFiles() const;
    std::istream* getStream(const std::wstring& name);

private:

#ifdef WIN32
    void scan(const std::wstring& path);
#else
    void scan(const std::string& path);
#endif

    std::wstring m_dirPath;
    std::list<std::wstring> m_files;
};
}

#endif // FSADAPTER_H
