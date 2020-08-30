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

#include <QWidget>

class TimeRuler : public QWidget
{
    Q_OBJECT

public:
    TimeRuler(QWidget* _parent = nullptr);

    void setTimeWindow(qreal _startTime, qreal _endTime);
    inline qreal getStartTime() { return m_startTime; }
    inline qreal getEndTime() { return m_endTime; }

public slots:
    virtual void paintEvent(QPaintEvent* _event) override;

signals:
    void onTimeSelected(qreal _time);

private:
    qreal m_startTime = 0.0;
    qreal m_endTime = 0.0;
};

#endif // TIMERULER_H
