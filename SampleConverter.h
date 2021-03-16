/*
 * Copyright 2021 Benoit Pelletier
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

#ifndef SAMPLECONVERTER_H
#define SAMPLECONVERTER_H

#include <QtCore>
#include <QtEndian>

class SampleConverter
{
public:

    // _sampleSize could be 8, 16, 24, 32, 48 or 64
    // (currently, it will throw NotImplementedException for 24 and 48, until I do them)
    // Assume that _output is already allocated and has a length of _sampleSize/8
    // throw ArgumentException if _sampleSize is incorrect
    static void ComputeSample(int _sampleSize, qreal _value, char*& _output);
    static qint64 GetMaxValue(int _sampleSize);

    static qint64 RecoverSample(int _sampleSize, const QByteArray* _input, int _index);

private:
    template<typename T> static void ConvertSample(qint64 _value, char*& _output);
    static void ConvertSample8(qint64 _value, char*& _output);
    static void ConvertSample24(qint64 _value, char*& _output);

    template<typename T> static qint64 ConvertSample(const QByteArray* _input, int _index);
    static qint64 ConvertSample8(const QByteArray* _input, int _index);
    static qint64 ConvertSample24(const QByteArray* _input, int _index);
};

template<typename T>
void SampleConverter::ConvertSample(qint64 _value, char*& _output)
{
    T value = static_cast<T>(_value);
    qToLittleEndian(value, _output);
}

template<typename T>
qint64 SampleConverter::ConvertSample(const QByteArray* _input, int _index)
{
    int length = sizeof(T);
    if(_input != nullptr && _index * length >= 0 && _index * length <= _input->size() - length)
    {
        return qFromLittleEndian<T>(_input->constData() + (_index * length));
    }
    return 0;
}

#endif // SAMPLECONVERTER_H
