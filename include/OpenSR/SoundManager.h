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

#ifndef OPENSR_SOUNDMANAGER_H
#define OPENSR_SOUNDMANAGER_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QString>

namespace OpenSR
{
struct SoundData
{
    int channelCount;
    int sampleCount;
    QByteArray samples;
};

struct SampleData: public QSharedData
{
public:
    SampleData();

    QSharedPointer<SoundData> samples;
    int processed;
    float volume;
};

class SoundManager;
class Sample
{
public:
    Sample(const QString &name, SoundManager *manager);

    QSharedPointer<SoundData> data() const;
    int processed() const;
    float volume() const;
    bool done() const;

    void processSamples(int count);
    float setVolume(float volume);

private:
    QSharedDataPointer<SampleData> d;
};

class SoundManager : public QObject
{
    Q_OBJECT
public:
    class DataIODev;

    SoundManager(QObject *parent = 0);
    virtual ~SoundManager();

    void start();
    QSharedPointer<SoundData> loadSound(const QString& name);
    void playSample(const Sample& sample);

private:
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

#endif // OPENSR_SOUNDMANAGER_H
