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

#ifndef UTILS_H
#define UTILS_H

#include <Qt>

class Utils
{
public:
    static qreal SmoothDamp(qreal current,
                            qreal target,
                            qreal& currentVelocity,
                            qreal smoothTime,
                            qreal maxSpeed,
                            qreal deltaTime);

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

    static qreal MapValue(qreal x, qreal p00, qreal p01, qreal p10, qreal p11);
};


#endif // UTILS_H
