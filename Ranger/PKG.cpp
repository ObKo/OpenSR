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

#include "OpenSR/libRanger.h"

#include <iomanip>
#include <cstdlib>

using namespace Rangers;

using namespace std;

struct PKGDirHeader
{
    uint32_t zero1;
    uint32_t itemsCount;
    uint32_t zero2;
};

PKGItem *loadItems(std::istream& stream, PKGItem *previous)
{
    uint32_t zero;
    PKGDirHeader dir;


    PKGItem *result = 0;

    if (!previous)
    {
        previous = new PKGItem;
        result = previous;
        previous->name[0] = 0;
        previous->fullName[0] = 0;
        previous->dataType = 3;
        previous->parent = 0;
        previous->size = 0;
        previous->sizeInArc = 0;
        previous->offset = 4;
    }

    stream.seekg(previous->offset, ios_base::beg);

    stream.read((char*)&dir, sizeof(PKGDirHeader));

    if (stream.eof() || stream.fail())
        return result;

    previous->childCount = dir.itemsCount;

    previous->childs = new PKGItem[dir.itemsCount];

    for (int i = 0; i < dir.itemsCount; i++)
    {
        PKGItem item;
        item.parent = previous;
        item.childCount = 0;
        item.childs = 0;

        stream.seekg(previous->offset + 12 + 158 * i, ios_base::beg);

        stream.read((char*)&item.sizeInArc, 4);
        stream.read((char*)&item.size, 4);
        stream.read((char*)item.fullName, 63);
        stream.read((char*)item.name, 63);
        stream.read((char*)&item.dataType, 4);
        stream.read((char*)&zero, 4);
        stream.read((char*)&zero, 4);
        stream.read((char*)&zero, 4);
        stream.read((char*)&item.offset, 4);
        stream.read((char*)&zero, 4);

        previous->childs[i] = item;

        if (item.dataType == 3)
            loadItems(stream, &previous->childs[i]);
    }

    return result;
}

/*!
 * \param item file item
 * \param pkgfile archive file
 * \return loaded data
 */
unsigned char *Rangers::extractFile(const PKGItem &item, std::istream& pkgfile)
{
    if (item.dataType == 3)
        return 0;

    if (item.dataType == 2)
    {
        size_t outsize = item.size;
        size_t buffer_size;
        size_t done = 0;
        pkgfile.seekg(item.offset + 4, ios_base::beg);

        unsigned char *inputbuffer = 0;
        unsigned char *outbuffer = new unsigned char[outsize];

        while ((done < item.size) && outsize)
        {
            buffer_size = 0;
            pkgfile.read((char *)&buffer_size, 4);
            unsigned char *inp = (unsigned char *)realloc(inputbuffer, buffer_size);
            if (!inp)
            {
                delete inputbuffer;
                return 0;
            }
            else
            {
                inputbuffer = inp;
            }
            pkgfile.read((char *)inputbuffer, buffer_size);
            unpackZL02(outbuffer + done, inputbuffer, buffer_size, outsize);
            done += outsize;
        }

        delete inputbuffer;

        return outbuffer;
    }

    if (item.dataType == 1)
    {
        pkgfile.seekg(item.offset + 4, ios_base::beg);
        unsigned char *outbuffer = new unsigned char[item.size];
        pkgfile.read((char *)outbuffer, item.size);
        return outbuffer;
    }

    return 0;
}

/*!
 * \param stream input stream
 * \return pointer to root item
 */
PKGItem *Rangers::loadPKG(std::istream& stream)
{
    uint32_t offset;
    uint32_t zero;
    stream.read((char*)&offset, sizeof(uint32_t));
    stream.seekg(offset, ios_base::beg);

    PKGItem *root = loadItems(stream, 0);

    return root;
}
