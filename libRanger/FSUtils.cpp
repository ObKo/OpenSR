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

#include "libRanger.h"

#ifdef WIN32
#include <windows.h>
#else
#include <sys/stat.h>
int createDir(const std::string &path, mode_t mode)
{
    struct stat st;
    int status = 0;

    if (stat(path.c_str(), &st) != 0)
    {
        if (mkdir(path.c_str(), mode) != 0)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
        status = -1;

    return status;
}
#endif

namespace Rangers
{
bool createDirPath(const std::wstring& path)
{
#ifdef WIN32
    //FIXME: Will work only on WinXP SP2 or newer.
    if(SHCreateDirectoryEx(NULL, path, NULL) != ERROR_SUCCESS)
        return false;
#else
    int startPos = 0;
    int endPos = 0;
    while((endPos = path.find(L'/', startPos)) != std::wstring::npos)
    {
        if(createDir(toLocal(path.substr(0, endPos + 1)).c_str(), 0666))
            return false;
        startPos = endPos + 1;
    }
#endif
    return true;
}
}
