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

#include "OpenSR/Music.h"

#include <OpenSR/Engine.h>
#include <OpenSR/SoundManager.h>
#include <OpenSR/MusicDecoder.h>
#include <QtQml>
#include <QDebug>

namespace OpenSR
{
namespace
{
const int BUFFER_SIZE_MS = 200;
const int BUFFER_COUNT = 5;
}
Music::Music(QObject *parent): QObject(parent),
    m_volume(1.0), m_alSource(0), m_freeBuffers(BUFFER_COUNT), m_decoder(0)
{
    alGenSources((ALuint)1, &m_alSource);

    alSourcef(m_alSource, AL_PITCH, 1);
    alSourcef(m_alSource, AL_GAIN, 1);
    alSource3f(m_alSource, AL_POSITION, 0, 0, 0);
    alSource3f(m_alSource, AL_VELOCITY, 0, 0, 0);
    alSourcei(m_alSource, AL_LOOPING, AL_FALSE);

    m_alBuffers = new ALuint[BUFFER_COUNT];
    alGenBuffers(BUFFER_COUNT, m_alBuffers);

    m_timer.setSingleShot(true);
    m_timer.setInterval(BUFFER_SIZE_MS);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(queue()));
}

Music::~Music()
{
    alDeleteSources(1, &m_alSource);
    alDeleteBuffers(BUFFER_COUNT, m_alBuffers);
    delete[] m_alBuffers;
}

void Music::setSource(const QUrl& source)
{
    m_source = source;
    resetDecoding();
}

QUrl Music::source() const
{
    return m_source;
}

void Music::setVolume(float volume)
{
    m_volume = volume;
    alSourcef(m_alSource, AL_GAIN, m_volume);
    emit(volumeChanged());
}

float Music::volume() const
{
    return m_volume;
}

void Music::pause()
{
    alSourcePause(m_alSource);
    m_timer.stop();
}

void Music::stop()
{

    alSourceStop(m_alSource);
    m_timer.stop();
}


void Music::play()
{
    m_timer.start();
    alSourcePlay(m_alSource);
}

void Music::queue()
{
    ALint doneBuffers = 0;
    alGetSourcei(m_alSource, AL_BUFFERS_PROCESSED, &doneBuffers);

    if (!doneBuffers && m_freeBuffers != BUFFER_COUNT)
    {
        m_timer.start();
        return;
    }

    if (!doneBuffers)
        return;

    m_freeBuffers += doneBuffers;
    ALuint doneBuf[BUFFER_COUNT];
    alSourceUnqueueBuffers(m_alSource, doneBuffers, doneBuf);
    for (int i = 0; i < doneBuffers; i++)
    {
        if (m_decoder->done())
            break;

        QByteArray decodedData = m_decoder->decode(BUFFER_SIZE_MS);

        if (decodedData.isEmpty())
            break;

        ALenum format;

        if (m_decoder->channels() == 1)
            format = m_decoder->bps() == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO8;
        else if (m_decoder->channels() == 2)
            format = m_decoder->bps() == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
        else
            break;

        alBufferData(doneBuf[i], format, decodedData.constData(), decodedData.size(), m_decoder->sampleRate());
        alSourceQueueBuffers(m_alSource, 1, &doneBuf[i]);

        m_freeBuffers--;
    }

    if (m_freeBuffers != BUFFER_COUNT)
        m_timer.start();

    if (m_freeBuffers == BUFFER_COUNT && m_decoder->done())
        emit(finished());
}

void Music::resetDecoding()
{
    stop();
    if (m_decoder)
    {
        delete m_decoder;
        m_decoder = 0;
    }
    m_decoder = ((Engine *)qApp)->sound()->getMusicDecoder(m_source, this);

    if (!m_decoder)
        return;

    if (!m_decoder->valid())
    {
        delete m_decoder;
        m_decoder = 0;
        return;
    }

    m_freeBuffers = BUFFER_COUNT;

    for (int i = 0; i < BUFFER_COUNT; i++)
    {
        if (m_decoder->done())
            break;

        QByteArray decodedData = m_decoder->decode(BUFFER_SIZE_MS);

        if (decodedData.isEmpty())
            break;

        ALenum format;

        if (m_decoder->channels() == 1)
            format = m_decoder->bps() == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO8;
        else if (m_decoder->channels() == 2)
            format = m_decoder->bps() == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
        else
            break;

        alBufferData(m_alBuffers[i], format, decodedData.constData(), decodedData.size(), m_decoder->sampleRate());
        alSourceQueueBuffers(m_alSource, 1, &m_alBuffers[i]);

        m_freeBuffers--;
    }
}
}

