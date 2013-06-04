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
#include <zlib.h>
#include <lzma.h>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/positioning.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/shared_ptr.hpp>

namespace
{
const uint32_t RPKG_DECODER_BUFFER_SIZE = 4096;
}

namespace Rangers
{
struct partial_file_source_tag: boost::iostreams::device_tag, boost::iostreams::input_seekable, boost::iostreams::multichar_tag {};
struct seekable_input_filter_tag: boost::iostreams::filter_tag, boost::iostreams::input_seekable, boost::iostreams::multichar_tag {};

class PartialFileSource
{
public:
    typedef partial_file_source_tag category;
    typedef char char_type;

    PartialFileSource(boost::shared_ptr<std::istream> stream, std::streampos pos, std::streamsize size):
        m_offset(pos), m_size(size), m_pos(0), m_stream(stream)
    {
        m_stream->seekg(pos);
    }

    std::streamsize read(char* s, std::streamsize n)
    {
        if (m_stream->fail())
            return -1;

        if (m_pos >= (m_offset + m_size))
            return -1;

        std::streamsize blockSize = n;

        if ((m_pos + blockSize) > m_size)
            blockSize = m_size - m_pos;

        m_pos += blockSize;
        m_stream->read(s, blockSize);
        return m_stream->gcount();
    }

    std::streampos seek(boost::iostreams::stream_offset off, std::ios_base::seekdir way)
    {
        std::streampos newpos;

        if (way == std::ios_base::beg)
        {
            newpos = off;
        }
        else if (way == std::ios_base::cur)
        {
            newpos = m_pos + off;
        }
        else if (way == std::ios_base::end)
        {
            newpos = m_size - off;
        }

        if (newpos > m_size)
            newpos = m_size;

        if (newpos < 0)
            newpos = 0;

        m_stream->seekg(m_offset + newpos);
        m_pos = newpos;

        return newpos;
    }

private:
    boost::shared_ptr<std::istream> m_stream;
    std::streampos m_offset;
    std::streamsize m_size;
    std::streampos m_pos;
};

class RPKGFilter
{
public:
    typedef seekable_input_filter_tag category;
    typedef char char_type;

    RPKGFilter(): m_size(0), m_chunkPosition(0), m_currentChunkOffset(0),
        m_chunkSize(0), m_pos(0), m_inputPos(0), m_currentChunkSize(0),
        m_ready(false), m_invalidStream(false), m_internalStream(0),
        m_decoderBuffer(0), m_bufferAvailable(0), m_bufferPos(0)
    {

    }

    ~RPKGFilter()
    {
        switch (m_compression)
        {
        case Rangers::RPKG_SEEKABLE_LZMA:
            lzma_end((lzma_stream *)m_internalStream);
            delete(lzma_stream *)m_internalStream;
            break;

        case Rangers::RPKG_SEEKABLE_ZLIB:
            inflateEnd((z_stream *)m_internalStream);
            delete(z_stream *)m_internalStream;
            break;
        }

        free(m_decoderBuffer);
    }

    template<typename Source>
    std::streamsize read(Source &src, char* s, std::streamsize n)
    {
        if (m_invalidStream)
            return -1;

        if (!m_ready)
        {
            initStream(src);
            if (m_invalidStream)
                return -1;
        }

        int readed;

        switch (m_compression)
        {
        case Rangers::RPKG_SEEKABLE_LZMA:
            return readLZMA(src, s, n);
            break;
        case Rangers::RPKG_SEEKABLE_ZLIB:
            return readZLIB(src, s, n);
            break;
        case Rangers::RPKG_NONE:
            readed = boost::iostreams::read(src, s, n);
            if (readed > 0)
                m_pos += readed;
            return readed;
            break;
        }

        return -1;
    }

