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

#ifndef OPENSR_SOUNDMANAGER_H
#define OPENSR_SOUNDMANAGER_H

#include <OpenSR/OpenSR.h>
#include <AL/al.h>

#include <QObject>
#include <QSharedPointer>

namespace OpenSR
{
class SoundManager;
struct SampleData;
class MusicDecoder;

class Sample
{
public:
    Sample();
    virtual ~Sample();

    ALuint openALBufferID() const;

private:
    Sample(QSharedPointer<SampleData> data);

    QSharedPointer<SampleData> d;
    friend class SoundManager;
};

class SoundManager : public QObject
{
    Q_OBJECT
    OPENSR_DECLARE_PRIVATE(SoundManager)

public:
    SoundManager(QObject *parent = 0);
    virtual ~SoundManager();

    Sample loadSample(const QUrl& url);
    void start();

    MusicDecoder *getMusicDecoder(const QUrl& url, QObject *parent = 0);

protected:
    OPENSR_DECLARE_DPOINTER(SoundManager)

private:
    Q_DISABLE_COPY(SoundManager)
};
}

#endif // OPENSR_SOUNDMANAGER_H
