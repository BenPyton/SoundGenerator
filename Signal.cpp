/*
 * Copyright 2019 Benoit Pelletier
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
#include "utils.h"
#include <QtCore>
#include <QtMath>
#include "Components.h"
#include <QAudioRecorder>
#include "wavformat.h"

Signal::Signal(QObject *parent)
    : QObject(parent),
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

    // start audio with buffer
    m_buffer = new QBuffer(m_samples);
}

Signal::~Signal()
{
    m_buffer->close();
    delete m_audio;
    delete m_buffer;
    delete m_samples;
}

void Signal::setVolume(qreal volume)
{
    if(m_audio != nullptr)
    {
        qreal linearVolume = QAudio::convertVolume(volume,
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

qint32 Signal::getSample(int index)
{
    if(m_samples != nullptr && index * 4 >= 0 && index * 4 <= m_samples->size() - 4)
    {
        //qint32 convertedValue =
        return qFromLittleEndian<qint32>(m_samples->constData() + (index * 4));
        //m_samples->append(convertedValue, 4);
    }
    return 0;
}

void Signal::generate()
{
//    if(m_samples != nullptr)
//    {
//        delete m_samples;
//    }




    int totalSample = qRound(m_duration * m_sampleRate);
    //m_samples = new QByteArray(totalSample, NULL);
    //output.init();

    m_samples->clear();

    if(m_component != nullptr)
    {
        m_component->init();
    }

    for (int i = 0; i < totalSample; i++)
    {
        qreal x = qreal(i) / m_sampleRate;
        //qDebug() << "time: " << x;
        //qreal sample = Utils::Clamp(sin(400.0 * x), -1.0, 1.0);
        //qreal sample = qSin(400.0 * x * 2 * M_PI);
        qreal sample = m_component != nullptr ? Utils::Clamp(m_component->getOutput(x), -1., 1.) : 0.0;

        //qDebug() << "time: " << x << " | value: " << sample;
        qint32 value = qint32(qRound(INT32_MAX * sample));

        char convertedValue[4] = {0};
        qToLittleEndian(value, convertedValue);
        m_samples->append(convertedValue, 4);//QByteArray::number(convertedValue));
    }

    qDebug() << "Generated!";

    emit signalChanged();
}

void Signal::exportWAV(QString fileName)
{
    WAVFormat wav(1, 1, m_sampleRate, 32);
    wav.writeToFile(fileName, m_samples);
}

void Signal::play()
{
    if(m_buffer->isOpen())
    {
        m_audio->stop();
        m_buffer->close();
    }

    m_buffer->open(QIODevice::ReadOnly);
    m_audio->start(m_buffer);
}

void Signal::pause()
{

}


void Signal::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            m_audio->stop();
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
