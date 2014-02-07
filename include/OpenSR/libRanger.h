/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LIBRANGER_H
#define RANGERS_LIBRANGER_H

#include <iostream>
#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <boost/shared_ptr.hpp>

#ifdef LIBRANGER_LIBRARY
#ifdef WIN32
#define LIBRANGER_API __declspec(dllexport)
#else
#define LIBRANGER_API
#endif
#else
#ifdef WIN32
#define LIBRANGER_API
#else
#define LIBRANGER_API extern
#endif
#endif

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
     *  -# 5 - Delta frame of GAI animation.
     */
    uint32_t layerCount; //!< Number of layers in frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
};

//! GI format frame.

struct GIFrame
{
    enum Format {Format_ARGB32, Format_RGB16};
    Format format;
    int width;   //!< Total frame width
    int height;   //!< Total frame height
    uint8_t *data; //!< Frame data
};

//! Header of *.gai animation file

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

//! GAI Animation

struct GAIAnimation
{
    int width;   //!< Total animation width
    int height;   //!< Total animation height
    int frameCount;  //!< Number of frames in animation

    uint32_t *times;  //!< Display time for each frame
    GIFrame *frames;  //!< Animation frames
};

//! Header of *.hai animation file

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

//! HAI animation

struct HAIAnimation
{
    int width;   //!< Animation width
    int height;   //!< Animation height
    int frameCount;  //!< Number of frames in animation
    uint8_t *frames; //!< Frames data
};

//! Compression type of file in RPKG archive
enum RPKGCompression {RPKG_SEEKABLE_ZLIB, RPKG_SEEKABLE_LZMA, RPKG_NONE};

//! File in RPKG archive
struct RPKGItem
{
    uint32_t packType;  //!< Compression type
    uint32_t packSize;  //!< Compressed data size
    uint32_t size;  //!< Uncompressed data size
    uint32_t chunkSize; //!< Compression chunk size
    unsigned char *data; //!< Data
};

//! File description in RPKG
struct RPKGEntry
{
    std::wstring name;  //!< File name
    uint32_t offset;  //!< File offset in archive
    uint32_t size; //!< File size in archive
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

//! PNG frame
struct PNGFrame
{
    //! Supported PNG color types
    enum Type {TYPE_INVALID, TYPE_GRAY, TYPE_RGB, TYPE_RGBA};
    int width;  		//!< Frame width
    int height;   		//!< Frame width
    Type type;		//!< Frame color type
    unsigned char *data;	//!< Frame pixel data
};

//! JPEG Frame
struct JPEGFrame
{
    enum Type {TYPE_INVALID, TYPE_GRAY, TYPE_RGB};
    Type type;
    int width;
    int height;
    uint8_t *data;
};

//! Record in DAT file (from orignal game)
struct DATRecord
{
    /*!
     * Record type
     */
    enum Type
    {
        VALUE, //!< key-value pair
        NODE, //!< node, array of key-value pairs
        ARRAY //!< array of values
    };

    Type type;           //!< Type of record

    std::wstring name;   //!< Name of record (key)
    std::wstring value;  //!< Value of record

    typedef std::vector<DATRecord>::iterator iterator;
    typedef std::vector<DATRecord>::const_iterator const_iterator;

    DATRecord(Type type = VALUE, const std::wstring& name = std::wstring(), const std::wstring& value = std::wstring());

    std::vector<DATRecord> children() const;

    std::vector<DATRecord>::size_type size() const;
    bool empty() const;

    iterator add(const DATRecord& record);
    void erase(int i);
    void erase(const std::wstring& key);
    void clear();

    const DATRecord& at(int i) const;
    const DATRecord& at(const std::wstring& key) const;

    const_iterator find(const std::wstring& key) const;
    iterator find(const std::wstring& key);

