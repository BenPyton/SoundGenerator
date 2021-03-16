/*
 * Copyright 2019-2021 Benoit Pelletier
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

#include "Utils.h"
#include <QtMath>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QJsonArray>

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define BUGFIX_VERSION 5
#define JSON_VERSION 2

const QString Utils::AppName = "Sound Generator";
const QString Utils::CompanyName = "Benoit Pelletier";

qreal Utils::SmoothDamp(qreal _current,
                 qreal _target,
                 qreal& _currentVelocity,
                 qreal _smoothTime,
                 qreal _maxSpeed,
                 qreal _deltaTime)
{
    _smoothTime = qMax(qreal(0.0001), _smoothTime);
    qreal num = 2.0 / _smoothTime;
    qreal num2 = num * _deltaTime;
    qreal num3 = 1.0 / (1.0 + num2 + 0.48 * num2 * num2 + 0.235 * num2 * num2 * num2);
    qreal dist = _current - _target;
    qreal num5 = _target;
    qreal maxDist = _maxSpeed * _smoothTime;
    dist = Clamp(dist, -maxDist, maxDist);
    _target = _current - dist;
    qreal num7 = (_currentVelocity + num * dist) * _deltaTime;
    _currentVelocity = (_currentVelocity - num * num7) * num3;
    qreal num8 = _target + (dist + num7) * num3;
    if (num5 - _current > 0.0 == num8 > num5)
    {
        num8 = num5;
        _currentVelocity = (num8 - num5) / _deltaTime;
    }
    return num8;
}

qreal Utils::MapValue(qreal _x, qreal _p00, qreal _p01, qreal _p10, qreal _p11)
{
    qreal newValue = 0.;
    if (!qFuzzyCompare(_p00, _p01) && !qFuzzyCompare(_p10, _p11))
    {
        qreal max0 = _p01 - _p00;
        qreal max1 = _p11 - _p10;
        newValue = _p10 + (_x - _p00) * (max1 / max0);
    }

    return newValue;
}

int Utils::MapValue(int _x, int _p00, int _p01, int _p10, int _p11)
{
    int newValue = 0;
    if (_p00 != _p01 && _p10 != _p11)
    {
        int max0 = _p01 - _p00;
        int max1 = _p11 - _p10;
        newValue = qRound(_p10 + (_x - _p00) * max1 / static_cast<qreal>(max0));
    }

    return newValue;
}

bool Utils::CheckJsonValue(const QJsonObject &_object, QString _name, QJsonValue::Type _type, int _startErrorCode)
{
    bool success = true;
    if(!_object.contains(_name))
    {
        ErrorMsg(_startErrorCode, "No \"" + _name + "\" provided");
        success = false;
    }

    if(_object[_name].type() != _type)
    {
        ErrorMsg(_startErrorCode+1, "\"" + _name + "\" is not a number");
        success = false;
    }
    return success;
}


void Utils::ErrorMsg(int _code, QString _msg)
{
    QMessageBox::critical(qApp->activeWindow(), "Load Error", "[Error #" + QString::number(_code) + "] " + _msg);
}

QString Utils::GetAppVersion()
{
    return QString::number(MAJOR_VERSION) + "." +  QString::number(MINOR_VERSION) + "." + QString::number(BUGFIX_VERSION);
}

int Utils::GetJSONVersion()
{
    return JSON_VERSION;
}

qreal Utils::Distance(const QPointF &_p1, const QPointF &_p2)
{
    return Length(_p2 - _p1);
}

qreal Utils::DistanceSqr(const QPointF &_p1, const QPointF &_p2)
{
    return LengthSqr(_p2 - _p1);
}


qreal Utils::Length(const QPointF &_p)
{
    return qSqrt(LengthSqr(_p));
}

qreal Utils::LengthSqr(const QPointF &_p)
{
    return _p.x() * _p.x() + _p.y() * _p.y();
}

QString Utils::FormatTimeCode(qreal _seconds)
{
    int min = qFloor(_seconds / 60.0);
    int sec = qFloor(_seconds - min * 60.0);
    int millisec = qRound((_seconds - min * 60.0 - sec) * 1000);

    return QString("%1:%2:%3").arg(min).arg(sec, 2, 10, QLatin1Char('0')).arg(millisec, 3, 10, QLatin1Char('0'));
}

QString Utils::GetComponentIconPath(QString _componentName)
{
    return QString(":/icons/components/%1").arg(_componentName.toLower());
}
