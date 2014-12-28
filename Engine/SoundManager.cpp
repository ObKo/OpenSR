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

#include "OpenSR/SoundManager.h"

#include <QIODevice>
#include <QtEndian>
#include <QBuffer>
#include <QDebug>

#ifdef OPENSR_USE_SRC
#include <samplerate.h>
#endif

namespace OpenSR
{
namespace
{
struct WAVFMTHeader
{
    quint32 id;
    quint32 size;
    quint16 format;
    quint16 channels;
    quint32 sampleRate;
    quint32 byteRate;
    quint16 align;
    quint16 bps;
};
const int SAMPLE_RATE = 48000;
const int SAMPLE_SIZE = 4;
const int CHANNEL_COUNT = 2;

const int DEV_BUFFER_MS_SIZE = 100;
const int DEV_BUFFER_SAMPLE_COUNT = SAMPLE_RATE / 1000 * DEV_BUFFER_MS_SIZE;
const int DEV_BUFFER_SIZE = DEV_BUFFER_SAMPLE_COUNT * CHANNEL_COUNT * SAMPLE_SIZE;

const int BUFFER_MS_SIZE = 50;
const int BUFFER_SAMPLE_COUNT = SAMPLE_RATE / 1000 * BUFFER_MS_SIZE;
const int BUFFER_SIZE = BUFFER_SAMPLE_COUNT * CHANNEL_COUNT * SAMPLE_SIZE;
}

class SoundManager::DataIODev: public QIODevice
{
public:
    DataIODev(SoundManager *parent): QIODevice(parent)
    {
        buffer = new float[BUFFER_SAMPLE_COUNT * CHANNEL_COUNT];
        size = BUFFER_SIZE;
        offset = 0;
        manager = parent;
    }

    int size;
    int offset;
    float *buffer;
    SoundManager *manager;

protected:
    virtual qint64 readData(char *data, qint64 maxSize)
    {
        qint64 readed = 0;
        while (readed < maxSize)
        {
            int curSize = size - offset;
            if (curSize > maxSize - readed)
            {
                memcpy(data + readed, ((char *)buffer) + offset, maxSize - readed);
                offset += maxSize - readed;
                readed = maxSize;
            }
            else
            {
                memcpy(data + readed, ((char *)buffer) + offset, curSize);
                readed += curSize;
                manager->refreshBuffer();
                offset = 0;
            }
        }
    }

