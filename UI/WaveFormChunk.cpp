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

#include "WaveFormChunk.h"
#include <QPainter>
#include <QtMath>
#include <QtCore>
#include "Signal.h"

WaveFormChunk::WaveFormChunk()
    : QPixmap()
{
}

WaveFormChunk::WaveFormChunk(int width, int height)
    : QPixmap(width, height)
{
}

void WaveFormChunk::updateWave(Signal* signal)
{
    QPainter painter(this);
    QColor color = QColor(35, 35, 35, 255);

    // clear the chunk
    painter.setPen(QPen(color, 0));
    painter.setBrush(color);
    painter.drawRect(0, 0, width(), height());

    if(signal != nullptr)
    {
        //qDebug() << "There is a signal";
        m_nbTotalSample = signal->sampleCount();

        int mid = qRound(height() / 2.);

        if(m_nbIndex < width()) // less index than pixel -> draw lines between each index
        {
            QPainterPath path;

            qreal nbPixelPerIndex = qreal(width()) / m_nbIndex;

            qint32 value = signal->getSample(m_startIndex);
            path.moveTo(0, qRound(value / qreal(INT32_MAX) * mid + mid));

            for(int i = 1; i < m_nbIndex + 1; i++)
            {
                qint32 value = signal->getSample(m_startIndex + i);
                path.lineTo(i * nbPixelPerIndex, qRound(value / qreal(INT32_MAX) * mid + mid));
            }

            painter.setPen(QPen(QColor(200, 200, 200), 1));
            painter.drawPath(path);
        }
        else // more index than pixel -> get min and max in each pixel then draw a vertical line per pixel
        {
            qreal nbIndexPerPixel = m_nbIndex / qreal(width());
            int prevIndex = 0;

            for(int i = 0; i < width(); i++)
            {
                int nbIndexInThisPixel = qRound((i + 1) * nbIndexPerPixel) - prevIndex;
                qint32 min = INT32_MAX;
                qint32 max = INT32_MIN;

                for(int k = 0; k < nbIndexInThisPixel + 1; k++)
                {
                    qint32 y = signal->getSample(m_startIndex + prevIndex + k);
                    if(y < min) min = y;
                    if(y > max) max = y;
                }

                painter.setPen(QPen(QColor(200, 200, 200), 1));
                painter.drawLine(i, qRound(min / qreal(INT32_MAX) * mid + mid),
                                 i, qRound(max / qreal(INT32_MAX) * mid + mid));

                prevIndex += nbIndexInThisPixel;
            }
        }

        /*painter.setPen(QPen(QColor(200, 0, 0), 1));
        painter.drawLine(0, 0,
                         0, height());*/
    }
    else
    {
        qDebug() << "No signal";
    }

}

void WaveFormChunk::setColor(const QColor &color)
{
    m_waveColor = color;
}

void WaveFormChunk::setIndexRange(int startIndex, int nbIndex)
{
    m_startIndex = startIndex;
    m_nbIndex = nbIndex;
}

//QColor WaveFormChunk::getColorAtIndex(int index)
//{
//    qreal sampleStep = 255.0 / 100.0;

//    if(index > m_nbTotalSample)
//        return QColor(0, 0, 0);

//    index %= 600;
//    if(index < 0)
//    {
//        return QColor(255, 255, 255);
//    }
//    else if(index < 100)
//    {
//        return QColor(255, qFloor(index * sampleStep), 0);
//    }
//    else if (index < 200)
//    {
//        return QColor(qFloor((100-(index-100)) * sampleStep), 255, 0);
//    }
//    else if (index < 300)
//    {
//        return QColor(0, 255, qFloor((index-200) * sampleStep));
//    }
//    else if (index < 400)
//    {
//        return QColor(0, qFloor((100 - (index-300)) * sampleStep), 255);
//    }
//    else if (index < 500)
//    {
//        return QColor(qFloor((index-400) * sampleStep), 0, 255);
//    }
//    else if (index < 600)
//    {
//        return QColor(255, 0, qFloor((100 - (index-500)) * sampleStep));
//    }
//    else
//    {
//        return QColor(0, 0, 0);
//    }
//}
