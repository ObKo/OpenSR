/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include <IL/il.h>
#include <IL/ilu.h>
#include <iostream>

namespace Rangers
{
int createPlanet(const std::string &colorFile, const std::string &grayFile, const std::string &outFile)
{
    ILuint colorImage;
    ILuint grayImage;
    ilGenImages(1, &colorImage);
    ilGenImages(1, &grayImage);

    ilBindImage(colorImage);
    ilLoadImage(colorFile.c_str());
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);

    ilBindImage(grayImage);
    ilLoadImage(grayFile.c_str());
    if ((ilGetInteger(IL_IMAGE_WIDTH) != width) || (ilGetInteger(IL_IMAGE_HEIGHT) != height))
    {
        std::cerr << "Images have different resolutions !!!";
        return -1;
    }

    char *color = new char[width * height * 4];
    ilBindImage(colorImage);
    ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE,  color);
    ilDeleteImage(colorImage);

    char *alpha = new char[width * height];
    ilBindImage(grayImage);
    ilCopyPixels(0, 0, 0, width, height, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, alpha);
    ilDeleteImage(colorImage);

    for (int i = 0; i < width * height; i++)
    {
        color[i * 4 + 3] = alpha[i];
    }

    ILuint resultImage;
    ilGenImages(1, &resultImage);
    ilTexImage(width, height, 0, 4, IL_RGBA, IL_UNSIGNED_BYTE, color);
    iluFlipImage();
    ilSaveImage(outFile.c_str());

    delete[] color;
    delete[] alpha;
    return 0;
}
}