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
    isNew = false;
    currentEntry.timestap = time(0);
}

Log* Log::instance()
{
    if (!logInstance)
        logInstance = new Log();
    
    return logInstance;
}

void Log::writeLogEntry(const LogEntry& s)
{
    bufferMutex.lock();
    logs.push_back(s);
    isNew = true;
    
    wstring prefix;
    
    switch(s.level)
    {
    case LDEBUG:
	prefix = L"[--] ";
	break;
    case LINFO:
	prefix = L"[II] ";
	break;
    case LWARNING:
	prefix = L"[WW] ";
	break;
    case LERROR:
	prefix = L"[EE] ";
	break;
    }
    
#ifdef __unix__
    switch(s.level)
    {
    case LINFO:
	prefix = L"\e[0;32m" + prefix;
	break;
    case LWARNING:
	prefix = L"\e[0;33m" + prefix;
	break;
    case LERROR:
	prefix = L"\e[0;31m" + prefix;
	break;
    }
    cout << toLocal(prefix + s.text) << "\e[0m" << endl;
#elif _WIN32
    cout << toLocal(prefix + s.text) << endl;
#else
    cout << toLocal(prefix + s.text) << endl;
#endif
    
    
    bufferMutex.unlock();
}

bool Log::checkForUpdate()
{
    bool r = isNew;
    isNew = false;
    return r;
}

Log::Logger Log::debug()
{
    return Log::Logger(LDEBUG);
}

Log::Logger Log::info()
{
    return Log::Logger(LINFO);
}

Log::Logger Log::warning()
{
    return Log::Logger(LWARNING);
}

Log::Logger Log::error()
{
    return Log::Logger(LERROR);
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

Log::Logger::Logger(LogLevel level)
{
    m_logLevel = level;
    loggerStream = boost::shared_ptr<std::wostringstream>(new std::wostringstream());
}

Log::Logger::Logger(const Rangers::Log::Logger& other)
{
    m_logLevel = other.m_logLevel;
    loggerStream = other.loggerStream;
}

Log::Logger::~Logger()
{
    if(loggerStream.use_count() == 1)
        Log::instance()->writeLogEntry(LogEntry(m_logLevel, loggerStream->str(), time(0)));
}

Log::Logger& Log::Logger::operator<<(const std::wstring& v)
{
    *loggerStream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(int v)
{
    *loggerStream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(float v)
{
    *loggerStream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(bool v)
{
    *loggerStream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const std::string& v)
{
    wstring w;
    w.assign(v.length(), '\0');
    std::copy(v.begin(), v.end(), w.begin());
    *loggerStream << w;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const char *s)
{
    wstring w;
    string v(s);
    w.assign(v.length(), '\0');
    std::copy(v.begin(), v.end(), w.begin());
    *loggerStream << w;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const wchar_t *v)
{
    *loggerStream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(size_t v)
{
    *loggerStream << v;
    return *this;
}

