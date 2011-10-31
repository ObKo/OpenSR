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
#include <iconv.h>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <locale.h>
#include <langinfo.h>
#endif


//TODO: Fix resulting length

/*!
 * \param to input encoding
 * \param from output encoding
 * \param what input string
 * \param srcLength input string size
 * \param destLength output string size
 * \return converted string
 */
char *Rangers::convertText(const char *to, const char *from, const char *what, int srcLength, int &destLength)
{
    iconv_t codec = iconv_open(to, from);
    if (codec == (iconv_t) - 1)
    {
        std::cerr << "Cannot open text codec: " << strerror(errno) << std::endl;
        char *r = new char[srcLength + 1];
        destLength = srcLength + 1;
        memcpy(r, what, srcLength + 1);
        return r;
    }
    if (srcLength < 0)
        srcLength = strlen(what);
    char *result = new char[4 * (srcLength + 1)];

    size_t inbuflength = srcLength + 1;
    size_t outbuflength = 4 * (srcLength + 1);
    char *pointer = result;
    char **inp = (char **)&what;
#ifdef _WIN32
    iconv(codec, (const char **)inp, &inbuflength, &pointer, &outbuflength);
#else
    iconv(codec, inp, &inbuflength, &pointer, &outbuflength);
#endif
    size_t l = 4 * (srcLength + 1) - outbuflength;

    //if(result[l])
    //{
    //  result[l] = '\0';
    //  outbuflength--;
    //}

    result = (char *)realloc(result, l);
    iconv_close(codec);
    destLength = 4 * (srcLength + 1) - outbuflength;
    return result;
}

/*!
 * \param codec input encoding
 * \param text input string
 * \param length input string size
 * \return converted string
 */
std::wstring Rangers::fromCodec(const char *codec, const char *text, int length)
{
    if (length < 0)
        length = strlen(text);

    int outl;

    //FIXME: Workaround about not working WCHAR_T on Windows XP
#ifdef _WIN32
    char *data = convertText("UCS-2LE", codec, text, length, outl);
#else
    char *data = convertText("WCHAR_T", codec, text, length, outl);
#endif

    if (!data)
        return std::wstring();

    std::wstring result = std::wstring((wchar_t *)data);
    delete data;
    return result;
}

std::wstring Rangers::fromUTF8(const char *text, int length)
{
    return fromCodec("UTF-8", text, length);
}

std::wstring Rangers::fromASCII(const char *text, int length)
{
    return fromCodec("ASCII", text, length);
}

/*!
 * \param codec output encoding
 * \param text input string
 * \param resultLength output string size
 * \return converted string
 */
char* Rangers::toCodec(const char *codec, const std::wstring& text, int& resultLength)
{
//FIXME: Workaround about not working WCHAR_T on Windows XP
#ifdef _WIN32
    return convertText(codec, "UCS-2LE", (char *)text.c_str(), (text.length() + 1) * sizeof(wchar_t), resultLength);
#else
    return convertText(codec, "WCHAR_T", (char *)text.c_str(), (text.length() + 1) * sizeof(wchar_t), resultLength);
#endif
}

/*!
 * \param text input string
 * \return converted string
 */
std::string Rangers::toASCII(const std::wstring& text)
{
    int resultLength;
    char *s = toCodec("ASCII", text, resultLength);
    std::string r(s, resultLength - 1);
    delete s;
    return r;
}

/*!
 * \param text input string
 * \return converted string
 */
std::string Rangers::toUTF8(const std::wstring& text)
{
    int resultLength;
    char *s = toCodec("UTF-8", text, resultLength);
    std::string r(s, resultLength - 1);
    delete s;
    return r;
}

/*!
 * \param text input string
 * \return converted string
 */
std::string Rangers::toLocal(const std::wstring& text)
{
    std::string codec;
#ifdef _WIN32
    int cp = GetACP();
    std::ostringstream ss;
    ss << "CP" << cp;
    codec = ss.str();
#else
    setlocale(LC_ALL, "");
    codec = nl_langinfo(CODESET);
#endif
    int resultLength;
    char *s = toCodec(codec.c_str(), text, resultLength);
    std::string r(s, resultLength - 1);
    delete s;
    return r;
}

std::wstring Rangers::fromLocal(const char *text, int length)
{
    const char *codec;
#ifdef _WIN32
    int cp = GetACP();
    std::ostringstream ss;
    ss << "CP" << cp;
    codec = ss.str().c_str();
#else
    setlocale(LC_ALL, "");
    codec = nl_langinfo(CODESET);
#endif
    return fromCodec(codec, text, length);
}

/*!
 * \param s string
 * \param c separator
 * \return array of strings
 */
std::vector<std::wstring> Rangers::split(const std::wstring& s, wchar_t c)
{
    std::vector<std::wstring> array;
    for (std::wstring::const_iterator it = s.begin(); it != s.end();)
    {
        while (((*it) == c) && (it != s.end()))
            it++;
        std::wstring::const_iterator begin = it;
        while (((*it) != c) && (it != s.end()))
            it++;
        if (it != begin)
            array.push_back(s.substr(begin - s.begin(), it - begin));
    }
    return array;
}

