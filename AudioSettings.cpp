/*
 * Copyright 2021 Benoit Pelletier
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

#include "AudioSettings.h"
#include "Signal.h"
#include "Utils.h"
#include <QtDebug>
#include <QJsonValue>

#define SAMPLE_RATE_LABEL "sampleRate"
#define SAMPLE_SIZE_LABEL "sampleSize"
#define CHANNEL_COUNT_LABEL "channelCount"
#define DURATION_LABEL "duration"

AudioSettings::AudioSettings()
    : m_duration(0.0), m_sampleRate(0), m_channelCount(0), m_sampleSize(0)
{
}

void AudioSettings::setSampleRate(int _sampleRate)
{
    if(m_sampleRate != _sampleRate)
    {
        m_sampleRate = _sampleRate;
        m_isDirty = true;
    }
}

void AudioSettings::setSampleSize(int _sampleSize)
{
    if(m_sampleSize != _sampleSize)
    {
        m_sampleSize = _sampleSize;
        m_isDirty = true;
    }
}

void AudioSettings::setChannelCount(int _channelCount)
{
    if(m_channelCount != _channelCount)
    {
        m_channelCount = _channelCount;
        m_isDirty = true;
    }
}

QJsonObject AudioSettings::toJson() const
{
    QJsonObject json;
    json[SAMPLE_RATE_LABEL] = m_sampleRate;
    json[SAMPLE_SIZE_LABEL] = m_sampleSize;
    json[CHANNEL_COUNT_LABEL] = m_channelCount;
    json[DURATION_LABEL] = m_duration;

    return json;
}

void AudioSettings::fromJson(const QJsonObject& json, const int version)
{
    if(Utils::CheckJsonValue(json, SAMPLE_RATE_LABEL, QJsonValue::Double, 200))
        m_sampleRate = json[SAMPLE_RATE_LABEL].toInt();
    if(Utils::CheckJsonValue(json, SAMPLE_SIZE_LABEL, QJsonValue::Double, 210))
        m_sampleSize = json[SAMPLE_SIZE_LABEL].toInt();
    if(Utils::CheckJsonValue(json, CHANNEL_COUNT_LABEL, QJsonValue::Double, 220))
        m_channelCount = json[CHANNEL_COUNT_LABEL].toInt();
    if(version >= 2)
    {
        if(Utils::CheckJsonValue(json, DURATION_LABEL, QJsonValue::Double, 230))
        m_duration = json[DURATION_LABEL].toDouble();
    }
    m_isDirty = true;
}
