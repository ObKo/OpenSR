/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2015 Kosyak <ObKo@mail.ru>

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

#ifndef LIBRANGER_QT_H
#define LIBRANGER_QT_H

#include <QtGlobal>

#ifdef LIBRANGER_LIBRARY
# ifdef Q_OS_WIN32
#  define LIBRANGER_API __declspec(dllexport)
# else
#  define LIBRANGER_API
# endif
#else
# ifdef Q_OS_WIN32
#  define LIBRANGER_API __declspec(dllimport)
# else
#  define LIBRANGER_API
# endif
#endif

#include <stdint.h>

#include <QImage>
#include <QByteArray>

class QIODevice;

namespace OpenSR
{
const uint32_t GI_FRAME_SIGNATURE = 0x00006967;
const uint32_t GAI_SIGNATURE = 0x00696167;
const uint32_t HAI_SIGNATURE = 0x04210420;
const uint32_t ZL02_SIGNATURE = 0x32304c5a;
const uint32_t ZL01_SIGNATURE = 0x31304c5a;
//FIXME: Check this
const int HAI_FRAME_TIME = 50;

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
     *  -# 5 - Delta frame of GAI animation.
     */
    uint32_t layerCount; //!< Number of layers in frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
};

//! Header of animation in *.gai file
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

//! PKG archive item
struct PKGItem
{
    uint32_t sizeInArc;  //!< Compressed data size
    uint32_t size;  //!< Uncompressed data size
    char fullName[63];  //!< File name in upper case
    char name[63];  //!< File name
    uint32_t dataType;  //!< Data type
    /*!<
     * Variants:
     *  -# 1 - RAW data
     *  -# 2 - ZLIB compressed file
     *  -# 3 - Directory
     */
    uint32_t offset;  //!< Fille offset in archive
    uint32_t childCount; //!< Number of childs
    PKGItem *parent;  //!< Parent item
    PKGItem *childs;  //!< Child items
};

struct Animation
{
    QVector<QImage> images;
    QVector<int> times;
};

LIBRANGER_API bool checkHAIHeader(QIODevice *dev);
LIBRANGER_API HAIHeader peekHAIHeader(QIODevice *dev);
LIBRANGER_API HAIHeader readHAIHeader(QIODevice *dev);

LIBRANGER_API QVector<int> loadHAITimes(QIODevice *dev, const HAIHeader& header);
LIBRANGER_API QImage loadHAIFrame(QIODevice *dev, const HAIHeader& header, int i);

LIBRANGER_API Animation loadHAIAnimation(QIODevice *dev);

LIBRANGER_API bool checkGAIHeader(QIODevice *dev);
LIBRANGER_API GAIHeader peekGAIHeader(QIODevice *dev);
LIBRANGER_API GAIHeader readGAIHeader(QIODevice *dev);

LIBRANGER_API QVector<int> loadGAITimes(QIODevice *dev, const GAIHeader& header);
LIBRANGER_API QImage loadGAIFrame(QIODevice *dev, const GAIHeader& header, int i, const QImage &background);

LIBRANGER_API Animation loadGAIAnimation(QIODevice *dev, const QImage &background = QImage());

LIBRANGER_API bool checkGIHeader(QIODevice *dev);
LIBRANGER_API GIFrameHeader peekGIHeader(QIODevice *dev);
LIBRANGER_API GIFrameHeader readGIHeader(QIODevice *dev);

LIBRANGER_API QImage loadGIFrame(QIODevice *dev, bool animation = false, const QImage &background = QImage(), int startX = 0, int startY = 0, int finishX = 0, int finishY = 0);
LIBRANGER_API void decodeGAIDeltaFrame(QImage &dest, int x, int y, QIODevice *dev);

LIBRANGER_API PKGItem *loadPKG(QIODevice *dev);
LIBRANGER_API QByteArray extractFile(const PKGItem &item, QIODevice *dev);

LIBRANGER_API QByteArray unpackZL(QByteArray &src);
LIBRANGER_API QByteArray packZL01(const QByteArray &src);
LIBRANGER_API QByteArray packZL02(const QByteArray &src);

LIBRANGER_API void readDATTree(QIODevice *dev, QVariantMap *parent, bool isCache = false);
LIBRANGER_API QVariantMap loadDAT(QIODevice *dev, bool isCache = false);

LIBRANGER_API void writeDATTree(QIODevice *dev, const QVariant& node, const QString& name);
LIBRANGER_API void saveDAT(QIODevice *dev, const QVariant& root);
}

#endif