    DATRecord& operator[](int i);
    DATRecord& operator[](const std::wstring& key);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

private:
    //! Record's children.
    /*!
     * In case of \ref ARRAY - array of records w/o names
     * In case of \ref NODE - array of records sorted by names
     *   for faster lookup.
     */
    std::vector<DATRecord> m_children;
};

//! Load GI frame type 0
LIBRANGER_API GIFrame loadFrameType0(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset = 0);
//! Load GI frame type 1
LIBRANGER_API GIFrame loadFrameType1(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset = 0);
//! Load GI frame type 2
LIBRANGER_API GIFrame loadFrameType2(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset = 0);
//! Load GI frame type 3
LIBRANGER_API GIFrame loadFrameType3(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset = 0);
//! Load GI frame type 4
LIBRANGER_API GIFrame loadFrameType4(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, uint32_t offset = 0);
//! Load GI frame type 5
LIBRANGER_API GIFrame loadFrameType5(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, GIFrame *background = 0, uint32_t offset = 0);
//! Load GI frame
LIBRANGER_API GIFrame loadGIImageData(const GIFrameHeader& image, const GILayerHeader *layers, std::istream& stream, GIFrame *background = 0, uint32_t offset = 0);

LIBRANGER_API void decodeGAIDeltaFrame(uint8_t *prevFrame, int prevWidth, int x, int y, const uint8_t* frameData);

//! Load GI frame from stream
LIBRANGER_API GIFrame loadGIFrame(std::istream& stream, bool animation = false, GIFrame *background = 0, uint32_t offset = 0, int startX = 0, int startY = 0, int finishX = 0, int finishY = 0);
//! Load HAI animation from stream
LIBRANGER_API HAIAnimation loadHAIAnimation(std::istream& stream);

//! Load GAI animation file
LIBRANGER_API GAIAnimation loadGAIAnimation(std::istream& stream, GIFrame *background = 0);
//! Load GAI animation file
LIBRANGER_API GAIAnimation loadGAIAnimation(std::istream& stream, const GAIHeader& header, GIFrame *background = 0);
//! Load GAI header from file
LIBRANGER_API GAIHeader loadGAIHeader(std::istream& stream);
//! Load GAI frame times from file
LIBRANGER_API uint32_t* loadGAITimes(std::istream& stream, const GAIHeader& header);

//! Get std::istream of file in package
LIBRANGER_API std::istream* getRPKGFileStream(const RPKGEntry& e, boost::shared_ptr<std::istream> stream);

//! Pack data to ZL01
LIBRANGER_API unsigned char *packZL01(const unsigned char * src, size_t srclen, size_t& destlen);
//! Unpack ZL01 - compressed data
LIBRANGER_API unsigned char *unpackZL01(const unsigned char * src, size_t srclen, size_t& destlen);
//! Unpack ZL02 - compressed data
LIBRANGER_API unsigned char *unpackZL02(const unsigned char * src, size_t srclen, size_t& destlen);
//! Unpack ZL02 - compressed data
LIBRANGER_API void unpackZL02(unsigned char * dst, const unsigned char * src, size_t srclen, size_t& destlen);

//! Load file descriptions from PKG archive
LIBRANGER_API PKGItem *loadPKG(std::istream& stream);
//! Extract file from PKG archive
LIBRANGER_API unsigned char *extractFile(PKGItem item, std::istream& pkgfile);

//! Pack data into RPKG using LZMA compression
LIBRANGER_API bool packRSXZ(const char * src, size_t srclen, RPKGItem &item);
//! Unpack LZMA-compressed file from RPKG archive
LIBRANGER_API char *unpackRSXZ(RPKGItem item);

//! Pack data into RPKG using ZLIB compression
LIBRANGER_API bool packRSZL(const char * src, size_t srclen, RPKGItem &item);
//! Unpack ZLIB-compressed file from RPKG archive
LIBRANGER_API char *unpackRSZL(RPKGItem item);

//! Load file descriptions from RPKG archive
LIBRANGER_API std::list<RPKGEntry> loadRPKG(std::istream& stream);
//! Load file from archive
LIBRANGER_API char *extractFile(const RPKGEntry& e, std::istream& stream, size_t &size);

//! Calculate size of RPKG archive header
LIBRANGER_API uint32_t calculateRPKGHeaderSize(const std::vector<RPKGEntry>& entryList);
//! Add data to RPKG archive
LIBRANGER_API void appendRPKGFile(std::ofstream& out, RPKGEntry& e, const char *data, uint32_t size, RPKGCompression compression);
//! Write RPKG archive header
LIBRANGER_API void writeRPKGHeader(std::ofstream& out, const std::vector<RPKGEntry>& entryList);

//! Convert text using iconv
LIBRANGER_API char* convertText(const char *to, const char *from, const char *what, int srcLength, int &destLength);

//! Convert text from charset to wstring
LIBRANGER_API std::wstring fromCodec(const char *codec, const char *text, int length = -1);
//! Convert text from UTF-8 to wstring
LIBRANGER_API std::wstring fromUTF8(const char *text, int length = -1);
//! Convert text from ASCII to wstring
LIBRANGER_API std::wstring fromASCII(const char *text, int length = -1);
//! Convert text from Local charset to wstring
LIBRANGER_API std::wstring fromLocal(const char *text, int length = -1);

//! Convert wstring to charset
LIBRANGER_API std::string toCodec(const char *codec, const std::wstring& text);

//! Convert wstring to UTF-8
LIBRANGER_API std::string toUTF8(const std::wstring& text);
//! Convert wstring to ASCII
LIBRANGER_API std::string toASCII(const std::wstring& text);
//! Convert wstring to local charset
LIBRANGER_API std::string toLocal(const std::wstring& text);

//! Split string
LIBRANGER_API std::vector<std::wstring> split(const std::wstring& s, wchar_t c);
//! Remove leading and trailing whitespace/tabs/newlines from a string
LIBRANGER_API std::wstring trim(const std::wstring& s);
//! Get file extension
LIBRANGER_API std::wstring suffix(const std::wstring& s);
//! Get file basename
LIBRANGER_API std::wstring basename(const std::wstring& s);
//! Get file dir
LIBRANGER_API std::wstring directory(const std::wstring& s);

//! Split string
LIBRANGER_API std::vector<std::string> split(const std::string& s, char c);
//! Remove leading and trailing whitespace/tabs/newlines from a string
LIBRANGER_API std::string trim(const std::string& s);
//! Get file extension
LIBRANGER_API std::string suffix(const std::string& s);
//! Get file basename
LIBRANGER_API std::string basename(const std::string& s);
//! Get file dir
LIBRANGER_API std::string directory(const std::string& s);

//! Calculate 32-bit FNV-1a hash of string
LIBRANGER_API uint32_t textHash32(const std::string& s);
//! Calculate 32-bit FNV-1a hash of string
LIBRANGER_API uint32_t textHash32(const std::wstring& s);

//! Create directory path
LIBRANGER_API bool createDirPath(const std::wstring& path);
//! Check directory writable
LIBRANGER_API bool checkDirWritable(const std::wstring& path);
//! Check file/directory exists
LIBRANGER_API bool fileExists(const std::wstring& path);

//! Load PNG file into memory
LIBRANGER_API PNGFrame loadPNG(const char *buffer, size_t bufferSize);
//! Load PNG file into memory
LIBRANGER_API PNGFrame loadPNG(std::istream &stream);

//! Read key-value tree from DAT file
LIBRANGER_API void readDATTree(std::istream &stream, DATRecord& node);
//! Load DAT file
LIBRANGER_API DATRecord loadDAT(std::istream &stream);

//! Write key-value DAT tree to stream
LIBRANGER_API void writeDATTree(std::ostream &stream, const DATRecord& node);
//! Save DAT file
LIBRANGER_API void saveDAT(std::ostream &stream, const DATRecord& root);

//! Load JPEG file into memory
LIBRANGER_API JPEGFrame loadJPEG(std::istream &stream);
}

#endif