    virtual qint64 writeData(const char *data, qint64 maxSize)
    {
        return -1;
    }
};

SoundManager::SoundManager(QObject *parent): QObject(parent)
{
    QAudioFormat f;
    f.setByteOrder(QAudioFormat::LittleEndian);
    f.setChannelCount(CHANNEL_COUNT);
    f.setSampleRate(SAMPLE_RATE);
    f.setSampleSize(SAMPLE_SIZE * 8);
    f.setSampleType(QAudioFormat::Float);
    m_outputDevice = new QAudioOutput(f, this);
    m_outputDevice->setBufferSize(DEV_BUFFER_SIZE);

    m_bufferDev = new DataIODev(this);
    m_bufferDev->open(QIODevice::ReadOnly);
}

SoundManager::~SoundManager()
{
}

void SoundManager::start()
{
    m_outputDevice->start(m_bufferDev);
}

void SoundManager::addSound(QSharedPointer<SoundData> sound)
{
    mix(sound, 0);

    if (sound->sampleCount > BUFFER_SAMPLE_COUNT)
    {
        m_currentSounds.append(qMakePair(sound, BUFFER_SAMPLE_COUNT));
    }
}

QSharedPointer<SoundData> SoundManager::loadWAVFile(QIODevice* d)
{
    quint32 header[3];
    d->read((char *)header, 3 * 4);

    if ((header[0] != qFromBigEndian<quint32>(0x52494646)) ||
            header[2] != qFromBigEndian<quint32>(0x57415645))
        return QSharedPointer<SoundData>();

    WAVFMTHeader fmtHeader;

    d->read((char *)&fmtHeader, sizeof(WAVFMTHeader));

    if (fmtHeader.id != qFromBigEndian<quint32>(0x666d7420))
        return QSharedPointer<SoundData>();

    if ((fmtHeader.format != 0x1) && (fmtHeader.format != 0x3))
        return QSharedPointer<SoundData>();

    d->seek(d->pos() + fmtHeader.size - 16);

    d->read((char *)&header, 2 * 4);

    if (header[0] != qFromBigEndian<quint32>(0x64617461))
        return QSharedPointer<SoundData>();


    QByteArray samples = d->read(header[1]);

    SoundData *data = new SoundData();

    data->channelCount = fmtHeader.channels;
    data->sampleCount = samples.size() / (fmtHeader.bps / 8) / fmtHeader.channels;

    if (data->channelCount != 1 && data->channelCount != 2)
        return QSharedPointer<SoundData>();

    QByteArray floatData;

    if (fmtHeader.format == 0x1)
    {
        floatData.resize(data->sampleCount * SAMPLE_SIZE * data->channelCount);
        int totalSamples = data->sampleCount * data->channelCount;
        float *res = (float*)floatData.data();
        const char *src = samples.constData();
        if (fmtHeader.bps == 16)
        {
            for (int i = 0; i < data->sampleCount * data->channelCount; i++)
                res[i] = ((qint16*)src)[i] / 32768.0f;
        }
        else if (fmtHeader.bps == 8)
        {
            for (int i = 0; i < data->sampleCount * data->channelCount; i++)
                res[i] = ((uchar *)src)[i] / 255.0f * 2.0f - 1.0f;
        }
    }
    else if (fmtHeader.format == 0x3)
    {
        floatData = samples;
    }
    else
        return QSharedPointer<SoundData>();

    if (fmtHeader.sampleRate != SAMPLE_RATE)
        data->samples = resample(floatData, data->sampleCount, fmtHeader.sampleRate, SAMPLE_RATE, data->channelCount);
    else
        data->samples = floatData;

    return QSharedPointer<SoundData>(data);
}

#ifdef OPENSR_USE_SRC
QByteArray SoundManager::resample(const QByteArray& floatData, int &sampleCount, int srcRate, int destRate, int channels)
{
    if (srcRate == destRate)
        return floatData;

    double ratio = double(destRate) / srcRate;
    int newSampleCount = sampleCount * ratio;
    QByteArray result(newSampleCount * SAMPLE_SIZE * channels, 0);

    SRC_DATA convData;
    convData.data_in = (float*)floatData.constData();
    convData.data_out = (float*)result.data();
    convData.end_of_input = 1;
    convData.input_frames = sampleCount;
    convData.output_frames = newSampleCount;
    convData.src_ratio = ratio;
    src_simple(&convData, SRC_SINC_MEDIUM_QUALITY, channels);
    sampleCount = newSampleCount;

    return result;
}
#else
//Using simple linear resampler
QByteArray SoundManager::resample(const QByteArray& floatData, int &sampleCount, int srcRate, int destRate, int channels)
{
    if (srcRate == destRate)
        return floatData;

    double ratio = double(destRate) / srcRate;
    int newSampleCount = sampleCount * ratio;
    QByteArray result(newSampleCount * SAMPLE_SIZE * channels, 0);

    const float *src = (float *)floatData.constData();
    float *dest = (float *)result.data();

    for (int i = 0; i < newSampleCount; i++)
    {
        int si = floor(i / ratio);
        double frac = i / ratio - si;
        if (si >= sampleCount - 1)
            break;

        dest[i] = src[si] + (src[si + 1] - src[si]) * frac;
    }

    sampleCount = newSampleCount;

    return result;
}
#endif

void SoundManager::refreshBuffer()
{
    memset(m_bufferDev->buffer, 0, BUFFER_SIZE);

    auto m_currentSoundsCopy = m_currentSounds;
    m_currentSounds.clear();

    for (const QPair<QSharedPointer<SoundData>, int> &p : m_currentSoundsCopy)
    {
        mix(p.first, p.second);

        if ((p.first->sampleCount - p.second) > BUFFER_SAMPLE_COUNT)
        {
            m_currentSounds.append(qMakePair(p.first, p.second + BUFFER_SAMPLE_COUNT));
        }
    }
}

void SoundManager::mix(QSharedPointer< SoundData > sound, int from)
{
    float *inBuf = 0;
    const float *samples = (const float *)sound->samples.constData() + from;
    int sCount = qMin(sound->sampleCount - from, BUFFER_SAMPLE_COUNT);

    if (sound->channelCount == 1)
    {
        for (int i = 0; i < sCount; i++)
        {
            m_bufferDev->buffer[i * 2] += samples[i];
            m_bufferDev->buffer[i * 2 + 1] += samples[i];
        }
    }
    else if (sound->channelCount == 2)
    {
        for (int i = 0; i < sCount; i++)
        {
            m_bufferDev->buffer[i * 2] += samples[i * 2];
            m_bufferDev->buffer[i * 2 + 1] += samples[i * 2 + 1];
        }

    }

}

}

