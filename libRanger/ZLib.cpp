/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

namespace
{
const uint32_t DEFAULT_ZLIB_CHUNK_SIZE = 256 * 1024;
}

namespace Rangers
{
//TODO: Normal error handling

/*!
 * \param src ZL01 - compressed data
 * \param srclen input data size
 * \param destlen output data size
 * \return unpacked data
 */
unsigned char *unpackZL01(const unsigned char * src, size_t srclen, size_t& destlen)
{
    uint32_t sig = ((uint32_t *)src)[0];

    if (sig != 0x31304c5a)
    {
        destlen == 0;
        return 0;
    }

    uLongf outsize = ((uint32_t *)src)[1];

    const unsigned char *inbuffer = src + 8;
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
void unpackZL02(unsigned char * dst, const unsigned char * src, size_t srclen, size_t& destlen)
{
    uint32_t sig = ((uint32_t *)src)[0];

    if (sig != 0x32304c5a)
    {
        destlen == 0;
        return;
    }

    uLongf outsize = destlen;

    const unsigned char *inbuffer = src + 8;
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
unsigned char *unpackZL02(const unsigned char * src, size_t srclen, size_t& destlen)
{
    uint32_t sig = ((uint32_t *)src)[0];

    if (sig != 0x32304c5a)
    {
        destlen == 0;
        return 0;
    }

    uLongf outsize = destlen;

    const unsigned char *inbuffer = src + 8;
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
bool packRSZL(const char * src, size_t srclen, RPKGItem &item)
{
    uint32_t outBufSize = (compressBound(DEFAULT_ZLIB_CHUNK_SIZE) + 8) * (srclen / DEFAULT_ZLIB_CHUNK_SIZE + 1);
    char* outdata = (char*)malloc(outBufSize);
    uint32_t pos = 0;
    int32_t chunkSize;

    item.packType = *((const uint32_t*)"RSZL");
    item.size = srclen;
    item.chunkSize = DEFAULT_ZLIB_CHUNK_SIZE;

    for (int i = 0; i < ((srclen - 1) / DEFAULT_ZLIB_CHUNK_SIZE + 1); i++)
    {
        chunkSize = (srclen - i * DEFAULT_ZLIB_CHUNK_SIZE) >= DEFAULT_ZLIB_CHUNK_SIZE ? DEFAULT_ZLIB_CHUNK_SIZE : (srclen - i * DEFAULT_ZLIB_CHUNK_SIZE);

        long unsigned int destSize = outBufSize - pos - 8;

        z_stream zlibStream;
        zlibStream.next_in = (unsigned char *)(src + i * DEFAULT_ZLIB_CHUNK_SIZE);
        zlibStream.avail_in = chunkSize;
        zlibStream.next_out = (unsigned char*)(outdata + pos + 8);
        zlibStream.avail_out = destSize;
        zlibStream.zalloc = 0;
        zlibStream.zfree = 0;

        deflateInit(&zlibStream, Z_DEFAULT_COMPRESSION);
        deflate(&zlibStream, Z_FINISH);
        deflateEnd(&zlibStream);

        *((uint32_t*)(outdata + pos)) = *((const uint32_t*)"SZLC");
        *((uint32_t*)(outdata + pos + 4)) = zlibStream.total_out;
        pos += zlibStream.total_out + 8;
    }

    item.packSize = pos;
    outdata = (char*)realloc(outdata, pos);
    item.data = (unsigned char*)outdata;
    return true;
}

/*!
 * \param item input data
 * \return unpacked data
 */
char *unpackRSZL(RPKGItem item)
{
    if (item.packType != *((uint32_t*)"RSZL"))
        return 0;

    char *data = new char[item.size];

    uint32_t extracted = 0;
    uint32_t pos = 0;

    while (extracted < item.size)
    {
        unsigned char *p = item.data + pos;
        uint32_t chunkSig = *((uint32_t*)p);
        if (chunkSig != *((uint32_t*)"SZLC"))
        {
            delete[] data;
            return 0;
        }
        uint32_t chunkSize = *((uint32_t*)(p + 4));
        z_stream zlibStream;
        zlibStream.next_in = p + 8;
        zlibStream.avail_in = chunkSize;
        zlibStream.next_out = (unsigned char*)(data + extracted);
        zlibStream.avail_out = item.size - extracted;
        zlibStream.zalloc = 0;
        zlibStream.zfree = 0;

        inflateInit(&zlibStream);
        inflate(&zlibStream, Z_FINISH);

        if (zlibStream.avail_in != 0)
        {
            delete[] data;
            return 0;
        }

        inflateEnd(&zlibStream);
        extracted = item.size - zlibStream.avail_out;
        pos += chunkSize + 8;
    }

    return data;
}
}
