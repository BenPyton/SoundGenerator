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

#include "WAVFormat.h"
#include <QDataStream>
#include <QFile>

WAVFormat::WAVFormat()
    : WAVFormat(1, 1, 48000, 16)
{

}

WAVFormat::WAVFormat(qint16 audioCodec, qint16 channelCount, qint32 frequency, qint16 bitsPerSample)
    : m_audioCodec(audioCodec),
      m_channelCount(channelCount),
      m_frequency(frequency),
      m_bitsPerSample(bitsPerSample)
{

}

bool WAVFormat::writeToFile(QString fileName, const QByteArray* data)
{
    bool success = true;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    qint16 blocSize = m_channelCount * (m_bitsPerSample/8);

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

                                                    // Header block
    stream.writeRawData("RIFF", 4);                 // RIFF constant (4 bytes)
    stream << (data->size() + 36);                  // file size minus 8 bytes (4 bytes)
    stream.writeRawData("WAVE", 4);                 // WAVE format (4 bytes)

                                                    // Format block
    stream.writeRawData("fmt ", 4);                 // bloc id (4 bytes)
    stream << 16;                                   // bloc size (4 bytes)
    stream << m_audioCodec;                         // audio format eg. 1 for PCM (2 bytes)
    stream << m_channelCount;                       // 1 to 6 (2 bytes)
    stream << m_frequency;                          // in hertz (4 bytes)
    stream << m_frequency * qint32(blocSize);       // BytePerSec: frequency * BytePerBloc (2 bytes)
    stream << blocSize;                             // BytePerBloc: nbChannel * bitsPerSample/8 (2 bytes)
    stream << m_bitsPerSample;                      // 8, 16 or 24 (2 bytes)

                                                    // data block
    stream.writeRawData("data", 4);                 // bloc id (4 bytes)
    stream << data->size();                         // data size (4 bytes)
    stream << *data;                                // data


    file.close();

    return success;
}

