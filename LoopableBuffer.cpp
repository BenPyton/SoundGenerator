/*
 * Copyright 2020 Benoit Pelletier
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

#include "LoopableBuffer.h"

LoopableBuffer::LoopableBuffer(QObject* _parent)
    : QBuffer(_parent), m_loop(false)
{
}

LoopableBuffer::LoopableBuffer(QByteArray *_byteArray, QObject* _parent)
    : QBuffer(_byteArray, _parent), m_loop(false)
{
}

qint64 LoopableBuffer::readData(char *_data, qint64 _len)
{
    if(_len <= 0)
        return qint64(0);

    if(m_loop)
    {
        qint64 dataPos = 0;
        qint64 tmpLen = 0;
        while(dataPos < _len)
        {
            tmpLen = qMin(_len - dataPos, size() - pos());
            dataPos += QBuffer::readData(_data + dataPos, tmpLen);
            if(atEnd())
                reset();
        }
        return dataPos;
    }
    else
    {
        return QBuffer::readData(_data, _len);
    }
}
