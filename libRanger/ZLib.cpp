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
#include <zlib.h>
#include <cstdlib>

using namespace Rangers;

//TODO: Normal error handling

/*!
 * \param src ZL01 - compressed data
 * \param srclen input data size
 * \param destlen output data size
 * \return unpacked data
 */
unsigned char *Rangers::unpackZL01(unsigned char * src, size_t srclen, size_t& destlen)
{
    uint32_t sig = ((uint32_t *)src)[0];

    if (sig != 0x31304c5a)
    {
        destlen == 0;
        return 0;
    }

    uLongf outsize = ((uint32_t *)src)[1];

    unsigned char *inbuffer = src + 8;
    unsigned char *outbuffer = new unsigned char[outsize];

    if (uncompress(outbuffer, &outsize, inbuffer, srclen - 8))
    {
        destlen = 0;
        return 0;
    }
    else
    {
        destlen = outsize;
        return outbuffer;
    }
}

/*!
 * \param dst Unpacked data buffer
 * \param src ZL02 - compressed data
 * \param srclen input data size
 * \param destlen output data size
 */
void Rangers::unpackZL02(unsigned char * dst, unsigned char * src, size_t srclen, size_t& destlen)
{
    uint32_t sig = ((uint32_t *)src)[0];

    if (sig != 0x32304c5a)
    {
        destlen == 0;
        return;
    }

    uLongf outsize = destlen;

    unsigned char *inbuffer = src + 8;
    unsigned char *outbuffer = dst;

    if (uncompress(outbuffer, &outsize, inbuffer, srclen - 8))
        destlen = 0;
    else
        destlen = outsize;
}

/*!
 * \param src ZL02 - compressed data
 * \param srclen input data size
 * \param destlen output data size
 * \return unpacked data
 */
unsigned char *Rangers::unpackZL02(unsigned char * src, size_t srclen, size_t& destlen)
{
    uint32_t sig = ((uint32_t *)src)[0];

    if (sig != 0x32304c5a)
    {
        destlen == 0;
        return 0;
    }

    uLongf outsize = destlen;

    unsigned char *inbuffer = src + 8;
    unsigned char *outbuffer = new unsigned char[outsize];

    if (uncompress(outbuffer, &outsize, inbuffer, srclen - 8))
    {
        destlen = 0;
        return 0;
    }
    else
    {
        destlen = outsize;
        return outbuffer;
    }
}

/*!
 * \param src input data
 * \param srclen input data size
 * \return RPKG file item
 */
RPKGItem Rangers::packZLIB(const char * src, size_t srclen)
{
    unsigned long dest_size = compressBound(srclen);
    unsigned char *data = new unsigned char[dest_size];

    if (compress2(data, &dest_size, (const Bytef *)src, srclen, 9))
    {
        RPKGItem result;
        result.data = 0;
        result.packSize = 0;
        result.packType = 0x42494c5a;
        result.size = 0;
        delete data;
        return result;
    }

    data = (unsigned char *)realloc(data, dest_size);

    RPKGItem result;
    result.data = data;
    result.packSize = dest_size;
    result.packType = 0x42494c5a;
    result.size = srclen;
    return result;
}

/*!
 * \param item input data
 * \return unpacked data
 */
char *Rangers::unpackZLIB(RPKGItem item)
{
    if (item.packType != 0x42494c5a)
        return 0;

    char *data = new char[item.size];

    unsigned long dest_size = item.size;

    if (uncompress((Bytef *)data, &dest_size, (const Bytef *)item.data, item.packSize))
    {
        delete data;
        return 0;
    }

    return data;
}
