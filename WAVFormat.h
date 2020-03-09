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

#ifndef WAVFORMAT_H
#define WAVFORMAT_H

#include <QString>

class WAVFormat
{
public:
    WAVFormat();
    WAVFormat(qint16 audioCodec, qint16 channelCount, qint32 frequency, qint16 bitsPerSample);

    qint16 audioCodec() { return m_audioCodec; }
    void setAudioCodec(qint16 codec) { m_audioCodec = codec; }
    qint16 channelCount() { return m_channelCount; }
    void setChannelCount(qint16 count) { m_channelCount = count; }
    qint32 frequency() { return m_frequency; }
    void setFrequency(qint32 frequency) { m_frequency = frequency; }
    qint16 bitsPerSample() { return m_bitsPerSample; }
    void steBitsPerSample(qint16 nbBits) { m_bitsPerSample = nbBits; }

    bool writeToFile(QString fileName, const QByteArray* data);

private:
    qint16 m_audioCodec;
    qint16 m_channelCount;
    qint32 m_frequency;
    qint16 m_bitsPerSample;
};

#endif // WAVFORMAT_H
