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

#include "SinusComponent.h"
#include <QtMath>

SinusComponent::SinusComponent()
{
	m_name = "Sinusoidal";
}

qreal SinusComponent::getOutput(qreal _time)
{
    qreal frequency = m_inputs[0].getValue(_time);
    qreal amplitude = m_inputs[1].getValue(_time);
    qreal offset = m_inputs[2].getValue(_time);

	m_temp += (_time - m_prevTime) * frequency;
	m_prevTime = _time;

    return offset + amplitude * qSin(2 * M_PI * m_temp);
}