/*!
 * \param s file path
 * \return file extension
 */
std::wstring Rangers::suffix(const std::wstring& s)
{
    int pos;
    if ((pos = s.rfind(L'.')) == std::wstring::npos)
        return std::wstring();
    else
        return s.substr(pos + 1);
}

/*!
 * \param s file path
 * \return file basename
 */
std::wstring Rangers::basename(const std::wstring& s)
{
    int endpos = s.rfind(L'.');
    int startpos = s.rfind(L'/');

    if (endpos == std::wstring::npos)
    {
        if (startpos == std::wstring::npos)
            return s;
        else
            return s.substr(startpos + 1);
    }
    else
    {
        if (startpos == std::wstring::npos)
            return s.substr(0, endpos);
        else
            return s.substr(startpos + 1, endpos - startpos - 1);
    }
}

/*!
 * \param s file path
 * \return file directory
 */
std::wstring Rangers::directory(const std::wstring& s)
{
    int pos;
    if ((pos = s.rfind(L'/')) == std::wstring::npos)
#ifdef WIN32
        if ((pos = s.rfind(L'\\')) == std::wstring::npos)
#endif
            return s;
    return s.substr(0, pos + 1);
}

/*!
 * \param s string
 * \param c separator
 * \return array of strings
 */
std::vector<std::string> Rangers::split(const std::string& s, char c)
{
    std::vector<std::string> array;
    for (std::string::const_iterator it = s.begin(); it != s.end();)
    {
        while ((it != s.end()) && ((*it) == c))
            it++;
        std::string::const_iterator begin = it;
        while ((it != s.end()) && ((*it) != c))
            it++;
        if (it != begin)
            array.push_back(s.substr(begin - s.begin(), it - begin));
    }
    return array;
}

/*!
 * \param s file path
 * \return file extension
 */
std::string Rangers::suffix(const std::string& s)
{
    int pos;
    if ((pos = s.rfind(L'.')) == std::string::npos)
        return std::string();
    else
        return s.substr(pos + 1);
}

/*!
 * \param s file path
 * \return file basename
 */
std::string Rangers::basename(const std::string& s)
{
    int endpos = s.rfind(L'.');
    int startpos = s.rfind(L'/');
#ifdef WIN32
    if (startpos == std::wstring::npos)
        startpos = s.rfind(L'\\');
#endif

    if (endpos == std::string::npos)
    {
        if (startpos == std::string::npos)
            return s;
        else
            return s.substr(startpos + 1);
    }
    else
    {
        if (startpos == std::string::npos)
            return s.substr(0, endpos);
        else
            return s.substr(startpos + 1, endpos - startpos - 1);
    }
}

/*!
 * \param s file path
 * \return file directory
 */
std::string Rangers::directory(const std::string& s)
{
    int pos;
    if ((pos = s.rfind(L'/')) == std::string::npos)
#ifdef WIN32
        if ((pos = s.rfind(L'\\')) == std::string::npos)
#endif
            return s;
    return s.substr(0, pos);
}

/*!
 * \param s input string
 * \return trimmed string
 */
std::wstring Rangers::trim(const std::wstring& s)
{
    std::wstring result = s;
    std::wstring::iterator end = result.end();
    std::wstring::iterator spaceEnd;
    for (spaceEnd = result.begin(); (spaceEnd != end) && ((*spaceEnd == L' ') || (*spaceEnd == L'\t') || (*spaceEnd == L'\n') || (*spaceEnd == L'\r')); ++spaceEnd);
    if (spaceEnd != result.begin())
        result.erase(result.begin(), spaceEnd);
    end = result.begin();
    for (spaceEnd = result.end(); (spaceEnd != end) && ((*(spaceEnd - 1) == L' ') || (*(spaceEnd - 1) == L'\t') || (*(spaceEnd - 1) == L'\n') || (*(spaceEnd - 1) == L'\r')); --spaceEnd);
    if (spaceEnd != result.end())
        result.erase(spaceEnd, result.end());
    return result;
}

/*!
 * \param s input string
 * \return trimmed string
 */
std::string Rangers::trim(const std::string& s)
{
    std::string result = s;
    std::string::iterator end = result.end();
    std::string::iterator spaceEnd;
    for (spaceEnd = result.begin(); (spaceEnd != end) && ((*spaceEnd == ' ') || (*spaceEnd == '\t') || (*spaceEnd == '\n') || (*spaceEnd == '\r')); ++spaceEnd);
    if (spaceEnd != result.begin())
        result.erase(result.begin(), spaceEnd);
    end = result.begin();
    for (spaceEnd = result.end(); (spaceEnd != end) && ((*(spaceEnd - 1) == ' ') || (*(spaceEnd - 1) == '\t') || (*(spaceEnd - 1) == '\n') || (*(spaceEnd - 1) == '\r')); --spaceEnd);
    if (spaceEnd != result.end())
        result.erase(spaceEnd, result.end());
    return result;
}
