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

#include "crypt.h"

#include <iostream>
#include <OpenSR/libRangerQt.h>
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QFile>

void printHelp()
{
    std::cout << "OpenSR DAT File Converter" << std::endl;
    std::cout << "Usage: opensr-dat-convert <command> <in_file> <out_file>" << std::endl;
    std::cout << "<command>:" << std::endl;
    std::cout << "  d - decrypt file" << std::endl;
    std::cout << "  x - decompress file" << std::endl;
    std::cout << "  dx - decrypt & decompress file" << std::endl;
    std::cout << "  e - encrypt file" << std::endl;
    std::cout << "  c - compress file" << std::endl;
    std::cout << "  ce - compress & encrypt file" << std::endl;
    std::cout << "  h<command> - assume that input file is CacheData.dat" << std::endl;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    if (app.arguments().count() != 4)
    {
        printHelp();
        return 0;
    }

    QString cmd = app.arguments()[1];

    bool isCache = cmd[0] == 'h';
    if (isCache)
        cmd.remove(0, 1);

    if (cmd != "d" && cmd != "x" && cmd != "dx" &&
            cmd != "c" && cmd != "e" && cmd != "ce")
    {
        qCritical() << "Invalid command: " << cmd;
        printHelp();
        return -1;
    }
    QFile inf(app.arguments()[2]);
    inf.open(QIODevice::ReadOnly);
    if (!inf.isOpen())
    {
        qCritical() << "Cannot open input file";
        return -1;
    }
    QFile outf(app.arguments()[3]);
    outf.open(QIODevice::WriteOnly);
    if (!outf.isOpen())
    {
        qCritical() << "Cannot open output file";
        return -1;
    }

    if (cmd == "d")
    {
        uint32_t crc;
        uint32_t key;

        inf.read((char*)&crc, 4);
        inf.read((char*)&key, 4);

        QByteArray dat = inf.readAll();

        OpenSR::DAT::decrypt(dat, key, isCache);
        if (crc != OpenSR::DAT::crc32(dat))
        {
            qCritical() << "CRC Error";
            return -1;
        }
        outf.write(dat);
    }
    else if (cmd == "x")
    {
        uint32_t sig;
        inf.peek((char*)&sig, 4);
        if (sig != OpenSR::ZL01_SIGNATURE)
        {
            qCritical() << "Invalid input file";
            return -1;
        }
        QByteArray input = inf.readAll();
        QByteArray dat = OpenSR::unpackZL(input);
        if (dat.isEmpty())
        {
            qCritical() << "Cannot unpack data";
            return -1;
        }
        outf.write(dat);
    }
    else if (cmd == "dx")
    {
        uint32_t crc;
        uint32_t key;

        inf.read((char*)&crc, 4);
        inf.read((char*)&key, 4);

        QByteArray dat = inf.readAll();

        OpenSR::DAT::decrypt(dat, key, isCache);
        if (crc != OpenSR::DAT::crc32(dat))
        {
            qCritical() << "CRC Error";
            return -1;
        }
        if (*((const uint32_t*)dat.constData()) != OpenSR::ZL01_SIGNATURE)
        {
            qCritical() << "Invalid input file";
            return -1;
        }
        dat = OpenSR::unpackZL(dat);
        if (dat.isEmpty())
        {
            qCritical() << "Cannot unpack data";
            return -1;
        }
        outf.write(dat);
    }
    else if (cmd == "e")
    {
        QByteArray dat = inf.readAll();
        uint32_t crc = OpenSR::DAT::crc32(dat);
        uint32_t key = OpenSR::DAT::genKey();
        OpenSR::DAT::encrypt(dat, key, isCache);
        outf.write((char*)&crc, 4);
        outf.write((char*)&key, 4);
        outf.write(dat);
    }
    else if (cmd == "c")
    {
        QByteArray result = OpenSR::packZL01(inf.readAll());
        if (result.isEmpty())
        {
            qCritical() << "Cannot pack data";
            return -1;
        }
        outf.write(result);
    }
    else if (cmd == "ce")
    {
        QByteArray dat = OpenSR::packZL01(inf.readAll());
        if (dat.isEmpty())
        {
            qCritical() << "Cannot pack data";
            return -1;
        }
        uint32_t crc = OpenSR::DAT::crc32(dat);
        uint32_t key = OpenSR::DAT::genKey();
        OpenSR::DAT::encrypt(dat, key, isCache);
        outf.write((char*)&crc, 4);
        outf.write((char*)&key, 4);
        outf.write(dat);
    }

    inf.close();
    outf.close();

    return 0;
}
