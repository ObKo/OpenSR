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
#include <libRanger.h>
#include <fstream>
#include <iostream>
#include <list>
#include <cstring>
#include <map>

using namespace std;
using namespace Rangers;

namespace Rangers
{
void countFiles(std::string dir, PKGItem *base, std::map<RPKGEntry *, PKGItem*>& itemMap)
{
    for (int i = 0; i < base->childCount; i++)
    {
        if (base->childs[i].dataType == 3)
            countFiles(dir + base->childs[i].name + "/", &base->childs[i], itemMap);
        else
        {
            RPKGEntry *e = new RPKGEntry;
            e->name = dir + base->childs[i].name;
            itemMap[e] = &base->childs[i];
        }
    }
}

void cleanup(PKGItem *root)
{
    for (int i = 0; i < root->childCount; i++)
    {
        if (root->childs[i].dataType == 3)
            cleanup(&root->childs[i]);
    }
    delete[] root->childs;
}

int pkg2rpkg(const std::string& inputFile, const std::string& outputFile, Rangers::RPKGCompression compression)
{
    ifstream pkgfile(inputFile.c_str(), ios::binary | ios::in);
    std::map<RPKGEntry*, PKGItem*> itemMap;
    PKGItem *root = loadPKG(pkgfile);
    if (root->childCount != 1)
    {
        std::cout << "Incorrect input file" << endl;
        return -1;
    }
    countFiles("", root, itemMap);

    //FIXME: OH SHI~
    std::vector<RPKGEntry> files;
    std::vector<PKGItem> pkgfiles;
    for (std::map<RPKGEntry*, PKGItem*>::iterator i = itemMap.begin(); i != itemMap.end(); ++i)
    {
        files.push_back(*(i->first));
        pkgfiles.push_back(*(i->second));
    }

    uint32_t headerSize = calculateRPKGHeaderSize(files);
    ofstream outfile(outputFile.c_str(), ios::binary | ios::out);
    //FIXME: Better way to fill file
    for (int i = 0; i < headerSize + 8; i++)
        outfile.put(0);

    for (int i = 0; i < files.size(); i++)
    {
        cout << files[i].name << endl;
        unsigned char *data = extractFile(pkgfiles[i], pkgfile);
        appendRPKGFile(outfile, files[i], (const char*)data, pkgfiles[i].size, compression);
        delete data;
    }
    outfile.seekp(0, ios_base::beg);
    writeRPKGHeader(outfile, files);

    cleanup(root);
    delete root;
    outfile.close();
    pkgfile.close();

    return 0;
}
}
