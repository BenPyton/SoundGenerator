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

#include "GeneratorComponent.h"

GeneratorComponent::GeneratorComponent()
{
	m_inputs.push_back(ComponentInput("Frequency", this));
	m_inputs.push_back(ComponentInput("Amplitude", this));
	m_inputs.push_back(ComponentInput("Offset", this));

    m_inputs[0].setDefaultValue(400.0);
    m_inputs[1].setDefaultValue(1.0);
    m_inputs[2].setDefaultValue(0.0);
}

void GeneratorComponent::init()
{
	Component::init();
    m_temp = 0.0;
    m_prevTime = 0.0;
}
