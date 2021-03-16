/*
 * Copyright 2019-2021 Benoit Pelletier
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
#include "AudioSettings.h"
#include "SampleConverter.h"

Signal::Signal(QObject *_parent)
    : QObject(_parent),
      m_audioSettings(nullptr),
      m_audio(nullptr),
      m_samples(nullptr),
      m_buffer(nullptr),
      m_component(nullptr)
{
    m_audioSettings = new AudioSettings();
    m_audioSettings->setSampleRate(48000);
    m_audioSettings->setSampleSize(32);
    m_audioSettings->setChannelCount(1);
    m_audioSettings->setDuration(1.0);

    int totalSample = 0;
    m_samples = new QByteArray(totalSample, NULL);
    m_cursorSample = 0;

    updateAudioOutput(QAudioDeviceInfo::defaultOutputDevice());

    // create the buffer with the audio samples
    m_buffer = new LoopableBuffer(m_samples);
    m_buffer->open(QIODevice::ReadOnly);
}

Signal::~Signal()
{
    stop();
    m_buffer->close();
    if(m_audio != nullptr)
    {
        delete m_audio;
    }
    delete m_buffer;
    delete m_samples;
    delete m_audioSettings;
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
        return m_samples->size() / (m_audioSettings->sampleSize() / 8);
    }
    return 0;
}

qint64 Signal::getSample(int _index)
{
    return SampleConverter::RecoverSample(m_audioSettings->sampleSize(), m_samples, _index);
}

void Signal::updateAudioOutput(const QAudioDeviceInfo& _info)
{
    bool update = false;
    QAudioFormat format;
    qreal volume = 1.0;
    if(m_audio == nullptr || m_audioSettings->isDirty())
    {
        update = true;
        m_audioSettings->cleanDirty();

        format.setSampleRate(m_audioSettings->sampleRate());
        format.setChannelCount(1); // TODO: Manage multiple channels
        format.setSampleSize(m_audioSettings->sampleSize());
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(m_audioSettings->sampleType());
    }
    else
    {
        format = m_audio->format();
        volume = m_audio->volume();
    }

    if(_info != m_deviceInfo)
    {
        update = true;
        m_deviceInfo = _info;
    }

    if(update)
    {
        if(m_audio != nullptr)
        {
            if(m_audio->state() != QAudio::State::StoppedState)
            {
                qDebug() << "Stop previous audio output";
                m_audio->stop();
            }

            qDebug() << "Delete previous audio";
            delete m_audio;
            m_audio = nullptr;
        }

        m_audio = new QAudioOutput(m_deviceInfo, format);
        connect(m_audio, &QAudioOutput::stateChanged, this, &Signal::handleStateChanged);
        m_audio->setVolume(volume);
        qDebug() << "Audio Created!";

        checkOutputDevice();
    }
}

void Signal::setCursorTime(qreal _cursorTime)
{
    m_cursorSample = qRound(_cursorTime * m_audioSettings->sampleRate());
}

void Signal::generate()
{
    int sampleRate = m_audioSettings->sampleRate();
    int totalSample = qRound(m_audioSettings->duration() * sampleRate);

    m_samples->clear();

    if(m_component != nullptr)
    {
        m_component->init();
    }

    int sampleSize = m_audioSettings->sampleSize();
    int length = sampleSize/8;
    char* convertedValue = new char[static_cast<quint64>(length)] {0};

    // TODO: thread this and show a progress bar
    for (int i = 0; i < totalSample; i++)
    {
        qreal x = qreal(i) / sampleRate;
        qreal sample = m_component != nullptr ? Utils::Clamp(m_component->getOutput(x), -1., 1.) : 0.0;

        SampleConverter::ComputeSample(sampleSize, sample, convertedValue);
        m_samples->append(convertedValue, length);
    }

    delete[] convertedValue;

    qDebug() << "Generated!";

    emit signalChanged();
}

void Signal::exportWAV(QString _fileName)
{
    WAVFormat wav(1, m_audioSettings->channelCount(), m_audioSettings->sampleRate(), m_audioSettings->sampleSize());
    wav.writeToFile(_fileName, m_samples);
}

void Signal::loop(bool _enable)
{
    m_buffer->setLoop(_enable);
}

void Signal::play()
{
    if(!checkOutputDevice())
        return;

    if(m_audio->state() == QAudio::SuspendedState)
    {
        m_audio->resume();
    }
    else
    {
        stop();
        toStart();
        m_audio->start(m_buffer);
        if(m_audio->error() != QAudio::Error::NoError)
        {
            qWarning() << "Error when opening audio!!!";
            emit handleError(Error::UNKNOWN_ERROR);
        }
    }
}

void Signal::stop()
{
    if(!checkOutputDevice())
        return;

    if(m_audio->state() == QAudio::ActiveState)
    {
        m_audio->stop();
        emit stopped();
    }
}

void Signal::pause()
{
    if(!checkOutputDevice())
        return;

    if(m_audio->state() == QAudio::ActiveState)
    {
        m_audio->suspend();
    }
}

void Signal::toStart()
{
    m_buffer->reset();
    m_buffer->seek(m_cursorSample * 4);
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

bool Signal::checkOutputDevice()
{
    if(m_audio == nullptr)
    {
        qWarning() << "No audio output device, cannot play audio.";
        emit handleError(Error::NO_OUTPUT_DEVICE);
        return false;
    }

    if(!m_deviceInfo.isFormatSupported(m_audio->format()))
    {
        qWarning() << "Audio format not supported by output device, cannot play audio.";
        emit handleError(Error::AUDIO_FORMAT_NOT_SUPPORTED);
        return false;
    }
    return true;
}
