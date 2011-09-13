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
#include <boost/thread/recursive_mutex.hpp>
#include <boost/shared_ptr.hpp>

namespace Rangers
{
enum LogLevel {LDEBUG, LINFO, LWARNING, LERROR};

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
    class Logger;

public:
    Log();

    static Log *instance();
    static Logger debug();
    static Logger info();
    static Logger warning();
    static Logger error();

    bool checkForUpdate();
    std::list<LogEntry> lines(int n = -1) const;
    void writeLogEntry(const LogEntry& s);

private:
    bool isNew;
    bool colorOutput;
    std::list<LogEntry> logs;
    boost::recursive_mutex bufferMutex;

    LogEntry currentEntry;
};

class Log::Logger
{
public:
    Logger(LogLevel level);
    Logger(const Logger& other);
    ~Logger();

    Logger& operator <<(LogLevel l);

    Logger& operator <<(const std::wstring& v);
    Logger& operator <<(int v);
    Logger& operator <<(float v);
    Logger& operator <<(size_t v);
    Logger& operator <<(bool v);
    Logger& operator <<(const std::string& v);
    Logger& operator <<(const char *v);
    Logger& operator <<(const wchar_t *v);

private:
    LogLevel m_logLevel;
    boost::shared_ptr<std::wostringstream> loggerStream;
};

};

#endif // LOG_H
