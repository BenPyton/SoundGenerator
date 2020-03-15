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

#ifndef LOOPABLEBUFFER_H
#define LOOPABLEBUFFER_H

#include <QBuffer>

class LoopableBuffer : public QBuffer
{
public:
    LoopableBuffer(QObject* _parent = nullptr);
    LoopableBuffer(QByteArray *_byteArray, QObject* _parent = nullptr);

    inline void setLoop(bool _loop) { m_loop = _loop; }
    inline bool loop() { return m_loop; }

protected:
    virtual qint64 readData(char* _data, qint64 _len) override;

private:
    bool m_loop;
};

#endif // LOOPABLEBUFFER_H
