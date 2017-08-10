/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2017 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/SoundManager.h"
#include "SoundManager_p.h"

#include <AL/alc.h>

#include <QtEndian>
#include <QFileInfo>
#include <QDebug>
#include <QUrl>
#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>

#include "MPG123MusicDecoder.h"
#include "VorbisMusicDecoder.h"

namespace OpenSR
{
namespace
{
struct WAVFMTHeader
{
    quint32 id;
    quint32 size;
    quint16 format;
    quint16 channels;
    quint32 sampleRate;
    quint32 byteRate;
    quint16 align;
    quint16 bps;
};
}

SampleData::SampleData(): m_alID(0)
{
    alGenBuffers(1, &m_alID);
}

SampleData::~SampleData()
{
    alDeleteBuffers(1, &m_alID);
}

Sample::Sample()
{
}

Sample::Sample(QSharedPointer<SampleData> data):
    d(data)
{
}

Sample::~Sample()
{
}

ALuint Sample::openALBufferID() const
{
    if (!d)
        return 0;

    return d->m_alID;
}

SoundManager::SoundManager(QObject *parent):
    QObject(parent), d_osr_ptr(new SoundManager::SoundManagerPrivate())
{
    Q_D(SoundManager);
    d->device = alcOpenDevice(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
    d->context = alcCreateContext(d->device, NULL);
    alcMakeContextCurrent(d->context);
}

SoundManager::~SoundManager()
{
    Q_D(SoundManager);
    alcMakeContextCurrent(0);
    alcDestroyContext(d->context);
    alcCloseDevice(d->device);
}

void SoundManager::start()
{
    Q_D(SoundManager);
    ALfloat direction[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, direction);
}

QSharedPointer<SampleData> SoundManager::SoundManagerPrivate::loadWAVFile(QIODevice* d)
{
    quint32 header[3];
    d->read((char *)header, 3 * 4);

    if ((header[0] != qFromBigEndian<quint32>(0x52494646)) ||
            header[2] != qFromBigEndian<quint32>(0x57415645))
        return QSharedPointer<SampleData>();

    WAVFMTHeader fmtHeader;

    d->read((char *)&fmtHeader, sizeof(WAVFMTHeader));

    if (fmtHeader.id != qFromBigEndian<quint32>(0x666d7420))
        return QSharedPointer<SampleData>();

    if (fmtHeader.format != 0x1)
        return QSharedPointer<SampleData>();

    if (fmtHeader.channels != 1 && fmtHeader.channels != 2)
        return QSharedPointer<SampleData>();

    d->seek(d->pos() + fmtHeader.size - 16);

    d->read((char *)&header, 2 * 4);

    if (header[0] != qFromBigEndian<quint32>(0x64617461))
        return QSharedPointer<SampleData>();

    QByteArray samples = d->read(header[1]);

    QSharedPointer<SampleData> data(new SampleData());

    ALenum alFormat;

    switch (fmtHeader.bps)
    {
    case 8:
        if (fmtHeader.channels == 1)
            alFormat = AL_FORMAT_MONO8;
        else
            alFormat = AL_FORMAT_STEREO8;
        break;

    case 16:
        if (fmtHeader.channels == 1)
            alFormat = AL_FORMAT_MONO16;
        else
            alFormat = AL_FORMAT_STEREO16;
        break;

    default:
        return QSharedPointer<SampleData>();
    }

    alBufferData(data->m_alID, alFormat, samples.constData(), samples.size(), fmtHeader.sampleRate);

    return data;
}

Sample SoundManager::loadSample(const QUrl& url)
{
    Q_D(SoundManager);

    auto it = d->m_soundCache.find(url);
    if (it != d->m_soundCache.end())
        return Sample(it.value());

    /*QFileInfo fi(url.path());
    if (fi.suffix().toLower() != "wav")
    {
        qCritical() << "Unsupported sound format: " << fi.suffix();
        return Sample(QSharedPointer<SampleData>());
    }*/

    QIODevice *f = ((Engine *)qApp)->resources()->getIODevice(url, this);

    if (!f)
        return Sample(QSharedPointer<SampleData>());

    if (!f->isOpen())
    {
        delete f;
        return Sample(QSharedPointer<SampleData>());
    }

    QSharedPointer<SampleData> data = d->loadWAVFile(f);

    f->close();
    delete f;

    if (!data)
        return data;

    d->m_soundCache[url] = data;
    return Sample(data);
}

MusicDecoder *SoundManager::getMusicDecoder(const QUrl& url, QObject *parent)
{
    QString path = url.path();

    if (!QFileInfo(path).suffix().compare("mp3", Qt::CaseInsensitive) ||
            !QFileInfo(path).suffix().compare("dat", Qt::CaseInsensitive))
    {
        QIODevice *dev = ((Engine *)qApp)->resources()->getIODevice(url);
        if (!dev)
            return 0;
        return new MPG123MusicDecoder(dev, parent);
    }
    else if (!QFileInfo(path).suffix().compare("ogg", Qt::CaseInsensitive))
    {
        QIODevice *dev = ((Engine *)qApp)->resources()->getIODevice(url);
        if (!dev)
            return 0;
        return new VorbisMusicDecoder(dev, parent);
    }
    qWarning() << "Unsupported music format: " << QFileInfo(path).suffix();
    return 0;
}

}

