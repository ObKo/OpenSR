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

#include "MPG123MusicDecoder.h"

#include <QIODevice>
#include <QDebug>
#include <QLibrary>

namespace OpenSR
{
namespace
{
#ifdef MPG123_LIBRARY_NAME
static const QString MPG123_NAME = MPG123_LIBRARY_NAME;
#else
static const QString MPG123_NAME = "mpg123";
#endif

ssize_t iodevRead(void *ptr, void *buf, size_t size)
{
    if (!ptr)
        return -1;

    QIODevice *dev = static_cast<QIODevice *>(ptr);
    return dev->read((char*)buf, size);
}

off_t iodevSeek(void *ptr, off_t offset, int whence)
{
    if (!ptr)
        return -1;

    QIODevice *dev = static_cast<QIODevice *>(ptr);
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

typedef int (*mpg123_init_f)(void);
typedef mpg123_handle* (*mpg123_new_f)(const char*, int*);
typedef void (*mpg123_delete_f)(mpg123_handle*);
typedef const char* (*mpg123_plain_strerror_f)(int);
typedef int (*mpg123_format_none_f)(mpg123_handle*);
typedef int (*mpg123_format_f)(mpg123_handle*, long, int, int);
typedef int (*mpg123_getformat_f)(mpg123_handle*, long*, int*, int*);
typedef int (*mpg123_open_handle_f)(mpg123_handle*, void*);
typedef int (*mpg123_close_f)(mpg123_handle*);
typedef int (*mpg123_read_f)(mpg123_handle*, unsigned char*, size_t, size_t*);
typedef off_t (*mpg123_length_f)(mpg123_handle*);
typedef int (*mpg123_replace_reader_handle_f)(mpg123_handle*, ssize_t (*)(void *, void *, size_t), off_t (*)(void *, off_t, int), void (*)(void*));

mpg123_init_f mpg123_init = 0;
mpg123_new_f mpg123_new = 0;
mpg123_delete_f mpg123_delete = 0;
mpg123_plain_strerror_f mpg123_plain_strerror = 0;
mpg123_format_none_f mpg123_format_none = 0;
mpg123_format_f mpg123_format = 0;
mpg123_getformat_f mpg123_getformat = 0;
mpg123_open_handle_f mpg123_open_handle = 0;
mpg123_close_f mpg123_close = 0;
mpg123_read_f mpg123_read = 0;
mpg123_length_f mpg123_length = 0;
mpg123_replace_reader_handle_f mpg123_replace_reader_handle = 0;
}
bool MPG123MusicDecoder::m_mpgInited = false;
bool MPG123MusicDecoder::m_mpgInitFailed = false;

MPG123MusicDecoder::MPG123MusicDecoder(QIODevice* dev, QObject *parent): MusicDecoder(parent),
    m_device(dev), m_done(false), m_mpgHandle(0)
{
    if (!mpg123_init && !m_mpgInitFailed)
    {
        QLibrary mpg123Lib(MPG123_NAME);
        if (mpg123Lib.load())
        {
            mpg123_init = (mpg123_init_f)mpg123Lib.resolve("mpg123_init");
            mpg123_new = (mpg123_new_f)mpg123Lib.resolve("mpg123_new");
            mpg123_delete = (mpg123_delete_f)mpg123Lib.resolve("mpg123_delete");
            mpg123_plain_strerror = (mpg123_plain_strerror_f)mpg123Lib.resolve("mpg123_plain_strerror");
            mpg123_format_none = (mpg123_format_none_f)mpg123Lib.resolve("mpg123_format_none");
            mpg123_format = (mpg123_format_f)mpg123Lib.resolve("mpg123_format");
            mpg123_getformat = (mpg123_getformat_f)mpg123Lib.resolve("mpg123_getformat");
            mpg123_open_handle = (mpg123_open_handle_f)mpg123Lib.resolve("mpg123_open_handle");
            mpg123_close = (mpg123_close_f)mpg123Lib.resolve("mpg123_close");
            mpg123_read = (mpg123_read_f)mpg123Lib.resolve("mpg123_read");
            mpg123_length = (mpg123_length_f)mpg123Lib.resolve("mpg123_length");
            mpg123_replace_reader_handle = (mpg123_replace_reader_handle_f)mpg123Lib.resolve("mpg123_replace_reader_handle");
        }
        else
        {
            qWarning().noquote() << QString("%1, mp3 music will be unavailable.").arg(mpg123Lib.errorString());
            m_mpgInitFailed = true;
        }
    }
    if (!m_mpgInited && !m_mpgInitFailed)
    {
        int err = MPG123_OK;
        err = mpg123_init();
        if (err == MPG123_OK)
        {
            m_mpgInited = true;
        }
        else
        {
            qWarning().noquote() << QString("Cannot init mpg123 library: %1, mp3 music will be unavailable.").arg(mpg123_plain_strerror(err));
            m_mpgInitFailed = true;
        }
    }

    if (m_device)
        m_device->setParent(this);

    if (m_mpgInited)
    {
        int err = MPG123_OK;

        m_mpgHandle = mpg123_new(0, &err);
        if (err == MPG123_OK)
        {
            mpg123_replace_reader_handle(m_mpgHandle, iodevRead, iodevSeek, 0);
            err = mpg123_open_handle(m_mpgHandle, dev);
        }
        if (err == MPG123_OK)
            err = mpg123_getformat(m_mpgHandle, &m_rate, &m_channels, &m_encoding);

        if (err == MPG123_OK)
            err = mpg123_format_none(m_mpgHandle);

        if (err == MPG123_OK)
            err = mpg123_format(m_mpgHandle, m_rate, m_channels, m_encoding);

        if (err == MPG123_OK)
            m_length = mpg123_length(m_mpgHandle);

        if (err != MPG123_OK)
        {
            qWarning().noquote() << QString("Cannot init mpg123 decoder: %1").arg(mpg123_plain_strerror(err));
            if (m_mpgHandle)
            {
                mpg123_close(m_mpgHandle);
                mpg123_delete(m_mpgHandle);
                m_mpgHandle = 0;
            }
        }
    }
}

MPG123MusicDecoder::~MPG123MusicDecoder()
{
    if (m_device)
    {
        m_device->close();
        delete m_device;
    }
    if (m_mpgHandle)
    {
        mpg123_close(m_mpgHandle);
        mpg123_delete(m_mpgHandle);
    }
}

int MPG123MusicDecoder::bps() const
{
    switch (m_encoding)
    {
    case MPG123_ENC_SIGNED_8:
        return 8;
    case MPG123_ENC_SIGNED_16:
        return 16;
    default:
        return 0;
    }
}

int MPG123MusicDecoder::channels() const
{
    return m_channels;
}

bool MPG123MusicDecoder::valid() const
{
    return m_mpgHandle != 0;
}

QByteArray MPG123MusicDecoder::decode(int ms)
{
    if (!m_mpgHandle)
        return QByteArray();

    quint32 size = (bps() / 8) * m_rate * ms * m_channels / 1000;

    QByteArray result(size, 0);
    int err;
    size_t decoded;

    err = mpg123_read(m_mpgHandle, (unsigned char*)result.data(), result.size(), &decoded);
    if (err != MPG123_OK && err != MPG123_DONE)
        qWarning().noquote() << QString("mpg123 decoding error: %1").arg(mpg123_plain_strerror(err));

    if (err == MPG123_DONE)
        m_done = true;

    if (decoded != size)
        result.resize(decoded);

    return result;
}

int MPG123MusicDecoder::sampleRate() const
{
    return m_rate;
}

int MPG123MusicDecoder::length() const
{
    return m_length;
}

bool MPG123MusicDecoder::done() const
{
    return m_done;
}

}
