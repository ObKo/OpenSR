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

#ifndef OPENSR_VORBISMUSICDECODER_H
#define OPENSR_VORBISMUSICDECODER_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/MusicDecoder.h>

#include "3rdparty/vorbisfile.h"

#include <QByteArray>

class QIODevice;

namespace OpenSR
{
class VorbisMusicDecoder: public MusicDecoder
{
public:
    VorbisMusicDecoder(QIODevice *dev, QObject *parent = 0);
    virtual ~VorbisMusicDecoder();

    virtual bool valid() const;

    virtual QByteArray decode(int ms);

    virtual int sampleRate() const;
    virtual int channels() const;
    virtual int bps() const;
    virtual int length() const;
    virtual bool done() const;

private:
    QIODevice *m_device;
    OggVorbis_File *m_vfFile;
    vorbis_info *m_vfInfo;
    int m_length;
    bool m_done;

    static bool m_vfInited;
    static bool m_vfInitFailed;
};
}

#endif // OPENSR_VORBISMUSICDECODER_H
