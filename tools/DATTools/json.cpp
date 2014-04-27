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

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <json/writer.h>
#include <json/reader.h>
#include <dirent.h>

#include <OpenSR/libRanger.h>

void record2JSON(const Rangers::DATRecord& record, Json::Value& json)
{
    if (record.type == Rangers::DATRecord::ARRAY)
    {
        json = Json::Value(Json::arrayValue);
        for (const Rangers::DATRecord& c : record)
        {
            Json::Value child;
            record2JSON(c, child);
            json.append(child);
        }
    }
    else if (record.type == Rangers::DATRecord::NODE)
    {
        json = Json::Value(Json::objectValue);
        for (const Rangers::DATRecord& c : record)
        {
            Json::Value child;
            record2JSON(c, child);
            json[c.name] = child;
        }
    }
    else if (record.type == Rangers::DATRecord::VALUE)
    {
        json = record.value;
    }
}

void JSON2Record(Rangers::DATRecord& record, const Json::Value& json)
{
    if (json.isObject())
    {
        record.type = Rangers::DATRecord::NODE;
        for (const std::string& id : json.getMemberNames())
        {
            Rangers::DATRecord child;
            child.name = id;
            JSON2Record(child, json[id]);
            record.add(child);
        }
    }
    else if (json.isArray())
    {
        record.type = Rangers::DATRecord::ARRAY;
        for (const Json::Value& c : json)
        {
            Rangers::DATRecord child;
            JSON2Record(child, c);
            record.add(child);
        }
    }
    else
    {
        record.type = Rangers::DATRecord::VALUE;
        record.value = json.asString();
    }
}

void printHelp()
{
    std::cout << "OpenSR DAT<->JSON Tool" << std::endl;
    std::cout << "Usage: opensr-dat-json <command> <in> <out>" << std::endl;
    std::cout << "<command>:" << std::endl;
    std::cout << "  d2j - convert (decrypted) DAT file to JSON" << std::endl;
    std::cout << "  d2jm - convert (decrypted) DAT file to multiple JSON files. out - output dir" << std::endl;
    std::cout << "  j2d - convert JSON file to DAT file" << std::endl;
    std::cout << "  j2d - convert multiple JSON file to DAT file. in - input dir" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printHelp();
        return 0;
    }

    std::string  cmd = argv[1];
    std::ifstream inf;
    std::ofstream outf;

    if (cmd != "d2j" && cmd != "d2jm" && cmd != "j2d" && cmd != "j2dm")
    {
        std::cerr << "Invalid command: " << cmd << std::endl;
        printHelp();
        return -1;
    }

    if (cmd != "j2dm")
    {
        inf.open(argv[2], std::ios_base::in | std::ios_base::binary);
        if (!inf)
        {
            std::cerr << "Cannot open input file" << std::endl;
            return -1;
        }
    }

    if (cmd != "d2jm")
    {
        outf.open(argv[3], std::ios_base::out | std::ios_base::binary);
        if (!outf)
        {
            std::cerr << "Cannot open output file" << std::endl;
            return -1;
        }
    }

    if (cmd == "d2j")
    {
        Rangers::DATRecord datRoot = Rangers::loadDAT(inf);
        Json::Value jsonRoot;
        record2JSON(datRoot, jsonRoot);

        Json::StyledStreamWriter w;
        w.write(outf, jsonRoot);
    }
    else if (cmd == "j2d")
    {
        Json::Reader reader;
        Json::Value root;
        reader.parse(inf, root);
        Rangers::DATRecord datRoot;
        JSON2Record(datRoot, root);
        Rangers::saveDAT(outf, datRoot);
    }
    else if (cmd == "d2jm")
    {
        Rangers::DATRecord datRoot = Rangers::loadDAT(inf);
        Json::Value jsonRoot;
        record2JSON(datRoot, jsonRoot);
        Json::StyledStreamWriter w;

        for (const std::string& id : jsonRoot.getMemberNames())
        {
            outf.open(std::string(argv[3]) + "/" + id + ".json", std::ios_base::out | std::ios_base::binary);
            if (!outf)
            {
                std::cerr << "Cannot open output file: " << std::string(argv[3]) + "/" + id + ".json" << std::endl;
                return -1;
            }
            w.write(outf, jsonRoot[id]);
            outf.close();
        }
    }
    else if (cmd == "j2dm")
    {
        DIR *dp = opendir(argv[2]);
        dirent *ep;

        if (dp == 0)
        {
            std::cerr << "Cannot open input directory" << std::endl;
            return -1;
        }

        Json::Value jsonRoot(Json::objectValue);
        Json::Reader reader;

        while ((ep = readdir(dp)) != 0)
        {
            if (ep->d_type != DT_REG)
                continue;

            if (Rangers::suffix(ep->d_name) != "json")
                continue;

            std::string id = Rangers::toUTF8(Rangers::fromLocal(Rangers::basename(ep->d_name).c_str()));
            std::string file = std::string(argv[2]) + "/" + ep->d_name;

            inf.open(file, std::ios_base::in | std::ios_base::binary);
            if (!inf)
            {
                std::cerr << "Cannot open input file: " << file << std::endl;
                return -1;
            }
            reader.parse(inf, jsonRoot[id]);
            inf.close();
        }
        closedir(dp);
        Rangers::DATRecord datRoot;
        JSON2Record(datRoot, jsonRoot);
        Rangers::saveDAT(outf, datRoot);
    }

    if (inf.is_open())
        inf.close();
    if (outf.is_open())
        outf.close();

    return 0;
}

