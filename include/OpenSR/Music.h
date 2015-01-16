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

#ifndef OPENSR_MUSIC_H
#define OPENSR_MUSIC_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/SoundManager.h>
#include <QObject>
#include <QUrl>
#include <QTimer>

namespace OpenSR
{
class MusicDecoder;
class ENGINE_API Music: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    Music(QObject *parent = 0);
    virtual ~Music();

    QUrl source() const;
    float volume() const;

    void setSource(const QUrl& source);
    void setVolume(float volume);

public Q_SLOTS:
    void play();
    void pause();
    void stop();

Q_SIGNALS:
    void sourceChanged();
    void volumeChanged();
    void finished();

private Q_SLOTS:
    void queue();

private:
    void resetDecoding();

    QUrl m_source;
    ALuint m_alSource;
    ALuint *m_alBuffers;
    float m_volume;
    MusicDecoder *m_decoder;
    QTimer m_timer;
    int m_freeBuffers;
};
}

#endif // OPENSR_MUSIC_H
