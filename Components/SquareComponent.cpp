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

#include "SquareComponent.h"
#include "ComponentInput.h"
#include <QtMath>

SquareComponent::SquareComponent()
{
	m_name = "Square";
}

qreal SquareComponent::getOutput(qreal _time)
{
    qreal frequency = getInput(0)->getValue(_time);
    qreal amplitude = getInput(1)->getValue(_time);
    qreal offset = getInput(2)->getValue(_time);

	m_temp += (_time - m_prevTime) * frequency;
	m_prevTime = _time;

    return offset + amplitude * (2. * qRound(fmod(m_temp, 1.)) - 1.);
}
