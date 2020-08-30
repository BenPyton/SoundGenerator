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

#ifndef UTILS_H
#define UTILS_H

#include <Qt>
#include <QJsonValue>
#include <QJsonObject>
#include <QPointF>

class Utils
{
public:
    static qreal SmoothDamp(qreal _current,
                            qreal _target,
                            qreal& _currentVelocity,
                            qreal _smoothTime,
                            qreal _maxSpeed,
                            qreal _deltaTime);

    template<typename T>
    static T Clamp(const T& _x, T _min, T _max)
    {
        if(_min > _max)
        {
            T tmp = _min;
            _min = _max;
            _max = tmp;
        }

        T value = _x;
        if(value < _min) value = _min;
        if(value > _max) value = _max;
        return value;
    }

    static qreal MapValue(qreal _x, qreal _p00, qreal _p01, qreal _p10, qreal _p11);

    static bool CheckJsonValue(QJsonObject& _object, QString _name, QJsonValue::Type _type, int _startErrorCode);

    static void ErrorMsg(int _code, QString _msg);

    static QString GetAppVersion();

    static qreal Distance(const QPointF& _p1, const QPointF& _p2);
    static qreal DistanceSqr(const QPointF& _p1, const QPointF& _p2);
    static qreal Length(const QPointF &_p);
    static qreal LengthSqr(const QPointF &_p);

    static QString FormatTimeCode(qreal _seconds);

    static const QString AppName;
    static const QString CompanyName;
};


#endif // UTILS_H
