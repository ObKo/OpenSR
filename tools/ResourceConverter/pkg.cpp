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
void extract(const PKGItem *item, std::istream &file, const std::string& currentPath, std::list<std::string> &fileNames)
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
            extract(&item->childs[i], file, newPath, fileNames);
        }
    }
    else
    {
        std::string fileName = currentPath + '/' + item->name;
        std::cout << fileName << std::endl;
        fileNames.push_back(fileName);
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

void extractQRC(const PKGItem *item, std::istream &file, const std::string& dir, const std::string& currentPath, std::ostream &qrcFile)
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
            if (currentPath.empty())
                newPath = item->name;
            else
                newPath = currentPath + '/' + item->name;
            std::cout << newPath << std::endl;
        }

        if (!createDirPath(dir + "/" + newPath))
        {
            std::cerr << "Cannot create dir " << dir + "/" + newPath << std::endl;
            return;
        }
        for (int i = 0; i < item->childCount; i++)
        {
            extractQRC(&item->childs[i], file, dir, newPath, qrcFile);
        }
    }
    else
    {
        std::string fileName = dir + '/' + currentPath + '/' + item->name;
        std::cout << fileName << std::endl;
        qrcFile << "<file>" << currentPath + '/' + item->name << "</file>" << std::endl;
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
    std::list<std::string> fnames;
    extract(root, pkg, outDir, fnames);
}

void extractPKGToQRC(const std::string &pkgFile, const std::string &outDir)
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
    std::ofstream qrc(outDir + "/" + basename(pkgFile) + ".qrc");
    qrc << "<!DOCTYPE RCC><RCC version=\"1.0\"><qresource>" << std::endl;
    extractQRC(root, pkg, outDir, "", qrc);
    qrc << "</qresource></RCC>" << std::endl;
    qrc.close();
}
}