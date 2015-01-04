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

#include <OpenSR/OpenSR.h>

#include <QObject>
#include <QByteArray>
#include <QSharedPointer>

namespace OpenSR
{
class SoundManagerPrivate;
class SoundManager;
class SampleData;

struct SoundData
{
    int channelCount;
    int sampleCount;
    QByteArray samples;
};

class Sample
{
public:
    Sample(const QString &name, SoundManager *manager);
    virtual ~Sample();
    //Argh, M$VC is so M$VC, it can't handle QSHDP without explicit copy constructor..
    Sample(const Sample& other);

    QSharedPointer<SoundData> data() const;
    int processed() const;
    float volume() const;
    bool done() const;

    void processSamples(int count);
    void setVolume(float volume);

private:
    QSharedDataPointer<SampleData> d;
};

class SoundManager : public QObject
{
    Q_OBJECT
    OPENSR_DECLARE_PRIVATE(SoundManager)

public:
    class DataIODev;

    SoundManager(QObject *parent = 0);
    virtual ~SoundManager();

    void start();
    QSharedPointer<SoundData> loadSound(const QString& name);
    void playSample(const Sample& sample);

protected:
    OPENSR_DECLARE_DPOINTER(SoundManager)

private:
    Q_DISABLE_COPY(SoundManager)
};
}

#endif // OPENSR_SOUNDMANAGER_H
