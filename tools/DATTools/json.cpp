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
            json[Rangers::toUTF8(c.name)] = child;
        }
    }
    else if (record.type == Rangers::DATRecord::VALUE)
    {
        json = Rangers::toUTF8(record.value);
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
            child.name = Rangers::fromUTF8(id.c_str());
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
        record.value = Rangers::fromUTF8(json.asString().c_str());
    }
}

void printHelp()
{
    std::cout << "OpenSR DAT<->JSON Tool" << std::endl;
    std::cout << "Usage: opensr-dat-json <command> <in_file> <out_file>" << std::endl;
    std::cout << "<command>:" << std::endl;
    std::cout << "  d2j - convert (decrypted) DAT file to JSON" << std::endl;
    std::cout << "  j2d - convert JSON file to DAT file" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printHelp();
        return 0;
    }

    std::string  cmd = argv[1];

    if (cmd != "d2j" && cmd != "j2d")
    {
        std::cerr << "Invalid command: " << cmd << std::endl;
        printHelp();
        return -1;
    }
    std::ifstream inf(argv[2], std::ios_base::in | std::ios_base::binary);
    if (!inf)
    {
        std::cerr << "Cannot open input file" << std::endl;
        return -1;
    }
    std::ofstream outf(argv[3], std::ios_base::out | std::ios_base::binary);
    if (!outf)
    {
        std::cerr << "Cannot open output file" << std::endl;
        return -1;
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

    inf.close();
    outf.close();

    return 0;
}

