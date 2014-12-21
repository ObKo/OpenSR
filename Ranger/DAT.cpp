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
#include <QBuffer>
#include <QVariantList>

namespace OpenSR
{
namespace
{
QString readWideString(QIODevice *dev)
{
    wchar_t c = 0;
    QString result;

    do
    {
        dev->read((char*)&c, 2);
        if (c != 0)
            result.append(QChar(c));
    }
    while (c != 0);

    return result;
}

void writeWideString(QIODevice *dev, const QString& value)
{
    dev->write((char*)value.utf16(), (value.length() + 1) * 2);
}
}

//TODO: Optimize
void writeDATTree(QIODevice *dev, const QVariant& node, const QString& name)
{
    if (node.type() == QVariant::Map)
    {
        QVariantMap m = node.toMap();
        uint32_t count = 0;
        uint8_t isTree = 0;
        for (const QVariant& c : m)
        {
            if (c.type() == QVariant::Map || c.type() == QVariant::List)
            {
                isTree = 1;
                if (c.type() == QVariant::List)
                    count += c.toList().count() - 1;
            }
        }
        count += m.count();
        dev->write((char*)&isTree, 1);
        dev->write((char*)&count, 4);

        if (isTree)
        {
            QVariantMap::const_iterator end = m.end();
            for (QVariantMap::const_iterator i = m.begin(); i != end; ++i)
            {
                uint32_t index = 0;
                uint32_t arrayCount = 1;
                uint8_t isText = 1;
                if (i.value().type() == QVariant::Map || i.value().type() == QVariant::List)
                    isText = 2;
                if (i.value().type() != QVariant::List)
                {
                    dev->write((char*)&index, 4);
                    dev->write((char*)&arrayCount, 4);
                    dev->write((char*)&isText, 1);
                    writeWideString(dev, i.key());
                }
                writeDATTree(dev, i.value(), i.key());
            }
        }
        else
        {
            QVariantMap::const_iterator end = m.end();
            for (QVariantMap::const_iterator i = m.begin(); i != end; ++i)
            {
                uint8_t isText = 1;
                dev->write((char*)&isText, 1);
                writeWideString(dev, i.key());
                writeDATTree(dev, i.value(), i.key());
            }
        }
    }
    else if (node.type() == QVariant::List)
    {
        QVariantList l = node.toList();
        uint32_t count = l.count();
        uint32_t i = 0;
        for (const QVariant& c : l)
        {
            uint32_t index = i;
            uint32_t arrayCount = 0;
            uint8_t isText = 1;
            if (i == 0)
                arrayCount = count;
            if (c.type() == QVariant::Map || c.type() == QVariant::List)
                isText = 2;
            dev->write((char*)&index, 4);
            dev->write((char*)&arrayCount, 4);
            dev->write((char*)&isText, 1);
            writeWideString(dev, name);
            //FIXME: Possible bug here, in case of array of array
            writeDATTree(dev, c, name);
            i++;
        }
    }
    else
    {
        writeWideString(dev, node.toString());
    }
}

void readDATTree(QIODevice *dev, QVariantMap* parent, bool isCache)
{
    uint8_t isTree = 0;
    uint32_t count;

    if (!isCache)
        dev->read((char*)&isTree, 1);
    dev->read((char*)&count, 4);

    if (isTree)
    {
        QVariantList l;
        QString currentName;
        uint32_t currentCount = 0;
        for (int i = 0; i < count; i++)
        {
            uint32_t index, arrayCount;
            uint8_t isText;

            dev->read((char*)&index, 4);
            dev->read((char*)&arrayCount, 4);
            dev->read((char*)&isText, 1);
            QString name = readWideString(dev);
            QVariant value;

            if (isText == 2)
            {
                QVariantMap r;
                readDATTree(dev, &r, isCache);
                value = QVariant(r);
            }
            else
            {
                value = QVariant(readWideString(dev));
            }

            if ((arrayCount > 1) && (index == 0))
            {
                currentCount = arrayCount;
                currentName = name;
                l.push_back(value);
            }
            else if ((arrayCount == 0) && (index < currentCount - 1))
            {
                l.push_back(value);
            }
            else if ((arrayCount == 0) && (index == currentCount - 1))
            {
                l.push_back(value);

                parent->insert(currentName, QVariant(l));

                currentCount = 0;
                l.clear();
            }
            else
            {
                parent->insert(name, value);
            }
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            uint8_t isText = 0;
            dev->read((char*)&isText, 1);

            QString name = readWideString(dev);
            QVariant value;

            if (isText == 2)
            {
                QVariantMap newMap;
                readDATTree(dev, &newMap, isCache);
                value = newMap;
            }
            else
            {
                value = QVariant(readWideString(dev));
            }
            parent->insert(name, value);
        }
    }
}

QVariantMap loadDAT(QIODevice *dev, bool isCache)
{
    QVariantMap root;
    uint32_t sig;
    dev->peek((char *)&sig, 4);

    if (sig == 0x31304c5a)
    {
        QByteArray whole = dev->readAll();
        QByteArray datData = unpackZL(whole);
        QBuffer dat(&datData);
        dat.open(QIODevice::ReadOnly);
        readDATTree(&dat, &root, isCache);
        dat.close();
    }
    else
        readDATTree(dev, &root, isCache);

    return root;
}

void saveDAT(QIODevice *dev, const QVariant& root)
{
    writeDATTree(dev, root, QString());
}
}
