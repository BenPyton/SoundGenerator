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

#include "InputComponent.h"
#include "ComponentInput.h"

InputComponent::InputComponent()
	: Component()
{
    m_name = "New Input";

    addInput(new ComponentInput("", this));

    getInput(0)->setDefaultValue(0.0);
    getInput(0)->setEditable(true);
    getInput(0)->setLinkable(false);

    m_renamable = true;
}

qreal InputComponent::getOutput(qreal _time)
{
    return getInput(0)->getValue(_time);
}
