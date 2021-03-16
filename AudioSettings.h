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

#ifndef AUDIOSETTINGS_H
#define AUDIOSETTINGS_H

#include <QAudioOutput>
#include <QJsonObject>

class Signal;

class AudioSettings
{
public:
    AudioSettings();

    inline qreal duration() const { return m_duration; }
    inline int sampleRate() const { return m_sampleRate; }
    inline int channelCount() const { return m_channelCount; }
    inline int sampleSize() const { return m_sampleSize; }
    inline QAudioFormat::SampleType sampleType() const { return m_sampleSize > 8 ? QAudioFormat::SignedInt : QAudioFormat::UnSignedInt; }
    inline bool isDirty() const { return m_isDirty; }

    inline void setDuration(qreal _duration) { m_duration = _duration; }
    void setSampleRate(int _frequency);
    void setChannelCount(int _channelCount);
    void setSampleSize(int _bitPerSample);
    inline void cleanDirty() { m_isDirty = false; }

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json, const int version);

private:
    qreal m_duration;
    int m_sampleRate;
    int m_channelCount;
    int m_sampleSize;
    bool m_isDirty;
};

#endif // AUDIOSETTINGS_H
