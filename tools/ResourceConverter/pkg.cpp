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

#include "pkg.h"
#include <fstream>
#include <iostream>
#include <libRanger.h>
#include <errno.h>
#include <cstring>

namespace Rangers
{
namespace
{
void extract(const PKGItem *item, std::istream &file, const std::string& currentPath)
{
    if (item->dataType == 3)
    {
        std::string newPath;
        if (std::string(item->name) == "")
        {
            newPath = currentPath;
        }
        else
        {
            newPath = currentPath + '/' + item->name;
            std::cout << newPath << std::endl;
        }

        if (!createDirPath(newPath))
        {
            std::cerr << "Cannot create dir " << newPath << std::endl;
            return;
        }
        for (int i = 0; i < item->childCount; i++)
        {
            extract(&item->childs[i], file, newPath);
        }
    }
    else
    {
        std::string fileName = currentPath + '/' + item->name;
        std::cout << fileName << std::endl;
        std::ofstream outf(fileName);
        if (!outf.is_open())
        {
            std::cerr << "Cannot open output file " << fileName << ": " << strerror(errno) << std::endl;
            return;
        }
        char *data = (char*)extractFile(*item, file);
        if (!data)
        {
            std::cerr << "Cannot extract file " << fileName << std::endl;
            return;
        }
        outf.write(data, item->size);
        delete[] data;
        outf.close();
    }
}
}

void extractPKG(const std::string &pkgFile, const std::string &outDir)
{
    std::ifstream pkg(pkgFile);
    if (!pkg.is_open())
    {
        std::cerr << "Cannot open pkg archive " << pkgFile << ": " << strerror(errno) << std::endl;
        return;
    }
    PKGItem *root = loadPKG(pkg);
    if (!root)
    {
        std::cerr << "Cannot load pkg archive " << pkgFile << ": " << strerror(errno) << std::endl;
        return;
    }
    extract(root, pkg, outDir);
}
}