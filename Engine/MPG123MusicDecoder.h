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

#ifndef OPENSR_MPG123MUSICDECODER_H
#define OPENSR_MPG123MUSICDECODER_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/MusicDecoder.h>

#include "3rdparty/mpg123.h"

#include <QByteArray>

class QIODevice;

namespace OpenSR
{
class MPG123MusicDecoder: public MusicDecoder
{
    Q_OBJECT
public:
    MPG123MusicDecoder(QIODevice *dev, QObject *parent = 0);
    virtual ~MPG123MusicDecoder();

    virtual bool valid() const;

    virtual QByteArray decode(int ms);

    virtual int sampleRate() const;
    virtual int channels() const;
    virtual int bps() const;
    virtual int length() const;
    virtual bool done() const;

private:
    QIODevice *m_device;
    mpg123_handle *m_mpgHandle;
    long m_rate;
    int m_channels, m_encoding;
    int m_length;
    bool m_done;

    static bool m_mpgInited;
    static bool m_mpgInitFailed;
};
}

#endif // OPENSR_MPG123MUSICDECODER_H
