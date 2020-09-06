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

    QBrush brush(Qt::BrushStyle::SolidPattern);
    brush.setColor(QColor(255, 255, 0));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    int cursorRadius = 2;
    int height = size().height() - 1;
    int x = qRound(Utils::MapValue(m_currentTime, m_startTime, m_endTime, 0.0, static_cast<qreal>(size().width()))) - CURSOR_HALF_WIDTH;

    QPainterPath cursor;
    cursor.moveTo(x, cursorRadius);
    cursor.arcTo(x, 0, 2 * cursorRadius, 2 * cursorRadius, 180, -90);
    cursor.arcTo(x + CURSOR_HALF_WIDTH * 2 - 2 * cursorRadius, 0, 2 * cursorRadius, 2 * cursorRadius, 90, -90);
    cursor.lineTo(x + 2 * CURSOR_HALF_WIDTH, height - CURSOR_HALF_WIDTH);
    cursor.lineTo(x + CURSOR_HALF_WIDTH, height);
    cursor.lineTo(x, height - CURSOR_HALF_WIDTH);
    cursor.closeSubpath();

    painter.fillPath(cursor, brush);
}

void TimeRuler::mousePressEvent(QMouseEvent *_event)
{
    if(_event->button() == Qt::MouseButton::LeftButton)
    {
        m_draggingCursor = true;
        int cursorPos = qRound(Utils::MapValue(m_currentTime, m_startTime, m_endTime, 0.0, static_cast<qreal>(size().width())));
        int mousePos = _event->pos().x();
        m_cursorOffset = qAbs(cursorPos - mousePos) < CURSOR_HALF_WIDTH ? cursorPos - mousePos : 0;
        updateCursor(mousePos);
    }
}

void TimeRuler::mouseMoveEvent(QMouseEvent *_event)
{
    if(m_draggingCursor)
    {
        updateCursor(_event->pos().x());
    }
}

void TimeRuler::mouseReleaseEvent(QMouseEvent *_event)
{
    Q_UNUSED(_event)

    if(_event->button() == Qt::MouseButton::LeftButton)
    {
        m_draggingCursor = false;
    }
}

void TimeRuler::updateCursor(int _mousePosX)
{
    qreal value = static_cast<qreal>(_mousePosX + m_cursorOffset);
    m_currentTime = Utils::MapValue(value, 0.0, static_cast<qreal>(size().width()), m_startTime, m_endTime);
    update();
    emit onTimeSelected(m_currentTime);
}
