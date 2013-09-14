/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Plugin.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <OpenSR/libRanger.h>

#include "OpenSR/Log.h"
#include "OpenSR/version.h"

namespace Rangers
{
Plugin::Plugin(const std::wstring& path):
    m_path(path), m_loaded(false), m_handle(0),
    m_rangersPluginInit(0), m_rangersAPIVersion(0),
    m_rangersPluginDeinit(0)
{
}

Plugin::~Plugin()
{
    if (m_loaded)
    {
#ifdef _WIN32
        FreeLibrary(m_handle);
#else
        dlclose(m_handle);
#endif
    }
}

int Plugin::load()
{
    if (m_loaded)
        return 0;
#ifdef _WIN32
    m_handle = LoadLibrary(m_path.c_str());
    if (!m_handle)
    {
        Log::error() << "Cannot load plugin \"" << m_path << ";";
        return -1;
    }
    m_rangersPluginInit = (int (*)())GetProcAddress(m_handle, "rangersPluginInit");
    m_rangersAPIVersion = (int (*)())GetProcAddress(m_handle, "rangersAPIVersion");
    m_rangersPluginDeinit = (void (*)())GetProcAddress(m_handle, "rangersPluginDeinit");

    if (!m_rangersPluginInit && !m_rangersAPIVersion && !m_rangersPluginDeinit)
    {
        Log::error() << "Cannot load some plugin symbols";
        FreeLibrary(m_handle);
        m_handle = 0;
        return -1;
    }
    if (m_rangersAPIVersion() != RANGERS_API_VERSION)
    {
        Log::error() << "Plugin requires API v" << m_rangersAPIVersion() << ", but engine is v" << RANGERS_API_VERSION;
        FreeLibrary(m_handle);
        m_handle = 0;
        return -1;
    }
    if (m_rangersPluginInit())
    {
        Log::error() << "Error during plugin init";
        FreeLibrary(m_handle);
        m_handle = 0;
        return -1;
    }
#else
    m_handle = dlopen(toLocal(m_path).c_str(), RTLD_LAZY);
    if (!m_handle)
    {
        Log::error() << "Cannot load plugin \"" << m_path << "\": " << fromLocal(dlerror());
        return -1;
    }
    m_rangersPluginInit = (int (*)())dlsym(m_handle, "rangersPluginInit");
    m_rangersAPIVersion = (int (*)())dlsym(m_handle, "rangersAPIVersion");
    m_rangersPluginDeinit = (void (*)())dlsym(m_handle, "rangersPluginDeinit");

    if (!m_rangersPluginInit && !m_rangersAPIVersion && !m_rangersPluginDeinit)
    {
        Log::error() << "Cannot load some plugin symbols";
        dlclose(m_handle);
        m_handle = 0;
        return -1;
    }
    if (m_rangersAPIVersion() != RANGERS_API_VERSION)
    {
        Log::error() << "Plugin requires API v" << m_rangersAPIVersion() << ", but engine is v" << RANGERS_API_VERSION;
        dlclose(m_handle);
        m_handle = 0;
        return -1;
    }
    if (m_rangersPluginInit())
    {
        Log::error() << "Error during plugin init";
        dlclose(m_handle);
        m_handle = 0;
        return -1;
    }
#endif

    m_loaded = true;
    return 0;
}

bool Plugin::isLoaded() const
{
    return m_loaded;
}
}
