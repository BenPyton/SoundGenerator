/*
 * Copyright 2019-2020 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "Signal.h"
#include "Utils.h"
#include <QtCore>
#include <QtMath>
#include "Components.h"
#include <QAudioRecorder>
#include "WAVFormat.h"
#include "LoopableBuffer.h"

Signal::Signal(QObject *_parent)
    : QObject(_parent),
      m_duration(1.0),
      m_audio(nullptr),
      m_samples(nullptr),
      m_buffer(nullptr),
      m_component(nullptr)
{
    m_sampleRate = 48000;

    int totalSample = 1 * m_sampleRate;
    m_samples = new QByteArray(totalSample, NULL);

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(m_sampleRate);
    format.setChannelCount(1);
    format.setSampleSize(32);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);


    QList<QAudioDeviceInfo> deviceList = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(QAudioDeviceInfo device : deviceList)
    {
        //qDebug() << "Device: " << device.deviceName();
                 //<< "| samples rates: " << device.supportedSampleRates()
                 //<< "| samples size: " << device.supportedSampleSizes()
                 //<< "| codecs: " << device.supportedCodecs();
                 //<< " | channel count: " << device.supportedChannelCounts()
                 //<< " | sample types: " << device.supportedSampleTypes()
                 //<< " | byte orders: " << device.supportedByteOrders();
    }

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    /*qDebug() << "Default Device: " << info.deviceName()
            << "| samples rates: " << info.supportedSampleRates()
            << " | channel count: " << info.supportedChannelCounts()
            << "| samples size: " << info.supportedSampleSizes()
            << "| codecs: " << info.supportedCodecs()
            << " | byte orders: " << info.supportedByteOrders()
            << " | sample types: " << info.supportedSampleTypes();*/
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_audio = new QAudioOutput(format);
    connect(m_audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    m_audio->setVolume(0.2);

    // create the buffer with the audio samples
    m_buffer = new LoopableBuffer(m_samples);
    m_buffer->open(QIODevice::ReadOnly);
}

Signal::~Signal()
{
    stop();
    m_buffer->close();
    delete m_audio;
    delete m_buffer;
    delete m_samples;
}

void Signal::setVolume(qreal _volume)
{
    if(m_audio != nullptr)
    {
        qreal linearVolume = QAudio::convertVolume(_volume,
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);
        m_audio->setVolume(linearVolume);
    }
}

qreal Signal::volume()
{
    if(m_audio != nullptr)
    {
        return m_audio->volume();
    }
    return 0.;
}

int Signal::sampleCount()
{
    if(m_samples != nullptr)
    {
        return m_samples->size() / 4;
    }
    return 0;
}

qint32 Signal::getSample(int _index)
{
    if(m_samples != nullptr && _index * 4 >= 0 && _index * 4 <= m_samples->size() - 4)
    {
        return qFromLittleEndian<qint32>(m_samples->constData() + (_index * 4));
    }
    return 0;
}

void Signal::generate()
{
    int totalSample = qRound(m_duration * m_sampleRate);

    m_samples->clear();

    if(m_component != nullptr)
    {
        m_component->init();
    }

    for (int i = 0; i < totalSample; i++)
    {
        qreal x = qreal(i) / m_sampleRate;
        qreal sample = m_component != nullptr ? Utils::Clamp(m_component->getOutput(x), -1., 1.) : 0.0;

        //qDebug() << "time: " << x << " | value: " << sample;
        qint32 value = qint32(qRound(INT32_MAX * sample));

        char convertedValue[4] = {0};
        qToLittleEndian(value, convertedValue);
        m_samples->append(convertedValue, 4);
    }

    qDebug() << "Generated!";

    emit signalChanged();
}

void Signal::exportWAV(QString _fileName)
{
    WAVFormat wav(1, 1, m_sampleRate, 32);
    wav.writeToFile(_fileName, m_samples);
}

void Signal::loop(bool _enable)
{
    m_buffer->setLoop(_enable);
}

void Signal::play()
{
    if(m_audio->state() == QAudio::SuspendedState)
    {
        m_audio->resume();
    }
    else
    {
        stop();
        toStart();
        m_audio->start(m_buffer);
    }
}

void Signal::stop()
{
    if(m_audio->state() == QAudio::ActiveState)
    {
        m_audio->stop();
        emit stopped();
    }
}

void Signal::pause()
{
    if(m_audio->state() == QAudio::ActiveState)
    {
        m_audio->suspend();
    }
}

void Signal::toStart()
{
    m_buffer->reset();
}

void Signal::toEnd()
{
    m_buffer->seek(qMax(qint64(0), m_buffer->size() - 1));
}


void Signal::handleStateChanged(QAudio::State _newState)
{
    switch (_newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            m_audio->stop();
            emit stopped();
            break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (m_audio->error() != QAudio::NoError) {
                // Error handling
                qWarning() << "Error when playing audio.";
            }
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}
