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

#include "pkg.h"
#include "pkg2rpkg.h"
#include "gai.h"
#include "gi.h"
#include "planet.h"
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <algorithm>

void showHelp()
{
    std::cout << "Usage: rangersresconv command [arguments]" << std::endl;
    std::cout << "Commands: " << std::endl;
    std::cout << "\tpkgx pkg_file output_dir - extract pkg file from original game to output_dir." << std::endl;
    std::cout << "\tpkg2qrc pkg_file output_dir - extract pkg file from original game to output_dir and create Qt Resource file description (qrc file)." << std::endl;
    std::cout << "\tpkg2rpkg compression pkg_file rpkg_file - convert pkg file from original game to rpkg format." << std::endl;
    std::cout << "\t\tSupported compressions: NONE ZLIB LZMA" << std::endl;
    std::cout << "\t\tSupported types: RGB16, RGB24, RGBA, DXT1, DXT3, DXT5" << std::endl;
    std::cout << "\tgai2png gai_file png_file_name - extract gai animation frames to png files." << std::endl;
    std::cout << "\t\tWill produce files named png_file_name$frame_number$.png" << std::endl;
    std::cout << "\tgi2png gi_file png_file - convert GI image file from original game to png." << std::endl;
    std::cout << "\tcreate_planet png_color_file png_gray_file planet_file - create planet image from color and grayscale image." << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        showHelp();
        return 0;
    }
    ilInit();
    iluInit();
    if (std::string(argv[1]) == "pkgx")
    {
        if (argc < 4)
        {
            showHelp();
            return 0;
        }
        Rangers::extractPKG(argv[2], argv[3]);
    }
    else if (std::string(argv[1]) == "pkg2rpkg")
    {
        if (argc < 5)
        {
            showHelp();
            return 0;
        }
        Rangers::RPKGCompression compression;
        std::string compStr = argv[2];
        std::transform(compStr.begin(), compStr.end(), compStr.begin(), toupper);

        if (compStr == "NONE")
            compression = Rangers::RPKG_NONE;
        else if (compStr == "LZMA")
            compression = Rangers::RPKG_SEEKABLE_LZMA;
        else if (compStr == "ZLIB")
            compression = Rangers::RPKG_SEEKABLE_ZLIB;

        Rangers::pkg2rpkg(argv[3], argv[4], compression);
    }
    else if (std::string(argv[1]) == "gai2png")
    {
        if (argc < 4)
        {
            showHelp();
            return 0;
        }
        Rangers::extractGAI2PNG(argv[2], argv[3]);
    }
    else if (std::string(argv[1]) == "gi2png")
    {
        if (argc < 4)
        {
            showHelp();
            return 0;
        }
        Rangers::gi2png(argv[2], argv[3]);
    }
    else if (std::string(argv[1]) == "pkg2qrc")
    {
        if (argc < 4)
        {
            showHelp();
            return 0;
        }
        Rangers::extractPKGToQRC(argv[2], argv[3]);
    }
    else if (std::string(argv[1]) == "create_planet")
    {
        if (argc < 5)
        {
            showHelp();
            return 0;
        }
        Rangers::createPlanet(argv[2], argv[3], argv[4]);
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
        showHelp();

    }
}
