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

#ifndef DDS_H
#define DDS_H

#include <string>

namespace Rangers
{
enum DDSCompression {DDS_DXT1, DDS_DXT3, DDS_DXT5};
int convertImageToDDS(const std::string& inFile, const std::string& outFile, DDSCompression compression);
int saveDataToDDS(int width, int height, unsigned char *bgraData, const std::string& outFile, DDSCompression compression);
unsigned char *compressDXTData(int width, int height, unsigned char *bgraData, DDSCompression compression);
int getLineSize(int width, int height, DDSCompression compression);
}

#endif
