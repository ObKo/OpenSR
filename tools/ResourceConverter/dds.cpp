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

#include "dds.h"
#include <IL/il.h>
#include <squish.h>
#include <iostream>
#include <fstream>
#include <libRanger.h>
#include <cstring>

namespace Rangers
{
void blitBGRAToRGB16(const unsigned char *bgra, unsigned char *rgb16, int width, int height)
{
    unsigned short *p = (unsigned short *)rgb16;
    for (int i = 0; i < width * height; i++)
        p[i] = ((bgra[i * 4] >> 3) << 11) | ((bgra[i * 4 + 1] >> 2) << 5) | ((bgra[i * 4 + 2] >> 3));
}

void blitBGRAToRGB24(const unsigned char *bgra, unsigned char *rgb24, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        rgb24[i * 3] = bgra[i * 4];
        rgb24[i * 3 + 1] = bgra[i * 4 + 1];
        rgb24[i * 3 + 2] = bgra[i * 4 + 2];
    }
}

int convertImageToDDS(const std::string& inFile, const std::string& outFile, DDSType type)
{
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ilLoadImage(inFile.c_str());
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);

    unsigned char *bgraData = new unsigned char[width * height * 4];
    ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, bgraData);
    ilDeleteImages(1, &image);

    int ret = saveDataToDDS(width, height, bgraData, outFile, type);
    delete bgraData;
    return ret;
}

int saveDataToDDS(int width, int height, unsigned char *bgraData, const std::string& outFile, DDSType type)
{
    uint32_t fourCC = 0;

    size_t dataSize;
    unsigned char *data = compressData(width, height, bgraData, dataSize, type);

    DDSHeader header;
    DDSPixelFormat pixelFormat;

    switch (type)
    {
    case DDS_DXT1:
        pixelFormat.fourCC =  *((uint32_t *)"DXT1");
        break;
    case DDS_DXT3:
        pixelFormat.fourCC =  *((uint32_t *)"DXT3");
        break;
    case DDS_DXT5:
        pixelFormat.fourCC =  *((uint32_t *)"DXT5");
        break;
    }


    switch (type)
    {
    case DDS_RGB16:
        pixelFormat.rBitMask = 0xf800;
        pixelFormat.gBitMask = 0x7e;
        pixelFormat.bBitMask = 0x1f;
        pixelFormat.aBitMask = 0x0;
        pixelFormat.flags = DDPF_RGB;
        pixelFormat.fourCC = 0;
        pixelFormat.rgbBitCount = 16;
        break;
    case DDS_RGB24:
        pixelFormat.rBitMask = 0xff0000;
        pixelFormat.gBitMask = 0xff00;
        pixelFormat.bBitMask = 0xff;
        pixelFormat.aBitMask = 0;
        pixelFormat.flags = DDPF_RGB;
        pixelFormat.fourCC = 0;
        pixelFormat.rgbBitCount = 24;
        break;
    case DDS_RGBA:
        pixelFormat.rBitMask = 0xff000000;
        pixelFormat.gBitMask = 0xff0000;
        pixelFormat.bBitMask = 0xff00;
        pixelFormat.aBitMask = 0xff;
        pixelFormat.flags = DDPF_ALPHAPIXELS | DDPF_RGB;
        pixelFormat.fourCC = 0;
        pixelFormat.rgbBitCount = 32;
        break;
    case DDS_DXT1:
    case DDS_DXT3:
    case DDS_DXT5:
        pixelFormat.aBitMask = 0xff000000;
        pixelFormat.rBitMask = 0xff0000;
        pixelFormat.gBitMask = 0xff00;
        pixelFormat.bBitMask = 0xff;
        pixelFormat.flags = DDPF_ALPHAPIXELS | DDPF_FOURCC;
        pixelFormat.fourCC = fourCC;
        pixelFormat.rgbBitCount = 32;
        break;
    }

    pixelFormat.size = 32;

    header.caps = DDSCAPS_TEXTURE;
    header.caps2 = 0;
    header.ddspf = pixelFormat;
    header.depth = 1;
    header.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
    header.height = height;
    header.mipMapCount = 0;
    header.pitchOrLinearSize = getLineSize(width, height, type);
    header.size = 124;
    header.width = width;

    std::ofstream out(outFile.c_str(), std::ios_base::out | std::ios_base::binary);
    uint32_t magic = 0x20534444;
    out.write((const char *)&magic, 4);
    out.write((const char *)&header, 124);
    out.write((const char *)data, dataSize);
    out.close();

    delete data;

    return 0;
}

unsigned char *compressData(int width, int height, unsigned char *bgraData, size_t& dataSize, DDSType type)
{
    unsigned char *data;
    if ((type == DDS_DXT1) || (type == DDS_DXT3) || (type == DDS_DXT5))
    {
        int squishFlags;
        switch (type)
        {
        case DDS_DXT1:
            squishFlags = squish::kDxt1;
            break;
        case DDS_DXT3:
            squishFlags = squish::kDxt3;
            break;
        case DDS_DXT5:
            squishFlags = squish::kDxt5;
            break;
        }

        dataSize = squish::GetStorageRequirements(width, height, squishFlags);
        data = new unsigned char[dataSize];
        squish::CompressImage(bgraData, width, height, data, squishFlags);
    }
    else
    {
        dataSize = getLineSize(width, height, type) * height;
        data = new unsigned char[dataSize];
        switch (type)
        {
        case DDS_RGB16:
            blitBGRAToRGB16(bgraData, data, width, height);
            break;
        case DDS_RGB24:
            blitBGRAToRGB24(bgraData, data, width, height);
            break;
        case DDS_RGBA:
            memcpy(data, bgraData, width * height * 4);
            break;
        }
    }
    std::cout << "Image: " << width << "x" << height << " (" << width * height * 4 / 1024 << " KiB) DDS size: "
              << dataSize / 1024 << " KiB" << std::endl;
    return data;
}

int getLineSize(int width, int height, DDSType type)
{
    int squishFlags;
    switch (type)
    {
    case DDS_DXT1:
        squishFlags = squish::kDxt1;
        break;
    case DDS_DXT3:
        squishFlags = squish::kDxt3;
        break;
    case DDS_DXT5:
        squishFlags = squish::kDxt5;
        break;
    case DDS_RGB16:
        return (width * 16 + 7) / 8;
        break;
    case DDS_RGB24:
        return (width * 24 + 7) / 8;
        break;
    case DDS_RGBA:
        return (width * 32 + 7) / 8;
        break;
    }
    int dxtDataSize = squish::GetStorageRequirements(width, height, squishFlags);
    return dxtDataSize / height;
}

}
