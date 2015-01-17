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

#include "VorbisMusicDecoder.h"

#include <QIODevice>
#include <QDebug>
#include <QLibrary>

namespace OpenSR
{
namespace
{
#ifdef VORBISFILE_LIBRARY_NAME
static const QString VORBISFILE_NAME = MPG123_LIBRARY_NAME;
#else
static const QString VORBISFILE_NAME = "vorbisfile";
#endif

size_t iodevRead(void *buf, size_t size, size_t nmemb, void *datasource)
{
    if (!datasource)
        return 0;

    QIODevice *dev = static_cast<QIODevice *>(datasource);
    return dev->read((char*)buf, size * nmemb) / size;
}

int iodevSeek(void *datasource, ogg_int64_t offset, int whence)
{
    if (!datasource)
        return -1;

    QIODevice *dev = static_cast<QIODevice *>(datasource);
    bool result;
    switch (whence)
    {
    case SEEK_CUR:
        result = dev->seek(dev->pos() + offset);
        break;
    case SEEK_SET:
        result = dev->seek(offset);
        break;
    case SEEK_END:
        result = dev->seek(dev->size() + offset);
        break;
    }
    return result ? dev->pos() : -1;
}

long iodevTell(void *datasource)
{
    if (!datasource)
        return -1;

    QIODevice *dev = static_cast<QIODevice *>(datasource);
    return dev->pos();
}

ov_callbacks iodevCallbacks = {iodevRead, iodevSeek, 0, iodevTell};

typedef int (*ov_open_callbacks_f)(void *datasource, OggVorbis_File *vf, const char *initial, long ibytes, ov_callbacks callbacks);
typedef int (*ov_clear_f)(OggVorbis_File *vf);
typedef vorbis_info* (*ov_info_f)(OggVorbis_File *vf, int link);
typedef ogg_int64_t (*ov_pcm_total_f)(OggVorbis_File *vf, int i);
typedef long (*ov_read_f)(OggVorbis_File *vf, char *buffer, int length, int bigendianp, int word, int sgned, int *bitstream);

ov_open_callbacks_f ov_open_callbacks = 0;
ov_clear_f ov_clear = 0;
ov_info_f ov_info = 0;
ov_pcm_total_f ov_pcm_total = 0;
ov_read_f ov_read = 0;
}
bool VorbisMusicDecoder::m_vfInited = false;
bool VorbisMusicDecoder::m_vfInitFailed = false;

VorbisMusicDecoder::VorbisMusicDecoder(QIODevice* dev, QObject *parent): MusicDecoder(parent),
    m_device(dev), m_vfFile(0), m_vfInfo(0), m_done(false)
{
    if (!ov_open_callbacks && !m_vfInitFailed)
    {
        QLibrary vfLib(VORBISFILE_NAME);
        if (vfLib.load())
        {
            ov_open_callbacks = (ov_open_callbacks_f)vfLib.resolve("ov_open_callbacks");
            ov_clear = (ov_clear_f)vfLib.resolve("ov_clear");
            ov_info = (ov_info_f)vfLib.resolve("ov_info");
            ov_pcm_total = (ov_pcm_total_f)vfLib.resolve("ov_pcm_total");
            ov_read = (ov_read_f)vfLib.resolve("ov_read");
        }
        else
        {
            qWarning().noquote() << QString("%1, mp3 music will be unavailable.").arg(vfLib.errorString());
            m_vfInitFailed = true;
        }
    }


    if (m_device)
    {
        m_device->setParent(this);

        m_vfFile = new OggVorbis_File();

        if (ov_open_callbacks(m_device, m_vfFile, 0, 0, iodevCallbacks) < 0)
        {
            qDebug() << "Cannot open Vorbis decoder.";
        }
        else
        {
            m_vfInfo = ov_info(m_vfFile, -1);
            m_length = ov_pcm_total(m_vfFile, -1);
        }
    }
}

VorbisMusicDecoder::~VorbisMusicDecoder()
{
    if (m_device)
    {
        m_device->close();
        delete m_device;
    }
    if (m_vfFile)
    {
        ov_clear(m_vfFile);
        delete m_vfFile;
    }
}

int VorbisMusicDecoder::bps() const
{
    return 16;
}

int VorbisMusicDecoder::channels() const
{
    if (!m_vfInfo)
        return 0;
    return m_vfInfo->channels;
}

bool VorbisMusicDecoder::valid() const
{
    return (m_device != 0) && (m_vfFile != 0) && (m_vfInfo != 0);
}

QByteArray VorbisMusicDecoder::decode(int ms)
{
    if (!m_vfFile || !m_vfInfo)
        return QByteArray();

    int needed = 2 * (quint64)m_vfInfo->rate * m_vfInfo->channels * ms / 1000;

    QByteArray result(needed, 0);

    while (needed > 0)
    {
        int current_section;
        long res = ov_read(m_vfFile, result.data() + (result.size() - needed), needed, 0, 2, 1, &current_section);

        if (res < 0)
            break;

        needed -= res;

        if (res == 0)
        {
            m_done = true;
            break;
        }
    }

    if (needed)
        result.resize(result.size() - needed);

    return result;
}

int VorbisMusicDecoder::sampleRate() const
{
    if (!m_vfInfo)
        return 0;
    return m_vfInfo->rate;
}

int VorbisMusicDecoder::length() const
{
    return m_length;
}

bool VorbisMusicDecoder::done() const
{
    return m_done;
}

}
