/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/FSAdapter.h"
#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#include <cerrno>
#include <algorithm>
#include <cstring>

#include <OpenSR/libRanger.h>

#include "OpenSR/Log.h"

using namespace std;

namespace Rangers
{
list< string > FSAdapter::getFiles() const
{
    list< string > keys;
    for (const std::pair<std::string, std::string> &kv : m_files)
    {
        keys.push_back(kv.first);
    }
    return keys;
}

void FSAdapter::load(const std::string& path)
{
    m_dirPath = path;
    if (m_dirPath.at(path.length() - 1) != '/')
        m_dirPath += '/';
#ifdef WIN32
    scan(L"");
#else
    scan("");
#endif
    if (m_files.size())
        Log::info() << "Loaded " << m_files.size() << " files from directory " << path;
}

#ifdef WIN32
void FSAdapter::scan(const wstring& path)
{
    wstring localPath = fromUTF8(m_dirPath) + path;
    WIN32_FIND_DATAW fd;
    HANDLE fh = FindFirstFileW((LPCWSTR)(localPath + L"*").c_str(), &fd);

    if (fh == INVALID_HANDLE_VALUE)
    {
        Log::error() << "Cannot open directory " << toUTF8(localPath) << ": " << strerror(errno);
        return;
    }
    do
    {
        if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L".."))
        {
            continue;
        }
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            scan(path + fd.cFileName + L'\\');
        else
        {
            wstring fileName = path + fd.cFileName;
            std::replace(fileName.begin(), fileName.end(), L'\\', L'/');
            std::wstring lcPath = fileName;
            std::transform(fileName.begin(), fileName.end(), lcPath.begin(), ::towlower);
            m_files[toUTF8(lcPath)] = toUTF8(fileName);
        }
    }
    while (FindNextFileW(fh, &fd));
}
#else
void FSAdapter::scan(const string& path)
{
    string localPath = m_dirPath + path;
    DIR* dir = opendir(localPath.c_str());
    if (!dir)
    {
        Log::error() << "Cannot open directory " << localPath.c_str() << ": " << strerror(errno);
        return;
    }
    dirent *current;
    while (current = readdir(dir))
    {
        if (!strcmp(current->d_name, ".") || !strcmp(current->d_name, ".."))
        {
            continue;
        }
        string fullPath = localPath + string(current->d_name);
        struct stat entry;
        if (!lstat(fullPath.c_str(), &entry))
        {

            switch (entry.st_mode & S_IFMT)
            {
            case S_IFDIR:
                scan(path + current->d_name + '/');
                break;
            case S_IFREG:
            {
                std::string p = path + current->d_name;
                std::string lp = p;
                std::transform(p.begin(), p.end(), lp.begin(), ::tolower);
                m_files[lp] = p;
                break;
            }
            default:
                Log::debug() << fullPath.c_str() << " unknown entry type";
            }
        }
        else
        {
            Log::debug() << "Cannot lstat: " << strerror(errno);
        }
    }
    closedir(dir);
}
#endif

std::istream* FSAdapter::getStream(const std::string& name)
{
    std::string realPath = m_files.at(name);
#ifdef WIN32
    ifstream *s = new ifstream(fromUTF8(m_dirPath + realPath), ios::in | ios::binary);
#else
    ifstream *s = new ifstream(m_dirPath + realPath, ios::in | ios::binary);
#endif
    if (!s->is_open())
    {
        Log::error() << "Cannot open file " << name << " from FS: " << strerror(errno);
        delete s;
        return 0;
    }
    return s;
}

FSAdapter::~FSAdapter()
{

}
}