    template<typename Source>
    std::streampos seek(Source &src, boost::iostreams::stream_offset off, std::ios_base::seekdir way)
    {
        if (m_invalidStream)
            return m_pos;

        if (!m_ready)
            initStream(src);

        int64_t seekDelta = 0;

        if (way == std::ios_base::cur)
            seekDelta = off;
        else if (way == std::ios_base::beg)
            seekDelta = off - m_pos;
        else if (way == std::ios_base::end)
            seekDelta = m_size - off - m_pos;
        else
            return m_pos;

        if (seekDelta + m_pos < 0)
            seekDelta = -m_pos;

        if (seekDelta + m_pos > m_size)
            seekDelta = m_size - m_pos;

        if (seekDelta == 0)
            return m_pos;

        if (m_compression == Rangers::RPKG_NONE)
        {
            uint32_t pos = boost::iostreams::seek(src, seekDelta, std::ios_base::cur);
            m_pos = pos - 16;
            return m_pos;
        }

        bool seekFromChunkStart = false;

        if ((seekDelta < 0) && (-seekDelta > (m_pos % m_chunkSize)))
        {
            boost::iostreams::seek(src, 16, std::ios_base::beg);
            m_inputPos = 16;
            m_currentChunkOffset = 16;
            if (!readChunkHeader(src))
            {
                m_invalidStream = true;
                return m_pos;
            }
            seekDelta = m_pos + seekDelta;
            m_pos = 0;
            seekFromChunkStart = true;
        }
        else if (seekDelta < 0)
        {
            boost::iostreams::seek(src, m_currentChunkOffset, std::ios_base::beg);
            m_inputPos = m_currentChunkOffset;
            if (!readChunkHeader(src))
            {
                m_invalidStream = true;
                return m_pos;
            }
            seekDelta += (m_pos % m_chunkSize);
            m_pos -= m_pos % m_chunkSize;
            seekFromChunkStart = true;
        }

        if (m_pos % m_chunkSize + seekDelta >= m_chunkSize)
        {
            seekFromChunkStart = true;
            seekDelta += (m_pos % m_chunkSize);
            m_pos -= m_pos % m_chunkSize;
            int chunkSeekCount = seekDelta / m_chunkSize;
            for (int i = 0; i < chunkSeekCount; i++)
            {
                boost::iostreams::seek(src, m_currentChunkOffset + m_currentChunkSize + 8, std::ios_base::beg);
                m_inputPos = m_currentChunkOffset + m_currentChunkSize + 8;
                m_currentChunkOffset = m_currentChunkOffset + m_currentChunkSize + 8;
                if (!readChunkHeader(src))
                {
                    m_invalidStream = true;
                    return m_pos;
                }
                m_pos += m_chunkSize;
                seekDelta -= m_chunkSize;
            }
        }

        if (seekFromChunkStart)
        {
            m_bufferAvailable = 0;
            m_bufferPos = 0;
        }

        uint32_t seeked = 0;

        switch (m_compression)
        {
        case Rangers::RPKG_SEEKABLE_LZMA:
            seeked = seekLZMA(src, seekDelta, seekFromChunkStart);
            break;
        case Rangers::RPKG_SEEKABLE_ZLIB:
            seeked = seekZLIB(src, seekDelta, seekFromChunkStart);
            break;
        }
        return m_pos;
    }

private:
    uint32_t m_size;
    uint32_t m_chunkSize;
    uint32_t m_pos;
    uint32_t m_chunkPosition;
    uint32_t m_inputPos;
    uint32_t m_currentChunkOffset;
    uint32_t m_currentChunkSize;
    bool m_ready;
    bool m_invalidStream;
    Rangers::RPKGCompression m_compression;
    void *m_internalStream;

    char *m_decoderBuffer;
    uint32_t m_bufferAvailable;
    uint32_t m_bufferPos;

    template<typename Source>
    bool readChunkHeader(Source &src)
    {
        uint32_t compressionSignature = 0;
        switch (m_compression)
        {
        case Rangers::RPKG_SEEKABLE_LZMA:
            compressionSignature = *((const uint32_t*)"SXZC");
            break;

        case Rangers::RPKG_SEEKABLE_ZLIB:
            compressionSignature = *((const uint32_t*)"SZLC");
            break;
        }

        uint32_t sig;
        std::streamsize s;
        s = boost::iostreams::read(src, (char*)&sig, 4);
        if ((s != 4) || (sig != compressionSignature))
        {
            m_invalidStream = true;
            return false;
        }
        s = boost::iostreams::read(src, (char*)&m_currentChunkSize, 4);
        m_inputPos += 8;
        return true;
    }

