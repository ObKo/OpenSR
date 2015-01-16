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

#ifndef OPENSR_MUSICDECODER_H
#define OPENSR_MUSICDECODER_H

#include <OpenSR/OpenSR.h>

#include <QObject>
#include <QByteArray>

class QIODevice;

namespace OpenSR
{
class MusicDecoder: public QObject
{
    Q_OBJECT
public:
    MusicDecoder(QObject *parent = 0): QObject(parent) {}
    virtual ~MusicDecoder() {}

    virtual bool valid() const = 0;

    virtual QByteArray decode(int ms) = 0;

    virtual int sampleRate() const = 0;
    virtual int channels() const = 0;
    virtual int bps() const = 0;
    virtual bool done() const = 0;
};
}

#endif // OPENSR_MUSICDECODER_H
