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

#include <jpeglib.h>
#include <cstring>

namespace Rangers
{
namespace
{
struct JpegStream
{
    jpeg_source_mgr mgr;
    std::istream* stream;
    uint8_t buffer[4096];
};

void initSource(j_decompress_ptr cinfo)
{
}

boolean fillBuffer(j_decompress_ptr cinfo)
{
    JpegStream* src = (JpegStream*)(cinfo->src);

    src->stream->read((char*)src->buffer, 4096);

    src->mgr.next_input_byte = src->buffer;
    src->mgr.bytes_in_buffer = src->stream->gcount();

    if (src->mgr.bytes_in_buffer == 0)
    {
        src->buffer[0] = 0xFF;
        src->buffer[1] = JPEG_EOI;
        src->mgr.bytes_in_buffer = 2;
    }

    return TRUE;
}

void skip(j_decompress_ptr cinfo, long count)
{
    JpegStream* src = (JpegStream*)(cinfo->src);

    if (count < src->mgr.bytes_in_buffer)
    {
        src->mgr.next_input_byte += count;
        src->mgr.bytes_in_buffer -= count;
    }
    else
    {
        src->stream->seekg(count - src->mgr.bytes_in_buffer, std::ios_base::cur);
        src->mgr.bytes_in_buffer = 0;
    }
}

void term(j_decompress_ptr cinfo)
{
}

void makeStream(j_decompress_ptr cinfo, std::istream* in)
{
    JpegStream * src;

    if (cinfo->src == NULL)
    {
        cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(JpegStream));
        src = reinterpret_cast<JpegStream*>(cinfo->src);
    }

    src = reinterpret_cast<JpegStream*>(cinfo->src);
    src->mgr.init_source = initSource;
    src->mgr.fill_input_buffer = fillBuffer;
    src->mgr.skip_input_data = skip;
    src->mgr.resync_to_restart = jpeg_resync_to_restart; /* use default method */
    src->mgr.term_source = term;
    src->stream = in;
    src->mgr.bytes_in_buffer = 0;
    src->mgr.next_input_byte = 0;
}
}

JPEGFrame loadJPEG(std::istream &s)
{
    JPEGFrame result;

    result.type = JPEGFrame::TYPE_INVALID;

    int n_samples;
    jpeg_error_mgr err;
    jpeg_decompress_struct cinfo;

    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&err);

    makeStream(&cinfo, &s);

    jpeg_read_header(&cinfo, 1);
    jpeg_start_decompress(&cinfo);

    result.width = cinfo.output_width;
    result.height = cinfo.output_height;

    if (cinfo.num_components == 1)
    {
        result.type = JPEGFrame::TYPE_GRAY;
    }
    else if (cinfo.num_components == 3)
    {
        result.type = JPEGFrame::TYPE_RGB;
    }
    else
    {
        jpeg_destroy_decompress(&cinfo);
    }

    result.data = new uint8_t[cinfo.output_width * cinfo.num_components * cinfo.output_height];
    uint8_t *samples = result.data;

    while (cinfo.output_scanline < cinfo.output_height)
    {
        n_samples = jpeg_read_scanlines(&cinfo, (JSAMPARRAY) &samples, 1);
        samples += n_samples * cinfo.image_width * cinfo.num_components;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return result;
}
}
