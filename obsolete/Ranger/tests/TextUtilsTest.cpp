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

#include <libRanger.h>
#include <string>
#include <iostream>

#define TEST_ASSERT(name, x, y) if(x != y) { \
        std::wcout << "Test " #name " failed!" << std::endl; \
        return -1; \
    }
using namespace Rangers;

int main(int argc, char **argv)
{
    std::wstring wstr;
    std::string str;

    wstr = fromCodec("", "");
    TEST_ASSERT(fromCodecNull, wstr, L"");

    wstr = fromCodec("invalid_codec", "Test string");
    TEST_ASSERT(fromCodecInvalidCodec, wstr, L"");

    wstr = fromCodec("ASCII", "\xD2\xE5\xF1\xF2");
    TEST_ASSERT(fromCodecInvalidData, wstr, L"");

    wstr = fromCodec("UTF-8", "\xD0\xA2\xD0\xB5\xD1\x81\xD1\x82");
    TEST_ASSERT(fromCodecUTF8, wstr, L"Тест");

    wstr = fromCodec("CP1251", "\xD2\xE5\xF1\xF2");
    TEST_ASSERT(fromCodecCP1251, wstr, L"Тест");

    wstr = fromCodec("WCHAR_T", (char *)L"Тест", 5 * sizeof(wchar_t));
    TEST_ASSERT(fromCodecWCHAR, wstr, L"Тест");

    wstr = fromCodec("WCHAR_T", (char *)L"Тест", 0);
    TEST_ASSERT(fromCodecWCHARNULL, wstr, L"");


    str = toCodec("", L"");
    TEST_ASSERT(toCodecNull, str, "");

    str = toCodec("invalid_codec", L"Test string");
    TEST_ASSERT(toCodecInvalidCodec, str, "");

    str = toCodec("UTF-8", L"Тест");
    TEST_ASSERT(toCodecUTF8, str, "\xD0\xA2\xD0\xB5\xD1\x81\xD1\x82");

    str = toCodec("CP1251", L"Тест");
    TEST_ASSERT(toCodecCP1251, str, "\xD2\xE5\xF1\xF2");

    str = toCodec("WCHAR_T", L"Тест");
    TEST_ASSERT(toCodecWCHAR, str, std::string((char *)L"Тест", 0, 4 * sizeof(wchar_t)));

    str = toCodec("WCHAR_T", L"");
    TEST_ASSERT(toCodecWCHARNULL, str, "");

    std::vector<std::wstring> splitW;
    std::vector<std::wstring> splitWResult;
    std::vector<std::string> splitResult;
    std::vector<std::string> splitR;

    splitWResult.push_back(L"");
    splitW = split(L"", L';');
    TEST_ASSERT(splitWNULL, splitW, splitWResult);
    splitWResult.clear();

    splitWResult.push_back(L"");
    splitWResult.push_back(L"");
    splitW = split(L";", L';');
    TEST_ASSERT(splitWDoubleNULL, splitW, splitWResult);
    splitWResult.clear();

    splitWResult.push_back(L"");
    splitWResult.push_back(L"");
    splitWResult.push_back(L"");
    splitWResult.push_back(L"");
    splitW = split(L";;;", L';');
    TEST_ASSERT(splitWQuadNULL, splitW, splitWResult);
    splitWResult.clear();

    splitWResult.push_back(L"Test");
    splitW = split(L"Test", L';');
    TEST_ASSERT(splitWNoSplit, splitW, splitWResult);
    splitWResult.clear();

    splitWResult.push_back(L"Test");
    splitWResult.push_back(L"");
    splitW = split(L"Test;", L';');
    TEST_ASSERT(splitWSplitNull, splitW, splitWResult);
    splitWResult.clear();

    splitWResult.push_back(L"1");
    splitWResult.push_back(L"2");
    splitWResult.push_back(L"3");
    splitWResult.push_back(L"4");
    splitW = split(L"1;2;3;4", L';');
    TEST_ASSERT(splitWSplitNull, splitW, splitWResult);
    splitWResult.clear();


    splitResult.push_back("");
    splitR = split("", ';');
    TEST_ASSERT(splitNULL, splitR, splitResult);
    splitResult.clear();

    splitResult.push_back("");
    splitResult.push_back("");
    splitR = split(";", ';');
    TEST_ASSERT(splitDoubleNULL, splitR, splitResult);
    splitResult.clear();

    splitResult.push_back("");
    splitResult.push_back("");
    splitResult.push_back("");
    splitResult.push_back("");
    splitR = split(";;;", ';');
    TEST_ASSERT(splitQuadNULL, splitR, splitResult);
    splitResult.clear();

    splitResult.push_back("Test");
    splitR = split("Test", ';');
    TEST_ASSERT(splitNoSplit, splitR, splitResult);
    splitResult.clear();

    splitResult.push_back("Test");
    splitResult.push_back("");
    splitR = split("Test;", ';');
    TEST_ASSERT(splitSplitNull, splitR, splitResult);
    splitResult.clear();

    splitResult.push_back("1");
    splitResult.push_back("2");
    splitResult.push_back("3");
    splitResult.push_back("4");
    splitR = split("1;2;3;4", ';');
    TEST_ASSERT(splitSplitNull, splitR, splitResult);
    splitResult.clear();

    wstr = trim(L"");
    TEST_ASSERT(trimWNull, wstr, L"");

    wstr = trim(L"   ");
    TEST_ASSERT(trimWEmpty, wstr, L"");

    wstr = trim(L"Test string");
    TEST_ASSERT(trimWNoTrim, wstr, L"Test string");

    wstr = trim(L"    Test string");
    TEST_ASSERT(trimWLTrim, wstr, L"Test string");

    wstr = trim(L"Test string   ");
    TEST_ASSERT(trimWRTrim, wstr, L"Test string");

    wstr = trim(L"   Test string   ");
    TEST_ASSERT(trimWRLTrim, wstr, L"Test string");

    str = trim("");
    TEST_ASSERT(trimNull, str, "");

    str = trim("   ");
    TEST_ASSERT(trimEmpty, str, "");

    str = trim("Test string");
    TEST_ASSERT(trimNoTrim, str, "Test string");

    str = trim("    Test string");
    TEST_ASSERT(trimLTrim, str, "Test string");

    str = trim("Test string   ");
    TEST_ASSERT(trimRTrim, str, "Test string");

    str = trim("   Test string   ");
    TEST_ASSERT(trimRLTrim, str, "Test string");


    wstr = suffix(L"");
    TEST_ASSERT(suffixWNull, wstr, L"");

    wstr = suffix(L".");
    TEST_ASSERT(suffixWDotNull, wstr, L"");

    wstr = suffix(L"..");
    TEST_ASSERT(suffixWDoubleDotNull, wstr, L".");

    wstr = suffix(L"/test/path/.");
    TEST_ASSERT(suffixWPathDot, wstr, L"");

    wstr = suffix(L"/test/path/..");
    TEST_ASSERT(suffixWPathDoubleDot, wstr, L".");

    wstr = suffix(L"test");
    TEST_ASSERT(suffixWNoSuffix, wstr, L"");

    wstr = suffix(L"test.zip");
    TEST_ASSERT(suffixWSingleSuffix, wstr, L"zip");

    wstr = suffix(L"test.tar.gz");
    TEST_ASSERT(suffixWDoubleSuffix, wstr, L"tar.gz");

    wstr = suffix(L"/test.path/test");
    TEST_ASSERT(suffixWPathNoSuffix, wstr, L"");

    wstr = suffix(L"/test.path/test.zip");
    TEST_ASSERT(suffixWPathSingleSuffix, wstr, L"zip");

    wstr = suffix(L"/tets.path/test.tar.gz");
    TEST_ASSERT(suffixWPathDoubleSuffix, wstr, L"tar.gz");

    str = suffix("");
    TEST_ASSERT(suffixNull, str, "");

    str = suffix(".");
    TEST_ASSERT(suffixDotNull, str, "");

    str = suffix("..");
    TEST_ASSERT(suffixDoubleDotNull, str, ".");

    str = suffix("/test/path/.");
    TEST_ASSERT(suffixPathDot, str, "");

    str = suffix("/test/path/..");
    TEST_ASSERT(suffixPathDoubleDot, str, ".");

    str = suffix("test");
    TEST_ASSERT(suffixNoSuffix, str, "");

    str = suffix("test.zip");
    TEST_ASSERT(suffixSingleSuffix, str, "zip");

    str = suffix("test.tar.gz");
    TEST_ASSERT(suffixDoubleSuffix, str, "tar.gz");

    str = suffix("/test.path/test");
    TEST_ASSERT(suffixPathNoSuffix, str, "");

    str = suffix("/test.path/test.zip");
    TEST_ASSERT(suffixPathSingleSuffix, str, "zip");

    str = suffix("/tets.path/test.tar.gz");
    TEST_ASSERT(suffixPathDoubleSuffix, str, "tar.gz");


    wstr = basename(L"");
    TEST_ASSERT(basenameWNull, wstr, L"");

    wstr = basename(L".");
    TEST_ASSERT(basenameWDotNull, wstr, L"");

    wstr = basename(L".suffix");
    TEST_ASSERT(basenameWSuffixNull, wstr, L"");

    wstr = basename(L"test file");
    TEST_ASSERT(basenameWBasenameNoSuffix, wstr, L"test file");

    wstr = basename(L"test file.");
    TEST_ASSERT(basenameWBasenameNullSuffix, wstr, L"test file");

    wstr = basename(L"test file.suffix");
    TEST_ASSERT(basenameWBasenameSuffix, wstr, L"test file");

    wstr = basename(L"test file.double.suffix");
    TEST_ASSERT(basenameWBasenameDoubleSuffix, wstr, L"test file");

    wstr = basename(L"/test/path/");
    TEST_ASSERT(basenameWPathNull, wstr, L"");

    wstr = basename(L"/test/path/.suffix");
    TEST_ASSERT(basenameWPathSuffixNull, wstr, L"");

    wstr = basename(L"/test/path/test file");
    TEST_ASSERT(basenameWPathBasenameNoSuffix, wstr, L"test file");

    wstr = basename(L"/test/path/test file.");
    TEST_ASSERT(basenameWPathBasenameNullSuffix, wstr, L"test file");

    wstr = basename(L"/test/path/test file.suffix");
    TEST_ASSERT(basenameWPathBasenameSuffix, wstr, L"test file");

    wstr = basename(L"/test/path/test file.double.suffix");
    TEST_ASSERT(basenameWPathBasenameDoubleSuffix, wstr, L"test file");


    str = basename("");
    TEST_ASSERT(basenameNull, str, "");

    str = basename(".");
    TEST_ASSERT(basenameDotNull, str, "");

    str = basename(".suffix");
    TEST_ASSERT(basenameSuffixNull, str, "");

    str = basename("test file");
    TEST_ASSERT(basenameBasenameNoSuffix, str, "test file");

    str = basename("test file.");
    TEST_ASSERT(basenameBasenameNullSuffix, str, "test file");

    str = basename("test file.suffix");
    TEST_ASSERT(basenameBasenameSuffix, str, "test file");

    str = basename("test file.double.suffix");
    TEST_ASSERT(basenameBasenameDoubleSuffix, str, "test file");

    str = basename("/test/path/");
    TEST_ASSERT(basenamePathNull, str, "");

    str = basename("/test/path/.suffix");
    TEST_ASSERT(basenamePathSuffixNull, str, "");

    str = basename("/test/path/test file");
    TEST_ASSERT(basenamePathBasenameNoSuffix, str, "test file");

    str = basename("/test/path/test file.");
    TEST_ASSERT(basenamePathBasenameNullSuffix, str, "test file");

    str = basename("/test/path/test file.suffix");
    TEST_ASSERT(basenamePathBasenameSuffix, str, "test file");

    str = basename("/test/path/test file.double.suffix");
    TEST_ASSERT(basenamePathBasenameDoubleSuffix, str, "test file");


    wstr = directory(L"");
    TEST_ASSERT(directoryWNull, wstr, L"");

    wstr = directory(L"/");
    TEST_ASSERT(directoryWRoot, wstr, L"/");

    wstr = directory(L"/test.suffix");
    TEST_ASSERT(directoryWRootFile, wstr, L"/");

    wstr = directory(L"test");
    TEST_ASSERT(directoryWNameNull, wstr, L"");

    wstr = directory(L"test/path");
    TEST_ASSERT(directoryWRelNamePath, wstr, L"test/");

    wstr = directory(L"test/path/");
    TEST_ASSERT(directoryWRelNamePathSlash, wstr, L"test/path/");

    wstr = directory(L"/test/path");
    TEST_ASSERT(directoryWNamePath, wstr, L"/test/");

    wstr = directory(L"/test/path/");
    TEST_ASSERT(directoryWNamePathSlash, wstr, L"/test/path/");

    wstr = directory(L"test/path/testFile.suffix");
    TEST_ASSERT(directoryWRelRealName, wstr, L"test/path/");

    wstr = directory(L"/test/path/testFile.suffix");
    TEST_ASSERT(directoryWRealName, wstr, L"/test/path/");


    str = directory("");
    TEST_ASSERT(directoryNull, str, "");

    str = directory("/");
    TEST_ASSERT(directoryRoot, str, "/");

    str = directory("/test.suffix");
    TEST_ASSERT(directoryRootFile, str, "/");

    str = directory("test");
    TEST_ASSERT(directoryNameNull, str, "");

    str = directory("test/path");
    TEST_ASSERT(directoryRelNamePath, str, "test/");

    str = directory("test/path/");
    TEST_ASSERT(directoryRelNamePathSlash, str, "test/path/");

    str = directory("/test/path");
    TEST_ASSERT(directoryNamePath, str, "/test/");

    str = directory("/test/path/");
    TEST_ASSERT(directoryNamePathSlash, str, "/test/path/");

    str = directory("test/path/testFile.suffix");
    TEST_ASSERT(directoryRelRealName, str, "test/path/");

    str = directory("/test/path/testFile.suffix");
    TEST_ASSERT(directoryRealName, str, "/test/path/");

    return 0;
}
