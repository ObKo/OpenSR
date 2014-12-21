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

#include <OpenSR/libRangerQt.h>

#include <QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDirIterator>
#include <iostream>

void printHelp()
{
    std::cout << "OpenSR DAT<->JSON Tool" << std::endl;
    std::cout << "Usage: opensr-dat-json <command> <in> <out>" << std::endl;
    std::cout << "<command>:" << std::endl;
    std::cout << "  d2j - convert (decrypted) DAT file to JSON" << std::endl;
    std::cout << "  d2jc - convert (decrypted) CacheData.dat file to JSON" << std::endl;
    std::cout << "  d2jm - convert (decrypted) DAT file to multiple JSON files. out - output dir" << std::endl;
    std::cout << "  j2d - convert JSON file to DAT file" << std::endl;
    std::cout << "  j2dm - convert multiple JSON file to DAT file. in - input dir" << std::endl;
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
    QFile inf(app.arguments()[2]);
    QFile outf(app.arguments()[3]);

    if (cmd != "d2j" && cmd != "d2jm" && cmd != "j2d" && cmd != "j2dm" && cmd != "d2jc")
    {
        qCritical() << "Invalid command: " << cmd;
        printHelp();
        return -1;
    }

    if (cmd != "j2dm")
    {
        inf.open(QIODevice::ReadOnly);
        if (!inf.isOpen())
        {
            qCritical() << "Cannot open input file";
            return -1;
        }
    }

    if (cmd != "d2jm")
    {
        outf.open(QIODevice::WriteOnly);
        if (!outf.isOpen())
        {
            qCritical() << "Cannot open output file";
            return -1;
        }
    }

    if (cmd == "d2j")
    {
        QVariantMap root = OpenSR::loadDAT(&inf, false);
        QJsonDocument doc;
        doc.setObject(QJsonObject::fromVariantMap(root));
        outf.write(doc.toJson());
    }
    else if (cmd == "d2jc")
    {
        QVariantMap root = OpenSR::loadDAT(&inf, true);
        QJsonDocument doc;
        doc.setObject(QJsonObject::fromVariantMap(root));
        outf.write(doc.toJson());
    }
    else if (cmd == "j2d")
    {
        QJsonParseError err;
        QVariant root = QJsonDocument::fromJson(inf.readAll(), &err).object().toVariantMap();
        if (err.error)
        {
            qCritical() << "Cannot parse JSON";
            return -1;
        }
        OpenSR::saveDAT(&outf, root);
    }
    else if (cmd == "d2jm")
    {
        QVariantMap root = OpenSR::loadDAT(&inf, false);

        QVariantMap::const_iterator end = root.end();
        for (QVariantMap::const_iterator i = root.begin(); i != end; ++i)
        {
            QJsonDocument doc;
            QFile of(QString("%1/%2.json").arg(app.arguments()[3], i.key()));
            of.open(QIODevice::WriteOnly);
            if (!of.isOpen())
            {
                qCritical() << "Cannot open output file: " << of.fileName();
                return -1;
            }
            doc.setObject(QJsonObject::fromVariantMap(i.value().toMap()));
            of.write(doc.toJson());
            of.close();
        }
    }
    else if (cmd == "j2dm")
    {
        QDirIterator dir(app.arguments()[2]);
        QVariantMap root;

        while (dir.hasNext())
        {
            QFileInfo f(dir.next());
            if (f.suffix() != "json")
                continue;

            QFile jf(f.canonicalPath());
            jf.open(QIODevice::ReadOnly);

            QJsonParseError err;
            QVariantMap fdata = QJsonDocument::fromJson(jf.readAll(), &err).object().toVariantMap();
            if (err.error)
            {
                qCritical() << "Cannot parse JSON file " << f.canonicalPath() << ":" << err.errorString();
                continue;
            }
            root.insert(f.baseName(), fdata);
        }
        OpenSR::saveDAT(&outf, root);
    }

    if (inf.isOpen())
        inf.close();
    if (outf.isOpen())
        outf.close();

    return 0;
}

