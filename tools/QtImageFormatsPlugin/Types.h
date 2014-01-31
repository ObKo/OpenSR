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

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <QPoint>

class QImage;
class QIODevice;

namespace Rangers
{
//! Header of layer in *.gi files
struct GILayerHeader
{
    uint32_t seek;  //!< Layer offset in file
    uint32_t size;  //!< Layer size
    uint32_t startX;  //!< Layer left corner
    uint32_t startY;  //!< Layer top corner
    uint32_t finishX;  //!< Layer rigth corner
    uint32_t finishY;  //!< Layer bottom corner
    uint32_t unknown1;
    uint32_t unknown2;
};

//! Header of frame in *.gi file

struct GIFrameHeader
{
    uint32_t signature;  //!< Signature
    uint32_t version;  //!< Version of GI file
    uint32_t startX;  //!< Left corner
    uint32_t startY;  //!< Top corner
    uint32_t finishX;  //!< Right corner
    uint32_t finishY;  //!< Bottom corner
    uint32_t rBitmask;  //!< Mask of r color component
    uint32_t gBitmask;  //!< Mask of g color component
    uint32_t bBitmask;  //!< Mask of b color component
    uint32_t aBitmask;  //!< Mask of a color component
    uint32_t type;  //!< Frame type
    /*!<
     * Variants:
     *  -# 0 - One layer, 16 or 32 bit, depends on mask.
     *  -# 1 - One layer, 16 bit RGB optimized.
     *  -# 2 - Three layers:
     *   -# 16 bit RGB optimized - body
     *   -# 16 bit RGB optimized - outline
     *   -# 6 bit Alpha optimized
     *  -# 3 - Two layers:
     *   -# Indexed RGB colors
     *   -# Indexed Alpha
     *  -# 4 - One layer, indexed RGBA colors
     *  -# 5 - Unknown type, used in large animations.
     */
    uint32_t layerCount; //!< Number of layers in frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
};

struct GAIHeader
{
    uint32_t signature;  //!< File signature
    uint32_t version;  //!< Format version
    uint32_t startX;  //!< Left corner
    uint32_t startY;  //!< Top corner
    uint32_t finishX;  //!< Right corner
    uint32_t finishY;  //!< Bottom corner
    uint32_t frameCount; //!< Number of frames in animation
    uint32_t haveBackground; //!< Animation a background in separate file
    uint32_t waitSeek;  //!< Wait seek?
    uint32_t waitSize;  //!< Wait size?
    uint32_t unknown1;
    uint32_t unknown2;
};

struct HAIHeader
{
    uint32_t signature;  //!< Signature
    uint32_t width;  //!< Animation width
    uint32_t height;  //!< Animation height
    uint32_t rowBytes;  //!< Bytes in one line
    uint32_t count;  //!< Number of frames in animation
    uint32_t frameSize;  //!< Size of one frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t unknown6;
    uint32_t palSize;  //!< Size of pallete
};

bool loadGIImageData(QIODevice *device, QImage *image, const GIFrameHeader& header, GILayerHeader *layers, uint32_t offset = 0, QImage *background = 0);
bool loadGIFrame(QIODevice *device, QImage *image, uint32_t offset = 0, QImage *background = 0, const QPoint& start = QPoint());
bool loadGAIFrame(QIODevice *device, QImage *image, GAIHeader *header, int num, QImage *background = 0);
bool loadHAIFrame(QIODevice *device, QImage *image, HAIHeader *header, int num);
}

#endif // TYPES_H
