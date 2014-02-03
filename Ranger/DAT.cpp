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
    return std::wstring();
}

DATRecord::iterator binary_find(std::vector<DATRecord> &r, const std::wstring& key)
{
    DATRecord dummy;
    dummy.name = key;
    DATRecord::iterator i = std::lower_bound(r.begin(), r.end(), dummy, [&](const DATRecord & a1, const DATRecord & a2)
    {
        return a1.name < a2.name;
    });

    if (i != r.end() && !(dummy.name < i->name))
        return i;
    else
        return r.end(); // not found
}


DATRecord::const_iterator binary_find(const std::vector<DATRecord> &r, const std::wstring& key)
{
    DATRecord dummy;
    dummy.name = key;
    DATRecord::const_iterator i = std::lower_bound(r.begin(), r.end(), dummy, [&](const DATRecord & a1, const DATRecord & a2)
    {
        return a1.name < a2.name;
    });

    if (i != r.end() && !(dummy.name < i->name))
        return i;
    else
        return r.end(); // not found
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
                DATRecord::iterator i = currentNode->add(array);
                currentNode = i.operator->();

                currentNode->add(r);
            }
            else if ((arrayCount == 0) && (index < currentCount - 1))
            {
                r.name = std::wstring();
                currentNode->add(r);
            }
            else if ((arrayCount == 0) && (index == currentCount - 1))
            {
                currentCount = 0;
                currentNode->add(r);
                currentNode = &node;
            }
            else
            {
                currentNode->add(r);
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

            node.add(r);
        }
    }
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
        delete[] datData;
    }
    else
    {
        stream.seekg(start, std::ios_base::beg);
        readDATTree(stream, root);
    }
    return root;
}

DATRecord::DATRecord(Type type , const std::wstring& name, const std::wstring& value):
    type(type), name(name), value(value)
{

}

std::vector<DATRecord> DATRecord::children() const
{
    return m_children;
}

std::vector<DATRecord>::size_type DATRecord::size() const
{
    return m_children.size();
}

bool DATRecord::empty() const
{
    return m_children.empty();
}

DATRecord::iterator DATRecord::add(const DATRecord& record)
{
    if (type == DATRecord::ARRAY)
    {
        m_children.push_back(record);
        return m_children.end() - 1;
    }
    else if (type == DATRecord::NODE)
    {
        iterator pos = std::lower_bound(
                           m_children.begin(),
                           m_children.end(),
                           record,
                           [&](const DATRecord & a1, const DATRecord & a2)
        {
            return a1.name < a2.name;
        }
                       );
        if (pos == m_children.end())
        {
            m_children.push_back(record);
            pos = m_children.end() - 1;
        }
        else if (pos->name == record.name)
            *pos = record;
        else
            pos = m_children.insert(pos, record);
        return pos;
    }
    return iterator();
}

void DATRecord::erase(int i)
{
    m_children.erase(m_children.begin() + i);
}

void DATRecord::erase(const std::wstring& key)
{
    iterator pos = binary_find(m_children, key);
    m_children.erase(pos);
}

void DATRecord::clear()
{
    m_children.clear();
}

const DATRecord& DATRecord::at(int i) const
{
    return m_children.at(i);
}

const DATRecord& DATRecord::at(const std::wstring& key) const
{
    const_iterator pos = binary_find(m_children, key);
    if (pos == m_children.end())
        throw std::out_of_range("DATRecord::at");
    return *pos;
}

DATRecord& DATRecord::operator[](int i)
{
    return m_children[i];
}

DATRecord& DATRecord::operator[](const std::wstring& key)
{
    DATRecord dummy;
    dummy.name = key;
    iterator pos = binary_find(m_children, key);

    if (pos == m_children.end())
    {
        return *(add(dummy));
    }
    else
        return *pos;
}

DATRecord::iterator DATRecord::begin()
{
    return m_children.begin();
}

DATRecord::const_iterator DATRecord::begin() const
{
    return m_children.begin();
}


DATRecord::iterator DATRecord::end()
{
    return m_children.end();
}

DATRecord::const_iterator DATRecord::end() const
{
    return m_children.end();
}
}
