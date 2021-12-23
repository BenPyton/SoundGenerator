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

#include "MultiplyComponent.h"
#include "ComponentInput.h"

MultiplyComponent::MultiplyComponent()
{
	m_name = "Multiply";

    addInput(new ComponentInput("Signal A", this));
    addInput(new ComponentInput("Signal B", this));

    getInput(0)->setDefaultValue(0.0);
    getInput(1)->setDefaultValue(0.0);
}

qreal MultiplyComponent::getOutput(qreal _time)
{
    qreal signalA = getInput(0)->getValue(_time);
    qreal signalB = getInput(1)->getValue(_time);
	return signalA * signalB;
}
