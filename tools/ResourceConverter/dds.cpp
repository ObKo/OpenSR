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

namespace Rangers
{
int convertImageToDDS(const std::string& inFile, const std::string& outFile, DDSCompression compression)
{
	ilInit();
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(inFile.c_str());
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	/*ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE);
	unsigned int dxtDataSize;
	char* data = (char*)ilCompressDXT(ilGetData(), width, height, 1, IL_DXT5, &dxtDataSize);*/

	unsigned char *bgraData = new unsigned char[width * height * 4];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, bgraData);
    ilDeleteImages(1, &image);

	int ret = saveDataToDDS(width, height, bgraData, outFile, compression);
	delete bgraData;
	return ret;
}

int saveDataToDDS(int width, int height, unsigned char *bgraData, const std::string& outFile, DDSCompression compression)
{
	int squishFlags;
	uint32_t fourCC;
	int blockSize;

	switch(compression)
	{
	case DDS_DXT1:
		squishFlags = squish::kDxt1;
	    fourCC =  *((uint32_t *)"DXT1");
		blockSize = 8;
		break;
	case DDS_DXT3:
	    squishFlags = squish::kDxt3;
		fourCC =  *((uint32_t *)"DXT3");
		blockSize = 16;
		break;
	case DDS_DXT5:
		squishFlags = squish::kDxt5;
		fourCC =  *((uint32_t *)"DXT5");
		blockSize = 16;
		break;
	}

	int dxtDataSize = squish::GetStorageRequirements(width, height, squishFlags);
    unsigned char *data = new unsigned char[dxtDataSize];
	squish::CompressImage(bgraData, width, height, data, squishFlags);

	std::cout << "Image: " << width << "x" << height << " (" << width * height * 4 / 1024 << " KiB) DXT size: "
			  << dxtDataSize / 1024 << " KiB" << std::endl;

	DDSHeader header;
	DDSPixelFormat pixelFormat;

	pixelFormat.aBitMask = 0xff000000;
	pixelFormat.rBitMask = 0xff0000;
	pixelFormat.gBitMask = 0xff00;
	pixelFormat.bBitMask = 0xff;
    pixelFormat.flags = DDPF_ALPHAPIXELS | DDPF_FOURCC;
    pixelFormat.fourCC = fourCC;
    pixelFormat.rgbBitCount = 32;
    pixelFormat.size = 32;

    header.caps = 0x1000;
	header.caps2 = 0;
	header.ddspf = pixelFormat;
	header.depth = 1;
	header.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	header.height = height;
	header.mipMapCount = 0;
	header.pitchOrLinearSize = ((width + 3) / 4) * blockSize;
    header.size = 124;
    header.width = width;

    std::ofstream out(outFile.c_str(), std::ios_base::out | std::ios_base::binary);
    uint32_t magic = 0x20534444;
    out.write((const char *)&magic, 4);
    out.write((const char *)&header, 124);
    out.write((const char *)data, dxtDataSize);
    out.close();

    delete data;

    return 0;
}
}
