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

#ifndef TIMERULER_H
#define TIMERULER_H

#include <QtWidgets>

class TimeRuler : public QWidget
{
    Q_OBJECT

public:
    TimeRuler(QWidget* _parent = nullptr);

    void setTimeWindow(qreal _startTime, qreal _endTime);
    inline qreal getStartTime() { return m_startTime; }
    inline qreal getEndTime() { return m_endTime; }
    inline qreal getCurrentTime() { return m_currentTime; }

public slots:
    virtual void paintEvent(QPaintEvent* _event) override;
    virtual void mousePressEvent(QMouseEvent* _event) override;
    virtual void mouseMoveEvent(QMouseEvent* _event) override;
    virtual void mouseReleaseEvent(QMouseEvent* _event) override;

signals:
    void onTimeSelected(qreal _time);

private:
    void updateCursor(int mousePosX);

private:
    qreal m_startTime = 0.0;
    qreal m_endTime = 0.0;
    qreal m_currentTime = 0.0;
    bool m_draggingCursor = false;
    int m_cursorOffset = 0.0;

    static const int CURSOR_HALF_WIDTH = 4;
};

#endif // TIMERULER_H
