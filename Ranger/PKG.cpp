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

#include <OpenSR/libRangerQt.h>

namespace OpenSR
{
struct PKGDirHeader
{
    uint32_t zero1;
    uint32_t itemsCount;
    uint32_t zero2;
};

PKGItem *loadItems(QIODevice *dev, PKGItem *previous)
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

    dev->seek(previous->offset);
    dev->read((char*)&dir, sizeof(PKGDirHeader));

    previous->childCount = dir.itemsCount;
    previous->childs = new PKGItem[dir.itemsCount];

    for (int i = 0; i < dir.itemsCount; i++)
    {
        PKGItem item;
        item.parent = previous;
        item.childCount = 0;
        item.childs = 0;

        dev->seek(previous->offset + 12 + 158 * i);

        dev->read((char*)&item.sizeInArc, 4);
        dev->read((char*)&item.size, 4);
        dev->read((char*)item.fullName, 63);
        dev->read((char*)item.name, 63);
        dev->read((char*)&item.dataType, 4);
        dev->read((char*)&zero, 4);
        dev->read((char*)&zero, 4);
        dev->read((char*)&zero, 4);
        dev->read((char*)&item.offset, 4);
        dev->read((char*)&zero, 4);

        previous->childs[i] = item;

        if (item.dataType == 3)
            loadItems(dev, &previous->childs[i]);
    }

    return result;
}

/*!
 * \param item file item
 * \param pkgfile archive file
 * \return loaded data
 */
QByteArray extractFile(const PKGItem &item, QIODevice *dev)
{
    if (item.dataType == 3)
        return QByteArray();

    QByteArray result;

    if (item.dataType == 2)
    {
        uint32_t outsize = item.size;
        uint32_t bufsize;
        uint32_t done = 0;
        dev->seek(item.offset + 4);

        while (done < outsize)
        {
            uint32_t bufsize;
            dev->read((char *)&bufsize, 4);
            QByteArray d = dev->read(bufsize);
            QByteArray r = unpackZL(d);
            result += r;
            done += r.size();
        }

        return result;
    }

    if (item.dataType == 1)
    {
        dev->seek(item.offset + 4);
        return dev->read(item.size);
    }

    return QByteArray();
}

/*!
 * \param stream input stream
 * \return pointer to root item
 */
PKGItem *loadPKG(QIODevice *dev)
{
    uint32_t offset;
    uint32_t zero;
    dev->read((char*)&offset, sizeof(uint32_t));
    dev->seek(offset);

    PKGItem *root = loadItems(dev, 0);

    return root;
}
}
