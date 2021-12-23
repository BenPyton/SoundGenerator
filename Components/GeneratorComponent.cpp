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
#include "ComponentInput.h"

GeneratorComponent::GeneratorComponent()
{
    addInput(new ComponentInput("Frequency", this));
    addInput(new ComponentInput("Amplitude", this));
    addInput(new ComponentInput("Offset", this));

    getInput(0)->setDefaultValue(400.0);
    getInput(1)->setDefaultValue(1.0);
    getInput(2)->setDefaultValue(0.0);
}

void GeneratorComponent::init()
{
	Component::init();
    m_temp = 0.0;
    m_prevTime = 0.0;
}
