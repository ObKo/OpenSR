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

#include "OpenSR/Log.h"

#include <ctime>
#include <iostream>
#include <libRanger.h>

#include "OpenSR/Types.h"

using namespace std;

namespace Rangers
{
Log *logInstance = 0;

Log::Log()
{
    m_needUpdate = false;
}

bool Log::colorOutput() const
{
    return m_colorOutput;
}

void Log::setColorOutput(bool colorOutput)
{
    m_colorOutput = colorOutput;
}

Log* Log::instance()
{
    if (!logInstance)
        logInstance = new Log();

    return logInstance;
}

void Log::writeLogEntry(const LogEntry& s)
{
    m_bufferMutex.lock();
    std::vector<std::wstring> l = split(s.m_text, L'\n');
    std::vector<std::wstring>::iterator end = l.end();
    for (std::vector<std::wstring>::iterator i = l.begin(); i != end; ++i)
    {
        LogEntry e = s;
        e.m_text = (*i);
        m_logEntries.push_back(e);

        wstring prefix;

        switch (e.m_level)
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
        if (m_colorOutput)
        {
            switch (e.m_level)
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
            cout << toLocal(prefix + e.m_text) << "\e[0m" << endl;
        }
        else
            cout << toLocal(prefix + e.m_text) << endl;
#elif _WIN32
        cout << toLocal(prefix + e.m_text) << endl;
#else
        cout << toLocal(prefix + e.m_text) << endl;
#endif
    }
    m_needUpdate = true;

    m_bufferMutex.unlock();
}

bool Log::needUpdate() const
{
    bool r = m_needUpdate;
    m_needUpdate = false;
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

std::list<LogEntry> Log::getLastLines(int n) const
{
    std::list<LogEntry> r;

    std::list<LogEntry>::const_iterator begin = m_logEntries.end();
    for (int i = 0; i < n; i++)
        if (begin != m_logEntries.begin())
            begin--;

    for (std::list<LogEntry>::const_iterator i = begin; i != m_logEntries.end(); i++)
        r.push_back(*i);

    return r;
}

LogEntry::LogEntry(LogLevel l, wstring t, unsigned int ts): m_level(l), m_text(t), m_timestamp(ts)
{
}

LogEntry::LogEntry(): m_level(LDEBUG), m_text(L""), m_timestamp(0)
{
}

Log::Logger::Logger(LogLevel level)
{
    m_logLevel = level;
    m_stream = boost::shared_ptr<std::wostringstream>(new std::wostringstream());
}

Log::Logger::Logger(const Rangers::Log::Logger& other)
{
    m_logLevel = other.m_logLevel;
    m_stream = other.m_stream;
}

Log::Logger::~Logger()
{
    if (m_stream.use_count() == 1)
        Log::instance()->writeLogEntry(LogEntry(m_logLevel, m_stream->str(), time(0)));
}

Log::Logger& Log::Logger::operator<<(const std::wstring& v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(int v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(long v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(long long v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(float v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(bool v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const std::string& v)
{
    wstring w;
    w.assign(v.length(), '\0');
    std::copy(v.begin(), v.end(), w.begin());
    *m_stream << w;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const char *s)
{
    wstring w;
    string v(s);
    w.assign(v.length(), '\0');
    std::copy(v.begin(), v.end(), w.begin());
    *m_stream << w;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const wchar_t *v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator <<(unsigned int v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator <<(unsigned long v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator <<(unsigned long long v)
{
    *m_stream << v;
    return *this;
}

Log::Logger& Log::Logger::operator<<(const Rect& r)
{
    *m_stream << "[" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << "]";
    return *this;
}

Log::Logger& Log::Logger::operator<<(const Vector& v)
{
    *m_stream << "[" << v.x << ", " << v.y << "]";
    return *this;
}
}