    template<typename Source>
    void initStream(Source &s)
    {
        uint32_t type;
        boost::iostreams::read(s, (char*)&type, 4);
        if (type == *((const uint32_t*)"RSZL"))
        {
            m_compression = Rangers::RPKG_SEEKABLE_ZLIB;
        }
        else if (type == *((const uint32_t*)"RSXZ"))
        {
            m_compression = Rangers::RPKG_SEEKABLE_LZMA;
            m_internalStream = new lzma_stream;
            //FIXME: F***ing MSVC cannot into C99
			//*((lzma_stream *)m_internalStream) = LZMA_STREAM_INIT;
			memset(m_internalStream, 0, sizeof(lzma_stream));
            lzma_auto_decoder(((lzma_stream *)m_internalStream), 1 << 24, LZMA_CHECK_CRC32);
        }
        else if (type == *((const uint32_t*)"NONE"))
        {
            m_compression = Rangers::RPKG_NONE;
        }
        else
        {
            m_invalidStream = true;
            return;
        }
        uint32_t whocares;
        boost::iostreams::read(s, (char*)&m_size, 4);
        boost::iostreams::read(s, (char*)&whocares, 4);
        boost::iostreams::read(s, (char*)&m_chunkSize, 4);

        m_pos = 0;
        m_chunkPosition = 0;
        m_inputPos = 16;
        m_currentChunkOffset = 16;

        if (m_compression != Rangers::RPKG_NONE)
            m_decoderBuffer = (char*)malloc(RPKG_DECODER_BUFFER_SIZE);

        if (m_compression == Rangers::RPKG_NONE)
        {
            m_ready = true;
            return;
        }

        if (!readChunkHeader(s))
            return;

        if (m_compression == Rangers::RPKG_SEEKABLE_ZLIB)
        {
            m_internalStream = new z_stream;
            if (!prepareBuffer(s))
                return;

            z_stream *zlibStream = ((z_stream *)m_internalStream);
            zlibStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
            zlibStream->avail_in = m_bufferAvailable;
            zlibStream->next_out = 0;
            zlibStream->avail_out = 0;
            inflateInit(zlibStream);
        }

        m_ready = true;
    }

    template<typename Source>
    std::streamsize readZLIB(Source &src, char* s, std::streamsize n)
    {
        uint32_t readed = 0;
        while (readed < n)
        {
            if (!prepareBuffer(src) || m_bufferAvailable == 0)
            {
                m_pos += readed;
                if (readed)
                    return readed;
                else
                    return -1;
            }
            z_stream *zlibStream = ((z_stream *)m_internalStream);
            zlibStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
            zlibStream->avail_in = m_bufferAvailable;
            zlibStream->next_out = (unsigned char*)(s + readed);
            zlibStream->avail_out = n - readed;

            inflate(zlibStream, Z_NO_FLUSH);

            m_bufferPos += m_bufferAvailable - zlibStream->avail_in;
            m_bufferAvailable = zlibStream->avail_in;
            readed += n - readed - zlibStream->avail_out;

            if ((zlibStream->avail_in == 0) && (m_inputPos >= m_currentChunkOffset + m_currentChunkSize + 8))
            {
                inflateEnd(zlibStream);
                if (!prepareBuffer(src) || m_bufferAvailable == 0)
                {
                    m_pos += readed;
                    if (readed)
                        return readed;
                    else
                        return -1;
                }
                zlibStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
                zlibStream->avail_in = m_bufferAvailable;
                zlibStream->next_out = 0;
                zlibStream->avail_out = 0;
                inflateInit(zlibStream);
            }
        }
        m_pos += readed;
        return readed;
    }

    template<typename Source>
    uint32_t seekZLIB(Source &src, uint32_t offset, bool fromStart)
    {
        z_stream *zlibStream = ((z_stream *)m_internalStream);
        if (fromStart)
        {
            inflateEnd(zlibStream);
            if (!prepareBuffer(src) || m_bufferAvailable == 0)
                return 0;
            zlibStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
            zlibStream->avail_in = m_bufferAvailable;
            zlibStream->next_out = 0;
            zlibStream->avail_out = 0;
            inflateInit(zlibStream);
        }
        uint32_t seeked = 0;
        unsigned char *seekBuffer = (unsigned char *)malloc(RPKG_DECODER_BUFFER_SIZE);
        while (seeked < offset)
        {
            if (!prepareBuffer(src) || m_bufferAvailable == 0)
            {
                m_pos += seeked;
                return seeked;
            }
            uint32_t outSize = RPKG_DECODER_BUFFER_SIZE > (offset - seeked) ? (offset - seeked) : RPKG_DECODER_BUFFER_SIZE;
            zlibStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
            zlibStream->avail_in = m_bufferAvailable;
            zlibStream->next_out = seekBuffer;
            zlibStream->avail_out = outSize;

            inflate(zlibStream, Z_NO_FLUSH);

            m_bufferPos += m_bufferAvailable - zlibStream->avail_in;
            m_bufferAvailable = zlibStream->avail_in;
            seeked += outSize - zlibStream->avail_out;
        }
        m_pos += seeked;
        free(seekBuffer);
        return seeked;
    }

