/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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
#include <lzma.h>
#include <cstdlib>

namespace
{
const uint32_t DEFAULT_LZMA_CHUNK_SIZE = 256 * 1024;
}

//TODO: Normal error handling
namespace Rangers
{

bool packRSXZ(const char * src, size_t srclen, RPKGItem &item)
{
    uint32_t outBufSize = (lzma_stream_buffer_bound(DEFAULT_LZMA_CHUNK_SIZE) + 8) * (srclen / DEFAULT_LZMA_CHUNK_SIZE + 1);
    char* outdata = (char*)malloc(outBufSize);
    uint32_t pos = 0;
    int32_t chunkSize;

    item.packType = *((const uint32_t*)"RSXZ");
    item.size = srclen;
    item.chunkSize = DEFAULT_LZMA_CHUNK_SIZE;

    for (int i = 0; i < ((srclen - 1) / DEFAULT_LZMA_CHUNK_SIZE + 1); i++)
    {
        chunkSize = (srclen - i * DEFAULT_LZMA_CHUNK_SIZE) >= DEFAULT_LZMA_CHUNK_SIZE ? DEFAULT_LZMA_CHUNK_SIZE : (srclen - i * DEFAULT_LZMA_CHUNK_SIZE);

        long unsigned int destSize = outBufSize - pos - 8;

        lzma_stream lzmaStream = LZMA_STREAM_INIT;
        lzmaStream.next_in = (unsigned char *)(src + i * DEFAULT_LZMA_CHUNK_SIZE);
        lzmaStream.avail_in = chunkSize;
        lzmaStream.next_out = (unsigned char*)(outdata + pos + 8);
        lzmaStream.avail_out = destSize;

        lzma_easy_encoder(&lzmaStream, LZMA_PRESET_EXTREME, LZMA_CHECK_CRC32);
        lzma_code(&lzmaStream, LZMA_FINISH);
        lzma_end(&lzmaStream);

        *((uint32_t*)(outdata + pos)) = *((const uint32_t*)"SXZC");
        *((uint32_t*)(outdata + pos + 4)) = lzmaStream.total_out;
        pos += lzmaStream.total_out + 8;
    }

    item.packSize = pos;
    outdata = (char*)realloc(outdata, pos);
    item.data = (unsigned char*)outdata;
    return true;
}

char *unpackRSXZ(RPKGItem item)
{
    if (item.packType != *((uint32_t*)"RSXZ"))
        return 0;

    char *data = new char[item.size];

    uint32_t extracted = 0;
    uint32_t pos = 0;

    while (extracted < item.size)
    {
        unsigned char *p = item.data + pos;
        uint32_t chunkSig = *((uint32_t*)p);
        if (chunkSig != *((uint32_t*)"SXZC"))
        {
            delete[] data;
            return 0;
        }
        uint32_t chunkSize = *((uint32_t*)(p + 4));
        lzma_stream lzmaStream = LZMA_STREAM_INIT;
        lzmaStream.next_in = p + 8;
        lzmaStream.avail_in = chunkSize;
        lzmaStream.next_out = (unsigned char*)(data + extracted);
        lzmaStream.avail_out = item.size - extracted;

        lzma_auto_decoder(&lzmaStream, 1 << 24, LZMA_CHECK_CRC32);
        lzma_code(&lzmaStream, LZMA_FINISH);

        if (lzmaStream.avail_in != 0)
        {
            delete[] data;
            return 0;
        }
        lzma_end(&lzmaStream);

        extracted = item.size - lzmaStream.avail_out;
        pos += chunkSize + 8;
    }

    return data;
}

}
