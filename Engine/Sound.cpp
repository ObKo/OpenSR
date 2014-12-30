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
Sound::Sound(QObject *parent): QObject(parent), m_volume(1.0)
{
}

void Sound::play()
{
    if (!m_source.isLocalFile() && m_source.scheme() != "qrc")
        return;

    QString path;
    if (m_source.scheme() == "qrc")
        path = ":/" + m_source.path();
    else
        path = m_source.toLocalFile();

    Engine *e = static_cast<Engine*>(qApp);
    Sample sample(path, e->sound());
    sample.setVolume(m_volume);
    e->sound()->playSample(sample);
}

void Sound::setSource(const QUrl& source)
{
    m_source = source;

    if (!m_source.isLocalFile() && m_source.scheme() != "qrc")
        qWarning() << "Non-local sound is not supported";

    emit(sourceChanged());
}

QUrl Sound::source() const
{
    return m_source;
}

void Sound::setVolume(float volume)
{
    m_volume = volume;
    emit(volumeChanged());
}

float Sound::volume() const
{
    return m_volume;
}

void qmlRegisterSoundType()
{
    qmlRegisterType<Sound>("OpenSR", 1, 0, "Sound");
}
}

