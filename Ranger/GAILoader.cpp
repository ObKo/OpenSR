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

#include <zlib.h>
#include <cstring>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

using namespace Rangers;
using namespace std;

void copyImageData(unsigned char *bufdes, int destwidth, int x, int y, int w, int h, unsigned char *graphbuf)
{
    uint32_t j;

    for (int i = 0; i < w * h; i++)
    {
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4) = *((graphbuf) + i * 4);
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4 + 1) = *((graphbuf) + i * 4 + 1);
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4 + 2) = *((graphbuf) + i * 4 + 2);
        *(bufdes + (destwidth * (i / w + y) + x + i % w) * 4 + 3) = *((graphbuf) + i * 4 + 3);
    }
}

/*!
 * \param stream input stream
 * \param offset offset in file
 * \return GAI animation
 */
GAIAnimation Rangers::loadGAIAnimation(std::istream& stream, GIFrame *background)
{
    GAIAnimation result;

    GAIHeader header;

    stream.read((char *)&header, sizeof(GAIHeader));

    if (header.signature != 0x00696167)
        return result;

    int width = header.finishX - header.startX;
    int height = header.finishY - header.startY;

    result.frames = new GIFrame[header.frameCount];
    result.frameCount = header.frameCount;
    result.width = width;
    result.height = height;
    result.times = loadGAITimes(stream, header);

    for (int i = 0; i < header.frameCount; i++)
    {
        uint32_t giSeek, giSize;
        stream.seekg(sizeof(GAIHeader) + i * 2 * sizeof(uint32_t), ios_base::beg);
        stream.read((char*)&giSeek, sizeof(uint32_t));
        stream.read((char*)&giSize, sizeof(uint32_t));

        if (giSeek && giSize)
        {
            size_t giOffset = giSeek;
            uint32_t signature;
            stream.seekg(giOffset, ios_base::beg);
            stream.read((char*)&signature, sizeof(uint32_t));

            if (signature == 0x31304c5a)
            {
                stream.seekg(giOffset, ios_base::beg);
                size_t outsize;
                char *buffer = new char[giSize];
                stream.read((char *)buffer, giSize);
                unsigned char *gi = unpackZL01((unsigned char *)buffer, giSize, outsize);
                delete[] buffer;
                boost::iostreams::stream<boost::iostreams::array_source> giStream(boost::iostreams::array_source((const char*)gi, outsize));
                result.frames[i] = loadGIFrame(giStream, true, background, 0, header.startX, header.startY, header.finishX, header.finishY);
                delete[] gi;
            }
            else
            {
                stream.seekg(giOffset, ios_base::beg);
                result.frames[i] = loadGIFrame(stream, true, background, giOffset, header.startX, header.startY, header.finishX, header.finishY);
            }
            if (background)
                background = &(result.frames[i]);
        }
        else
        {
            GIFrame frame;
            frame.width = width;
            frame.height = height;
            frame.data = new unsigned char[frame.width * frame.height * 4];
            frame.format = GIFrame::Format_ARGB32;
            memset(frame.data, 0, frame.width * frame.height * 4);

            if (background)
                copyImageData(frame.data, frame.width, 0, 0, background->width, background->height, background->data);

            result.frames[i] = frame;
            background = &(result.frames[i]);
        }
    }

    return result;
}

uint32_t* Rangers::loadGAITimes(std::istream& stream, const GAIHeader& header)
{
    uint32_t *times = new uint32_t[header.frameCount];
    memset(times, 0, header.frameCount * 4);

    if (!header.waitSize)
        return times;

    uint8_t *waitData = new uint8_t[header.waitSize];
    stream.seekg(header.waitSeek, std::ios_base::beg);
    stream.read((char*)waitData, header.waitSize);

    uint32_t timeBlockCount = *((uint32_t*)waitData);
    uint8_t *p = waitData + 4 + timeBlockCount * 8 + 2;

    for (int i = 0; i < timeBlockCount; i++)
    {
        uint32_t blockFrameCount = *((uint32_t *)p);
        for (int j = 0; j < blockFrameCount; j++)
        {
            uint32_t frame = *((uint32_t *)(p + 4 + j * 8));
            uint32_t time = *((uint32_t *)(p + 4 + j * 8 + 4));
            if (frame < header.frameCount)
                times[frame] = time;
        }
        p += blockFrameCount * 8 + 4 + 2;
    }

    delete[] waitData;
    return times;
}

GAIHeader Rangers::loadGAIHeader(std::istream& stream)
{
    GAIHeader header;
    stream.read((char *)&header, 48);
    return header;
}
