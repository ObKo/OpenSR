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
#include <png.h>

namespace Rangers
{
struct PNGBufferHandler
{
    const char *ptr;
    size_t pos;
    size_t size;
};

struct PNGStreamHandler
{
    std::istream *stream;
};

void readPNGDataFromBuffer(png_structp png_ptr, png_bytep out, png_size_t size)
{
    if (png_get_io_ptr(png_ptr) == 0)
        return;
    PNGBufferHandler *h = (PNGBufferHandler*)png_get_io_ptr(png_ptr);
    if (h->size < h->pos + size)
        return;

    memcpy(out, h->ptr + h->pos, size);
    h->pos += size;
}

void readPNGDataFromStream(png_structp png_ptr, png_bytep out, png_size_t size)
{
    if (png_get_io_ptr(png_ptr) == 0)
        return;
    PNGStreamHandler *h = (PNGStreamHandler*)png_get_io_ptr(png_ptr);
    h->stream->read((char *)out, size);
}

bool readPNG(PNGFrame &frame, png_structp png_ptr, png_infop info_ptr)
{
    png_set_sig_bytes(png_ptr, 8);

    if (setjmp(png_jmpbuf(png_ptr)))
        return false;

    png_read_info(png_ptr, info_ptr);
    png_set_expand(png_ptr);
    png_set_strip_16(png_ptr);
    png_set_packing(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
        return false;

    frame.width = png_get_image_width(png_ptr, info_ptr);
    frame.height = png_get_image_height(png_ptr, info_ptr);
    int colorType = png_get_color_type(png_ptr, info_ptr);
    int bitDepth = png_get_bit_depth(png_ptr, info_ptr);
    int rowBytes = png_get_rowbytes(png_ptr, info_ptr);

    switch (colorType)
    {
    case PNG_COLOR_TYPE_GRAY:
        frame.type = PNG_GRAY;
        break;
    case PNG_COLOR_TYPE_RGB:
        frame.type = PNG_RGB;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        frame.type = PNG_RGBA;
        break;
    default:
        return false;
    }

    frame.data = new unsigned char[rowBytes * frame.height];
    png_bytep *rowPointers = new png_bytep[frame.height];
    for (int i = 0; i < frame.height; i++)
        rowPointers[i] = (png_bytep)(frame.data + rowBytes * i);

    png_read_image(png_ptr, rowPointers);

    delete[] rowPointers;

    return true;
}

PNGFrame loadPNG(const char *buffer, size_t bufferSize)
{
    PNGFrame frame;
    frame.width = 0;
    frame.height = 0;
    frame.type = PNG_INVALID;
    frame.data = 0;

    if (!png_check_sig((png_bytep)buffer, 8))
        return frame;

    png_structp png_ptr = 0;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

    if (png_ptr == 0)
        return frame;

    png_infop info_ptr = 0;
    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == 0)
        return frame;

    PNGBufferHandler h;
    h.ptr = buffer;
    h.pos = 8;
    h.size = bufferSize;
    png_set_read_fn(png_ptr, &h, readPNGDataFromBuffer);

    if (!readPNG(frame, png_ptr, info_ptr))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        frame.width = 0;
        frame.height = 0;
        frame.type = PNG_INVALID;
        frame.data = 0;
        return frame;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return frame;
}

PNGFrame loadPNG(std::istream &stream)
{
    PNGFrame frame;
    frame.width = 0;
    frame.height = 0;
    frame.type = PNG_INVALID;
    frame.data = 0;

    char buffer[8];
    stream.read(buffer, 8);

    if (!png_check_sig((png_bytep)buffer, 8))
        return frame;

    png_structp png_ptr = 0;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

    if (png_ptr == 0)
        return frame;

    png_infop info_ptr = 0;
    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == 0)
        return frame;

    PNGStreamHandler h;
    h.stream = &stream;
    png_set_read_fn(png_ptr, &h, readPNGDataFromStream);

    if (!readPNG(frame, png_ptr, info_ptr))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        frame.width = 0;
        frame.height = 0;
        frame.type = PNG_INVALID;
        frame.data = 0;
        return frame;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return frame;
}
}
