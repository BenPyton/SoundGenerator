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

#ifndef WAVEFORMCHUNK_H
#define WAVEFORMCHUNK_H

#include <QPixmap>

class Signal;

class WaveFormChunk : public QPixmap
{
public: // overloaded methods
    WaveFormChunk();
    WaveFormChunk(int width, int height);

public: // my methods
    void updateWave(Signal* signal);

    void setColor(const QColor& color);
    void setIndexRange(int startIndex, int nbIndex);

    inline int startIndex() { return m_startIndex; }

protected:
    //QColor getColorAtIndex(int index);

private:
    QColor m_waveColor;

    int m_startIndex;
    int m_nbIndex;
    int m_nbTotalSample;
};

#endif // WAVEFORMCHUNK_H
