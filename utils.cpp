/*
 * Copyright 2019 Benoit Pelletier
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

#include "utils.h"
#include <QtMath>

qreal Utils::SmoothDamp(qreal current,
                 qreal target,
                 qreal& currentVelocity,
                 qreal smoothTime,
                 qreal maxSpeed,
                 qreal deltaTime)
{
    smoothTime = qMax(qreal(0.0001), smoothTime);
    qreal num = 2.0 / smoothTime;
    qreal num2 = num * deltaTime;
    qreal num3 = 1.0 / (1.0 + num2 + 0.48 * num2 * num2 + 0.235 * num2 * num2 * num2);
    qreal dist = current - target;
    qreal num5 = target;
    qreal maxDist = maxSpeed * smoothTime;
    dist = Clamp(dist, -maxDist, maxDist);
    target = current - dist;
    qreal num7 = (currentVelocity + num * dist) * deltaTime;
    currentVelocity = (currentVelocity - num * num7) * num3;
    qreal num8 = target + (dist + num7) * num3;
    if (num5 - current > 0.0 == num8 > num5)
    {
        num8 = num5;
        currentVelocity = (num8 - num5) / deltaTime;
    }
    return num8;
}

qreal Utils::MapValue(qreal x, qreal p00, qreal p01, qreal p10, qreal p11)
{
    qreal newValue = 0.;
    if (!qFuzzyCompare(p00, p01) && !qFuzzyCompare(p10, p11))
    {
        qreal max0 = p01 - p00;
        qreal max1 = p11 - p10;
        newValue = p10 + (x - p00) * (max1 / max0);
    }

    return newValue;
}