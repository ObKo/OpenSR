/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2015 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_SOUND_MANAGER_P_H
#define OPENSR_SOUND_MANAGER_P_H

#include "OpenSR/SoundManager.h"

#include <QByteArray>
#include <QSharedPointer>
#include <QAudioOutput>
#include <QList>
#include <QMap>
#include <QSharedData>

namespace OpenSR
{
class SampleData: public QSharedData
{
public:
    SampleData();
    virtual ~SampleData();

    QSharedPointer<SoundData> samples;
    int processed;
    float volume;
};

class SoundManagerPrivate
{
public:
    class DataIODev: public QIODevice
    {
    public:
        DataIODev(SoundManagerPrivate *p, SoundManager *parent);

        int size;
        int offset;
        float *buffer;
        SoundManagerPrivate *manager;

    protected:
        virtual qint64 readData(char *data, qint64 maxSize);
        virtual qint64 writeData(const char *data, qint64 maxSize);
    };

    void refreshBuffer();
    void mixSample(Sample& sound);
    QByteArray resample(const QByteArray& floatData, int &sampleCount, int srcRate, int destRate, int channels);
    QSharedPointer<SoundData> loadWAVFile(QIODevice *d);

    QAudioOutput *m_outputDevice;
    DataIODev *m_bufferDev;

    QList<Sample> m_currentSamples;
    QMap<QString, QSharedPointer<SoundData> > m_soundCache;
};
}

#endif // OPENSR_SOUND_MANAGER_P_H
