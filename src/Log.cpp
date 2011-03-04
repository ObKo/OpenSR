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

#include "Log.h"
#include <ctime>
#include <iostream>
#include <libRanger.h>

using namespace Rangers;
using namespace std;

Log *logInstance = 0;

Log::Log()
{
    bufferMutex = SDL_CreateMutex();
    isNew = false;
    currentEntry.level = LEND;
    currentEntry.timestap = time(0);
}

void Log::writeLogEntry(const LogEntry& s)
{
    SDL_mutexP(bufferMutex);
    logs.push_back(s);
    isNew = true;
    
    switch(s.level)
    {
    case LDEBUG:
	cout << "[--] ";
	break;
    case LINFO:
	cout << "[II] ";
	break;
    case LWARNING:
	cout << "[WW] ";
	break;
    case LERROR:
	cout << "[EE] ";
	break;
    }
    cout << toLocal(s.text) << endl;
    
    SDL_mutexV(bufferMutex);
}

bool Log::checkForUpdate()
{
    bool r = isNew;
    isNew = false;
    return r;
}


Log::~Log()
{
    SDL_DestroyMutex(bufferMutex);
}

Log& Rangers::logger()
{
    if (!logInstance)
        logInstance = new Log();

    return *logInstance;
}

std::list<LogEntry> Log::lines(int n) const
{
    std::list<LogEntry> r;

    std::list<LogEntry>::const_iterator begin = logs.end();
    for (int i = 0; i < n; i++)
        if (begin != logs.begin())
            begin--;

    for (std::list<LogEntry>::const_iterator i = begin; i != logs.end(); i++)
        r.push_back(*i);

    return r;
}

LogEntry::LogEntry(LogLevel l, wstring t, unsigned int ts): level(l), text(t), timestap(ts)
{
}

LogEntry::LogEntry(): level(LDEBUG), text(L""), timestap(0)
{
}

Log& Log::operator<<(LogLevel l)
{
    if ((currentStream.str() != L"") && (currentEntry.level != LEND))
    {
        currentEntry.text = currentStream.str();
        writeLogEntry(currentEntry);
    }

    currentEntry.level = l;
    currentEntry.text = L"";
    currentEntry.timestap = time(0);
    currentStream.str(L"");
    return *this;
}

Log& Log::operator<<(const std::wstring& v)
{
    currentStream << v;
    return *this;
}

Log& Log::operator<<(int v)
{
    currentStream << v;
    return *this;
}

Log& Log::operator<<(float v)
{
    currentStream << v;
    return *this;
}

Log& Log::operator<<(bool v)
{
    currentStream << v;
    return *this;
}

Log& Log::operator<<(const std::string& v)
{
    wstring w;
    w.assign(v.length(), '\0');
    std::copy(v.begin(), v.end(), w.begin());
    currentStream << w;
    return *this;
}

Log& Log::operator<<(const char *s)
{
    wstring w;
    string v(s);
    w.assign(v.length(), '\0');
    std::copy(v.begin(), v.end(), w.begin());
    currentStream << w;
    return *this;
}

Log& Log::operator<<(const wchar_t *v)
{
    currentStream << v;
    return *this;
}

Log& Log::operator<<(size_t v)
{
    currentStream << v;
    return *this;
}


/*LogEntry Rangers::Debug(const std::wstring& s)
{
    return LogEntry(DEBUG, s, time(0));
}

LogEntry Rangers::Error(const std::wstring& s)
{
    return LogEntry(ERROR, s, time(0));
}

LogEntry Rangers::Info(const std::wstring& s)
{
    return LogEntry(INFO, s, time(0));
}

LogEntry Rangers::Warning(const std::wstring& s)
{
    return LogEntry(WARNING, s, time(0));
}*/

