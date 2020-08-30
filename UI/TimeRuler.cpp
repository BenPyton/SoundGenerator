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

#include "TimeRuler.h"
#include <QPainter>
#include <qmath.h>
#include "Utils.h"
#include <math.h>

TimeRuler::TimeRuler(QWidget *_parent)
    : QWidget(_parent)
{
    m_startTime = 0.0;
    m_endTime = 1.0;
}

void TimeRuler::setTimeWindow(qreal _startTime, qreal _endTime)
{
    m_startTime = _startTime;
    m_endTime = _endTime;

    update();
}

void TimeRuler::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event);

    QPen pen;
    pen.setColor(QColor(200, 200, 200));
    pen.setStyle(Qt::PenStyle::SolidLine);

    QPainter painter(this);
    painter.setPen(pen);

    qreal pixelPerSecond = size().width() / (m_endTime - m_startTime);
    qreal minDistBetweenTimecodes = 200.0;
    qreal timeInterval = 1.0 / qPow(2, qFloor(log2(pixelPerSecond / minDistBetweenTimecodes)));
    qreal pixelInterval = timeInterval * pixelPerSecond;
    qreal firstTimeCode = qFloor(m_startTime / timeInterval) * timeInterval;
    int pixelOffset = qRound((firstTimeCode - m_startTime) * pixelPerSecond);
    int nbLine = qCeil(size().width() / pixelInterval) + 1;
    int nbSubdiv = 10;

    QVector<QLineF> lines(nbLine * nbSubdiv);
    QRectF textRect;
    QPointF pos;
    qreal minY = 15.0;
    qreal maxY = static_cast<qreal>(size().height());

    // main divisions with timecodes
    for(int i = 0; i < nbLine; ++i)
    {
        pos.setX(static_cast<qreal>(qRound(i * pixelInterval + pixelOffset)));
        textRect.moveCenter(pos);
        painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextDontClip
                         , Utils::FormatTimeCode(firstTimeCode + i * timeInterval));

        lines[i * nbSubdiv].setLine(pos.x(), minY, pos.x(), maxY);
    }

    // sub division (N subdivisions, so N-1 lines in each division)
    for(int i = 0; i < nbLine * nbSubdiv; ++i)
    {
        if(i % nbSubdiv != 0)
        {
            qreal x = static_cast<qreal>(qRound(i * pixelInterval / static_cast<qreal>(nbSubdiv) + pixelOffset));
            lines[i].setLine(x, minY + 5, x, maxY);
        }
    }

    painter.drawLines(lines);
}
