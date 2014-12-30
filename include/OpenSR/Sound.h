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

#ifndef OPENSR_SOUND_H
#define OPENSR_SOUND_H

#include <QObject>
#include <QUrl>

namespace OpenSR
{
class Sound: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    Sound(QObject *parent = 0);

    QUrl source() const;
    float volume() const;

    void setSource(const QUrl& source);
    void setVolume(float volume);

public Q_SLOTS:
    void play();

Q_SIGNALS:
    void sourceChanged();
    void volumeChanged();

private:
    QUrl m_source;
    float m_volume;
};
}

#endif // OPENSR_SOUND_H