    template<typename Source>
    std::streamsize readLZMA(Source &src, char* s, std::streamsize n)
    {
        uint32_t readed = 0;
        while (readed < n)
        {
            if (!prepareBuffer(src) || m_bufferAvailable == 0)
            {
                m_pos += readed;
                if (readed)
                    return readed;
                else
                    return -1;
            }
            lzma_stream *lzmaStream = ((lzma_stream *)m_internalStream);
            lzmaStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
            lzmaStream->avail_in = m_bufferAvailable;
            lzmaStream->next_out = (unsigned char*)(s + readed);
            lzmaStream->avail_out = n - readed;

            lzma_code(lzmaStream, LZMA_RUN);

            m_bufferPos += m_bufferAvailable - lzmaStream->avail_in;
            m_bufferAvailable = lzmaStream->avail_in;
            readed += n - readed - lzmaStream->avail_out;

            if ((lzmaStream->avail_in == 0) && (m_inputPos >= m_currentChunkOffset + m_currentChunkSize + 8))
            {
                lzma_end(lzmaStream);
                //FIXME: F***ing MSVC cannot into C99
			    //*((lzma_stream *)m_internalStream) = LZMA_STREAM_INIT;
			    memset(m_internalStream, 0, sizeof(lzma_stream));
                lzma_auto_decoder(lzmaStream, 1 << 24, LZMA_CHECK_CRC32);
            }
        }
        m_pos += readed;
        return readed;
    }

    template<typename Source>
    uint32_t seekLZMA(Source &src, uint32_t offset, bool fromStart)
    {
        if (fromStart)
        {
            lzma_stream *lzmaStream = ((lzma_stream *)m_internalStream);
            lzma_end(lzmaStream);
            //FIXME: F***ing MSVC cannot into C99
			//*((lzma_stream *)m_internalStream) = LZMA_STREAM_INIT;
			memset(lzmaStream, 0, sizeof(lzma_stream));
            lzma_auto_decoder(lzmaStream, 1 << 24, LZMA_CHECK_CRC32);
        }
        uint32_t seeked = 0;
        unsigned char *seekBuffer = (unsigned char *)malloc(RPKG_DECODER_BUFFER_SIZE);
        while (seeked < offset)
        {
            if (!prepareBuffer(src) || m_bufferAvailable == 0)
            {
                m_pos += seeked;
                return seeked;
            }
            uint32_t outSize = RPKG_DECODER_BUFFER_SIZE > (offset - seeked) ? (offset - seeked) : RPKG_DECODER_BUFFER_SIZE;
            lzma_stream *lzmaStream = ((lzma_stream *)m_internalStream);
            lzmaStream->next_in = (unsigned char*)(m_decoderBuffer + m_bufferPos);
            lzmaStream->avail_in = m_bufferAvailable;
            lzmaStream->next_out = seekBuffer;
            lzmaStream->avail_out = outSize;

            lzma_code(lzmaStream, LZMA_RUN);

            m_bufferPos += m_bufferAvailable - lzmaStream->avail_in;
            m_bufferAvailable = lzmaStream->avail_in;
            seeked += outSize - lzmaStream->avail_out;
        }
        m_pos += seeked;
        free(seekBuffer);
        return seeked;
    }

    template<typename Source>
    bool prepareBuffer(Source &src)
    {
        if ((m_bufferAvailable == 0))
        {
            if (m_inputPos >= m_currentChunkOffset + m_currentChunkSize + 8)
            {
                if (m_chunkPosition >= (m_size - 1) / m_chunkSize)
                    return true;

                m_currentChunkOffset = m_currentChunkOffset + m_currentChunkSize + 8;
                m_chunkPosition += 1;
                if (!readChunkHeader(src))
                {
                    m_invalidStream = true;
                    return false;
                }
            }
            uint32_t bytesToRead = (m_currentChunkOffset + m_currentChunkSize + 8 - m_inputPos);
            bytesToRead = bytesToRead > RPKG_DECODER_BUFFER_SIZE ? RPKG_DECODER_BUFFER_SIZE : bytesToRead;
            std::streamsize ns = boost::iostreams::read(src, m_decoderBuffer, bytesToRead);
            if (ns != bytesToRead)
            {
                m_invalidStream = true;
                return false;
            }
            m_inputPos += bytesToRead;
            m_bufferAvailable = bytesToRead;
            m_bufferPos = 0;
        }
        return true;
    }
};

std::istream* getRPKGFileStream(const RPKGEntry& e, boost::shared_ptr<std::istream> stream)
{
    boost::iostreams::filtering_istream *in = new boost::iostreams::filtering_istream();
    in->push(RPKGFilter());
    in->push(PartialFileSource(stream, e.offset, e.size + 16));
    return in;
}

}