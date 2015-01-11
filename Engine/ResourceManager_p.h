/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_RESOURCEMANAGER_P_H
#define OPENSR_RESOURCEMANAGER_P_H

#include "OpenSR/ResourceManager.h"

#include <OpenSR/libRangerQt.h>
#include <QNetworkReply>

class QFile;
class QBuffer;

namespace OpenSR
{
class ResourceManagerNAM: public QNetworkAccessManager
{
    Q_OBJECT
public:
    ResourceManagerNAM(ResourceManager *manager, QObject *parent);
    virtual ~ResourceManagerNAM();

    virtual QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice *outgoingData);

private:
    ResourceManager *m_manger;

private Q_SLOTS:
    void emitReplyFinished();
};

class ResourceReply: public QNetworkReply
{
    Q_OBJECT
protected:
    ResourceReply(const QUrl& url, QIODevice *device, QObject *parent);
    virtual ~ResourceReply();

    virtual qint64 bytesAvailable() const;
    virtual bool canReadLine() const;
    virtual bool isSequential() const;
    virtual qint64 size() const;
    virtual bool seek(qint64 pos);

    virtual void close();

public Q_SLOTS:
    virtual void abort();

protected:
    virtual qint64 readData(char * data, qint64 maxSize);

private:
    QIODevice *m_device;
    friend class ResourceManagerNAM;

private Q_SLOTS:
    void emitFinished();
};

class FSProvider: public ResourceProvider
{
public:
    FSProvider(const QString& dir);
    virtual ~FSProvider();

    virtual void load(ResourceNode& root);
    virtual QIODevice *getDevice(const ResourceNode& node, QObject *parent = 0);

private:
    void load(ResourceNode& current, const QDir& dir);
    QString m_dir;
};

class PGKResourceInfo: public ResourceInfo
{
public:
    PGKResourceInfo(PKGItem *item);
    virtual ~PGKResourceInfo();

    PKGItem *item;
};

class PKGIODevice: public QIODevice
{
public:
    PKGIODevice(const PKGItem& item, QIODevice *archiveDev, QObject *parent = 0);
    virtual ~PKGIODevice();

    virtual qint64 bytesAvailable() const;
    virtual bool canReadLine() const;
    virtual bool isSequential() const;
    virtual qint64 size() const;
    virtual bool seek(qint64 pos);

    virtual void close();

protected:
    virtual qint64 readData(char * data, qint64 maxSize);
    virtual qint64 writeData(const char * data, qint64 maxSize);

private:
    QByteArray m_data;
    QBuffer *m_buffer;
};

class PKGProvider: public ResourceProvider
{
public:
    PKGProvider(const QString& file);
    virtual ~PKGProvider();

    virtual void load(ResourceNode& root);
    virtual QIODevice *getDevice(const ResourceNode& node, QObject *parent = 0);

private:
    void load(ResourceNode& current, PKGItem* item);
    void cleanup(PKGItem *item);

    PKGItem *m_root;
    QString m_path;
};

}

#endif // OPENSR_RESOURCEMANAGER_P_H
