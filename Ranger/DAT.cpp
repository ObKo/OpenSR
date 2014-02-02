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

#include "OpenSR/libRanger.h"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace Rangers
{
namespace
{
std::wstring readWideString(std::istream &stream)
{
    //TODO: Optimize?
    wchar_t c = 0;
    std::wstring result;

    while (stream.good())
    {
        stream.read((char*)&c, 2);
        if (c == 0)
            return result;
        result.push_back(c);
    }
}

}

/*!
 * Read recursively key-value tree from *.dat file.
 * \param stream input stream (uncompressed DAT!)
 * \param node current node
 */
void readDATTree(std::istream &stream, DATRecord& node)
{
    uint8_t isTree;
    uint32_t count;

    stream.read((char*)&isTree, 1);
    stream.read((char*)&count, 4);

    node.type = DATRecord::NODE;

    if (isTree)
    {
        DATRecord* currentNode = &node;
        uint32_t currentCount = 0;
        for (int i = 0; i < count; i++)
        {
            uint32_t index, arrayCount;
            uint8_t isText;

            DATRecord r;
            r.type = DATRecord::VALUE;

            stream.read((char*)&index, 4);
            stream.read((char*)&arrayCount, 4);
            stream.read((char*)&isText, 1);
            r.name = readWideString(stream);

            if (isText == 2)
                readDATTree(stream, r);
            else
                r.value = readWideString(stream);

            if ((arrayCount > 1) && (index == 0))
            {
                DATRecord array;
                array.type = DATRecord::ARRAY;
                array.name = r.name;

                r.name = std::wstring();

                currentCount = arrayCount;
                currentNode->children.push_back(array);
                currentNode = &currentNode->children.back();

                currentNode->children.push_back(r);
            }
            else if ((arrayCount == 0) && (index < currentCount - 1))
            {
                currentNode->children.push_back(r);
                r.name = std::wstring();
            }
            else if ((arrayCount == 0) && (index == currentCount - 1))
            {
                currentCount = 0;
                currentNode->children.push_back(r);
                currentNode = &node;
            }
            else
            {
                currentNode->children.push_back(r);
            }
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            DATRecord r;
            r.type = DATRecord::VALUE;
            uint8_t isText;

            stream.read((char*)&isText, 1);
            r.name = readWideString(stream);
            r.value = readWideString(stream);

            node.children.push_back(r);
        }
    }
    std::sort(node.children.begin(), node.children.end(), [&](const DATRecord & a1, const DATRecord & a2)
    {
        return a1.name < a2.name;
    });
}

/*!
 * Load whole *.dat file.
 * \param stream - input stream (uncompressed DAT!)
 * \return root node
 */
DATRecord loadDAT(std::istream &stream)
{
    DATRecord root;
    std::streampos start = stream.tellg();
    uint32_t sig;
    stream.read((char*)&sig, 4);

    if (sig == 0x31304c5a)
    {
        stream.seekg(0, std::ios_base::end);
        uint32_t size = stream.tellg() - start;
        uint8_t *data = new uint8_t[size];
        stream.seekg(start, std::ios_base::beg);
        stream.read((char*)data, size);
        size_t resultSize = 0;
        uint8_t *datData = unpackZL01(data, size, resultSize);
        delete[] data;
        boost::iostreams::stream<boost::iostreams::array_source> datStream(boost::iostreams::array_source((const char*)datData, resultSize));
        readDATTree(datStream, root);
    }
    else
    {
        stream.seekg(start, std::ios_base::beg);
        readDATTree(stream, root);
    }
    return root;
}

}
