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

#include "SampleConverter.h"
#include "Exceptions.h"

#define SAMPLESIZE_8 UINT8_MAX
#define SAMPLESIZE_16 INT16_MAX
//#define SAMPLESIZE_24 8388607i32 // TODO: endianness conversion + sizeof
#define SAMPLESIZE_32 INT32_MAX

void SampleConverter::ComputeSample(int _sampleSize, qreal _value, char*& _output)
{
    qint64 value = qRound64(GetMaxValue(_sampleSize) * _value);
    qint64 uvalue = qRound64(GetMaxValue(_sampleSize) * ((_value + 1.0) / 2.0));

    switch(_sampleSize)
    {
    case 8:
        ConvertSample8(uvalue, _output);
        break;
    case 16:
        ConvertSample<qint16>(value, _output);
        break;
    case 24:
        ConvertSample24(value, _output);
        break;
    case 32:
        ConvertSample<qint32>(value, _output);
        break;
    default:
        throw ArgumentException("_sampleSize must only be 8, 16, 24 or 32");
    }
}

qint64 SampleConverter::GetMaxValue(int _sampleSize)
{
    switch(_sampleSize)
    {
    case 8:
        return SAMPLESIZE_8;
    case 16:
        return SAMPLESIZE_16;
    case 24: // TODO
        throw NotImplementedException();
    case 32:
        return SAMPLESIZE_32;
    default:
        throw ArgumentException("_sampleSize must only be 8, 16, 24 or 32");
    }
}

qint64 SampleConverter::RecoverSample(int _sampleSize, const QByteArray* _input, int _index)
{
    switch(_sampleSize)
    {
    case 8:
        return 2 * ConvertSample8(_input, _index) - SAMPLESIZE_8;
    case 16:
        return ConvertSample<qint16>(_input, _index);
    case 24:
        return ConvertSample24(_input, _index);
    case 32:
        return ConvertSample<qint32>(_input, _index);
    default:
        throw ArgumentException("_sampleSize must only be 8, 16, 24 or 32");
    }
}

void SampleConverter::ConvertSample8(qint64 _value, char*& _output)
{
    quint8 value = static_cast<quint8>(_value);
    memcpy(_output, &value, 1);
}

void SampleConverter::ConvertSample24(qint64 _value, char*& _output)
{
    // TODO
    Q_UNUSED(_value);
    Q_UNUSED(_output);
    throw NotImplementedException();
}

qint64 SampleConverter::ConvertSample8(const QByteArray* _input, int _index)
{
    if(_input != nullptr && _index >= 0 && _index < _input->size())
    {
        quint8 value;
        memcpy(&value, _input->constData() + _index, 1);
        return static_cast<qint64>(value);
    }
    return 0;
}

qint64 SampleConverter::ConvertSample24(const QByteArray* _input, int _index)
{
    // TODO
    Q_UNUSED(_input);
    Q_UNUSED(_index);
    throw NotImplementedException();
}
