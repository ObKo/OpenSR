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

#include "gai.h"
#include <fstream>
#include <IL/il.h>
#include <cstring>
#include <cstdlib>
#include <libRanger.h>
#include "utils.h"

namespace Rangers
{
int extractGAI2PNG(const std::string& gaiFile, const std::string& outName)
{
    std::ifstream gaiStream(gaiFile.c_str(), std::ios_base::in | std::ios_base::binary);
    size_t offset = 0;
    GAIHeader header = loadGAIHeader(gaiStream, offset);
    offset = 0;
    GIFrame *bg = 0;
    if (header.haveBackground)
    {
        std::ifstream bgStream((directory(gaiFile) + "/" + basename(gaiFile) + ".gi").c_str(), std::ios_base::in | std::ios_base::binary);
        bg = new GIFrame();
        *bg = loadGIFile(bgStream);
        bgStream.close();
    }
    GAIAnimation gai = loadGAIAnimation(gaiStream, offset, bg);
    gaiStream.close();

    ILuint frames[gai.frameCount];
    char counts[5];
    ilInit();
    ilGenImages(gai.frameCount, frames);
    for (int i = 0; i < gai.frameCount; i++)
    {
        snprintf(counts, 5, "%04d", i);
        GIFrame g = gai.frames[i];
        ilBindImage(frames[i]);
        ilTexImage(g.width, g.height, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, g.data);
        ilSaveImage((directory(outName) + "/" + basename(outName) + counts + ".png").c_str());
        ilDeleteImage(frames[i]);
        delete g.data;
    }
    delete gai.frames;
    return 0;
}

int gai2dds(const std::string& gaiFile, const std::string& ddsFile, DDSCompression compression)
{
    std::ifstream gaiStream(gaiFile.c_str(), std::ios_base::in | std::ios_base::binary);
    size_t offset = 0;
    GAIHeader header = loadGAIHeader(gaiStream, offset);
    offset = 0;
    GIFrame *bg = 0;
    if (header.haveBackground)
    {
        std::ifstream bgStream((directory(gaiFile) + "/" + basename(gaiFile) + ".gi").c_str(), std::ios_base::in | std::ios_base::binary);
        bg = new GIFrame();
        *bg = loadGIFile(bgStream);
        bgStream.close();
    }
    GAIAnimation gai = loadGAIAnimation(gaiStream, offset, bg);
    gaiStream.close();

    DDSHeader ddsHeader;
    DDSPixelFormat pixelFormat;

    uint32_t fourCC;

    switch (compression)
    {
    case DDS_DXT1:
        fourCC =  *((uint32_t *)"DXT1");
        break;
    case DDS_DXT3:
        fourCC =  *((uint32_t *)"DXT3");
        break;
    case DDS_DXT5:
        fourCC =  *((uint32_t *)"DXT5");
        break;
    }

    pixelFormat.aBitMask = 0xff000000;
    pixelFormat.rBitMask = 0xff0000;
    pixelFormat.gBitMask = 0xff00;
    pixelFormat.bBitMask = 0xff;
    pixelFormat.flags = DDPF_ALPHAPIXELS | DDPF_FOURCC;
    pixelFormat.fourCC = fourCC;
    pixelFormat.rgbBitCount = 32;
    pixelFormat.size = 32;

    ddsHeader.caps = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX;
    ddsHeader.caps2 = DDSCAPS2_VOLUME;
    ddsHeader.ddspf = pixelFormat;
    ddsHeader.depth = gai.frameCount;
    ddsHeader.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_DEPTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
    ddsHeader.height = gai.height;
    ddsHeader.mipMapCount = 0;
    ddsHeader.pitchOrLinearSize = getLineSize(gai.width, gai.height, compression);
    ddsHeader.size = 124;
    ddsHeader.width = gai.width;
    ddsHeader.reserved1[0] = gai.waitSeek;
    ddsHeader.reserved1[1] = gai.waitSize;

    std::ofstream out(ddsFile.c_str(), std::ios_base::out | std::ios_base::binary);
    uint32_t magic = 0x20534444;
    out.write((const char *)&magic, 4);
    out.write((const char *)&ddsHeader, 124);

    for (int i = 0; i < gai.frameCount; i++)
    {
        GIFrame g = gai.frames[i];
        BGRAToRGBA((char*)g.data, g.width, g.height);
        unsigned char *dxtData = compressDXTData(gai.width, gai.height, g.data, compression);
        out.write((const char *)dxtData, ddsHeader.pitchOrLinearSize * gai.height);
        delete dxtData;
        delete g.data;
    }
    delete gai.frames;
    out.close();
    return 0;
}
}
