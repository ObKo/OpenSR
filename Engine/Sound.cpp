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

#include "OpenSR/Sound.h"

#include <OpenSR/Engine.h>
#include <OpenSR/SoundManager.h>
#include <QtQml>
#include <QDebug>

namespace OpenSR
{
Sound::Sound(QObject *parent): QObject(parent), m_volume(1.0), m_alSource(0)
{
    alGenSources((ALuint)1, &m_alSource);

    alSourcef(m_alSource, AL_PITCH, 1);
    alSourcef(m_alSource, AL_GAIN, 1);
    alSource3f(m_alSource, AL_POSITION, 0, 0, 0);
    alSource3f(m_alSource, AL_VELOCITY, 0, 0, 0);
    alSourcei(m_alSource, AL_LOOPING, AL_FALSE);
}

Sound::~Sound()
{
    alDeleteSources(1, &m_alSource);
}

void Sound::play()
{
    alSourcePlay(m_alSource);
}

void Sound::setSource(const QUrl& source)
{
    m_source = source;

    if (!source.isLocalFile() && source.scheme().compare("qrc", Qt::CaseInsensitive) &&
            source.scheme().compare("res", Qt::CaseInsensitive) &&
            source.scheme().compare("dat", Qt::CaseInsensitive))
        qWarning() << "Non-local sound is not supported";


    m_sample = ((Engine *)qApp)->sound()->loadSample(source);

    alSourcei(m_alSource, AL_BUFFER, m_sample.openALBufferID());

    emit(sourceChanged());
}

QUrl Sound::source() const
{
    return m_source;
}

void Sound::setVolume(float volume)
{
    m_volume = volume;
    alSourcef(m_alSource, AL_GAIN, m_volume);
    emit(volumeChanged());
}

float Sound::volume() const
{
    return m_volume;
}
}

