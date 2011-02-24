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

#ifndef RANGERS_LOG_H
#define RANGERS_LOG_H
#include <sstream>
#include <list>
#include <SDL_mutex.h>

namespace Rangers
{
enum LogLevel {LEND, LDEBUG, LINFO, LWARNING, LERROR};

struct LogEntry
{
    LogEntry(LogLevel l, std::wstring t, unsigned int ts);
    LogEntry();
    LogLevel level;
    std::wstring text;
    unsigned int timestap;
};

class Log
{
public:
    Log();
    ~Log();

    bool checkForUpdate();
    std::list<LogEntry> lines(int n = -1) const;
    void writeLogEntry(const LogEntry& s);

    Log& operator <<(LogLevel l);

    Log& operator <<(const std::wstring& v);
    Log& operator <<(int v);
    Log& operator <<(size_t v);
    Log& operator <<(bool v);
    Log& operator <<(const std::string& v);
    Log& operator <<(const char *v);
    Log& operator <<(const wchar_t *v);

private:
    bool isNew;
    std::list<LogEntry> logs;
    SDL_mutex *bufferMutex;

    LogEntry currentEntry;
    std::wostringstream currentStream;
};

Log &logger();

/*LogEntry Error(const std::wstring& s);
LogEntry Warning(const std::wstring& s);
LogEntry Info(const std::wstring& s);
LogEntry Debug(const std::wstring& s);*/

};

#endif // LOG_H
