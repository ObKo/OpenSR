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

#ifndef RANGERS_RESOURCEADAPTER_H
#define RANGERS_RESOURCEADAPTER_H

#include <list>
#include <string>

namespace Rangers
{
class ResourceAdapter
{
public:
    virtual std::list<std::wstring> getFiles() const = 0;
    virtual char *loadData(const std::wstring& name, size_t& size) = 0;
};
};

#endif // RESOURCEADAPTER_H
