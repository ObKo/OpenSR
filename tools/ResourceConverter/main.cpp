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

#include "pkg2rpkg.h"
#include "dds.h"
#include "gai.h"
#include "gi.h"
#include <iostream>
#include <algorithm>

void showHelp()
{
    std::cout << "Usage: rangersresconv command [arguments]" << std::endl;
    std::cout << "Commands: " << std::endl;
    std::cout << "\tpkg2rpkg pkg_file rpkg_file - convert pkg file from original game to rpkg format." << std::endl;
    std::cout << "\tany2dds type image_file dds_file - create dds texture from image." << std::endl;
    std::cout << "\t\tSupported types: RGB16, RGB24, RGBA, DXT1, DXT3, DXT5" << std::endl;
    std::cout << "\tgai2png gai_file png_file_name - extract gai animation frames to png files." << std::endl;
    std::cout << "\t\tWill produce files named png_file_name$frame_number$.png" << std::endl;
    std::cout << "\tgi2png gi_file png_file - convert GI image file from original game to png." << std::endl;
    std::cout << "\tgi2dds compression gi_file dds_file - convert GI image file from original game to dds format." << std::endl;
    std::cout << "\tgai2dds compression gai_file dds_file - convert GAI animation file from original game to dds animation." << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        showHelp();
        return 0;
    }
    if (std::string(argv[1]) == "pkg2rpkg")
    {
        if (argc < 4)
        {
            showHelp();
            return 0;
        }
        Rangers::pkg2rpkg(argv[2], argv[3]);
    }
    else if (std::string(argv[1]) == "any2dds")
    {
        if (argc < 5)
        {
            showHelp();
            return 0;
        }
        std::string argv2 = std::string(argv[2]);
        std::transform(argv2.begin(), argv2.end(), argv2.begin(), ::toupper);
        Rangers::DDSType type;
        if (argv2 == "DXT1")
            type = Rangers::DDS_DXT1;
        else if (argv2 == "DXT3")
            type = Rangers::DDS_DXT3;
        else if (argv2 == "DXT5")
            type = Rangers::DDS_DXT5;
        else if (argv2 == "RGB16")
            type = Rangers::DDS_RGB16;
        else if (argv2 == "RGB24")
            type = Rangers::DDS_RGB24;
        else if (argv2 == "RGBA")
            type = Rangers::DDS_RGBA;
        else
        {
            std::cout << "Unsupported type." << std::endl;
            showHelp();
            return 0;
        }
        Rangers::convertImageToDDS(argv[3], argv[4], type);
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
    else if (std::string(argv[1]) == "gi2dds")
    {
        if (argc < 5)
        {
            showHelp();
            return 0;
        }
        std::string argv2 = std::string(argv[2]);
        std::transform(argv2.begin(), argv2.end(), argv2.begin(), ::toupper);
        Rangers::DDSType type;
        if (argv2 == "DXT1")
            type = Rangers::DDS_DXT1;
        else if (argv2 == "DXT3")
            type = Rangers::DDS_DXT3;
        else if (argv2 == "DXT5")
            type = Rangers::DDS_DXT5;
        else if (argv2 == "RGB16")
            type = Rangers::DDS_RGB16;
        else if (argv2 == "RGB24")
            type = Rangers::DDS_RGB24;
        else if (argv2 == "RGBA")
            type = Rangers::DDS_RGBA;
        else
        {
            std::cout << "Unsupported compression." << std::endl;
            showHelp();
            return 0;
        }
        Rangers::gi2dds(argv[3], argv[4], type);
    }
    else if (std::string(argv[1]) == "gai2dds")
    {
        if (argc < 5)
        {
            showHelp();
            return 0;
        }
        std::string argv2 = std::string(argv[2]);
        std::transform(argv2.begin(), argv2.end(), argv2.begin(), ::toupper);
        Rangers::DDSType compression;
        if (argv2 == "DXT1")
            compression = Rangers::DDS_DXT1;
        else if (argv2 == "DXT3")
            compression = Rangers::DDS_DXT3;
        else if (argv2 == "DXT5")
            compression = Rangers::DDS_DXT5;
        else
        {
            std::cout << "Unsupported compression." << std::endl;
            showHelp();
            return 0;
        }
        Rangers::gai2dds(argv[3], argv[4], compression);
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
        showHelp();

    }
}
