/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_PLUGIN_H
#define RANGERS_PLUGIN_H

#include "OpenSR/config.h"

#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

struct lua_State;

namespace Rangers
{
class RANGERS_ENGINE_API Plugin
{
public:
    Plugin(const std::wstring& path);

    int load();
    bool isLoaded() const;

    ~Plugin();

private:
    bool m_loaded;
    std::wstring m_path;
    int (*m_rangersPluginInit)();
    int (*m_rangersAPIVersion)();
    void (*m_rangersPluginDeinit)();
#ifdef _WIN32
    HMODULE m_handle;
#else
    void *m_handle;
#endif
};
}

#endif // RANGERS_PLUGIN_H
