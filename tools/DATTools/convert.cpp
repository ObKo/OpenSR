/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include <stdint.h>
#include <iostream>
#include <fstream>

#include <boost/crc.hpp>

#include <OpenSR/libRanger.h>
#include "crypt.h"

uint32_t crc32(uint8_t *data, uint32_t size)
{
    boost::crc_32_type crc;
    crc.process_bytes(data, size);
    return crc.checksum();
}

void printHelp()
{
    std::cout << "OpenSR DAT File Converter" << std::endl;
    std::cout << "Usage: opensr-dat-convert <command> <in_file> <out_file>" << std::endl;
    std::cout << "<command>:" << std::endl;
    std::cout << "  d - decrypt file" << std::endl;
    std::cout << "  x - decompress file" << std::endl;
    std::cout << "  dx - decrypt & decompress file" << std::endl;
    std::cout << "  e - encrypt file" << std::endl;
    std::cout << "  c - compress file" << std::endl;
    std::cout << "  ce - compress & encrypt file" << std::endl;
    std::cout << "  h<command> - assume that input file is CacheData.dat" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printHelp();
        return 0;
    }

    std::string  cmd = argv[1];

    bool isCache = cmd[0] == 'h';
    if (isCache)
        cmd.erase(0, 1);

    if (cmd != "d" && cmd != "x" && cmd != "dx" &&
            cmd != "c" && cmd != "e" && cmd != "ce")
    {
        std::cerr << "Invalid command: " << cmd << std::endl;
        printHelp();
        return -1;
    }
    std::ifstream inf(argv[2], std::ios_base::in | std::ios_base::binary);
    if (!inf)
    {
        std::cerr << "Cannot open input file" << std::endl;
        return -1;
    }
    std::ofstream outf(argv[3], std::ios_base::out | std::ios_base::binary);
    if (!outf)
    {
        std::cerr << "Cannot open output file" << std::endl;
        return -1;
    }
    inf.seekg(0, std::ios_base::end);
    uint32_t size = inf.tellg();
    inf.seekg(0, std::ios_base::beg);

    uint8_t *data = new uint8_t[size];
    inf.read((char*)data, size);

    if (inf.gcount() != size)
    {
        std::cerr << "Cannot read input file" << std::endl;
        return -1;
    }

    if (cmd == "d")
    {
        uint32_t crc = *((uint32_t*)data);
        uint32_t key = *(((uint32_t*)data) + 1);
        Rangers::DAT::decrypt(data + 8, size - 8, key, isCache);
        if (crc != crc32(data + 8, size - 8))
        {
            std::cerr << "CRC Error" << std::endl;
            return -1;
        }
        outf.write((char*)(data + 8), size - 8);
    }
    else if (cmd == "x")
    {
        if (*((uint32_t*)data) != 0x31304c5a)
        {
            std::cerr << "Invalid input file" << std::endl;
            return -1;
        }
        size_t resSize;
        uint8_t *result = (uint8_t*)Rangers::unpackZL01(data, size, resSize);
        if (!result)
        {
            std::cerr << "Cannot unpack data" << std::endl;
            return -1;
        }
        outf.write((char*)result, resSize);
        delete[] result;
    }
    else if (cmd == "dx")
    {
        uint32_t crc = *((uint32_t*)data);
        uint32_t key = *(((uint32_t*)data) + 1);
        Rangers::DAT::decrypt(data + 8, size - 8, key, isCache);
        if (crc != crc32(data + 8, size - 8))
        {
            std::cerr << "CRC Error" << std::endl;
            return -1;
        }
        if (*((uint32_t*)(data + 8)) != 0x31304c5a)
        {
            std::cerr << "Invalid input file" << std::endl;
            return -1;
        }
        size_t resSize;
        uint8_t *result = (uint8_t*)Rangers::unpackZL01(data + 8, size - 8, resSize);
        if (!result)
        {
            std::cerr << "Cannot unpack data" << std::endl;
            return -1;
        }
        outf.write((char*)result, resSize);
        delete[] result;
    }
    else if (cmd == "e")
    {
        uint32_t crc = crc32(data, size);
        uint32_t key = Rangers::DAT::genKey();
        Rangers::DAT::encrypt(data, size, key, isCache);
        outf.write((char*)&crc, 4);
        outf.write((char*)&key, 4);
        outf.write((char*)(data), size);
    }
    else if (cmd == "c")
    {
        size_t resSize;
        uint8_t *result = (uint8_t*)Rangers::packZL01(data, size, resSize);
        if (!result)
        {
            std::cerr << "Cannot pack data" << std::endl;
            return -1;
        }
        outf.write((char*)result, resSize);
        delete[] result;
    }
    else if (cmd == "ce")
    {
        size_t resSize;
        uint8_t *result = (uint8_t*)Rangers::packZL01(data, size, resSize);
        if (!result)
        {
            std::cerr << "Cannot pack data" << std::endl;
            return -1;
        }
        uint32_t crc = crc32(result, resSize);
        uint32_t key = Rangers::DAT::genKey();
        Rangers::DAT::encrypt(result, resSize, key, isCache);
        outf.write((char*)&crc, 4);
        outf.write((char*)&key, 4);
        outf.write((char*)(result), resSize);
        delete[] result;
    }

    delete[] data;
    inf.close();
    outf.close();

    return 0;
}
