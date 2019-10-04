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

#include "ComponentFactory.h"
#include "../Components.h"

Component * ComponentFactory::CreateComponent(const QString & _compName)
{
	Component* pComp = nullptr;

    if (_compName == "Output")
    {
        pComp = new OutputComponent();
    }
    else if (_compName == "Sinusoidal")
	{
		pComp = new SinusComponent();
	}
	else if (_compName == "Square")
	{
		pComp = new SquareComponent();
	}
	else if (_compName == "Triangle")
	{
		pComp = new TriangleComponent();
	}
	else if (_compName == "Saw Tooth")
	{
		pComp = new SawToothComponent();
	}
	else if (_compName == "Random")
	{
		pComp = new RandomComponent();
	}
	else if (_compName == "Add")
	{
		pComp = new AddComponent();
	}
	else if (_compName == "Multiply")
	{
		pComp = new MultiplyComponent();
	}
	else if (_compName == "Repeat")
	{
		pComp = new RepeatComponent();
	}
	else if (_compName == "Delay")
	{
		pComp = new DelayComponent();
	}
	else if (_compName == "ADSR")
	{
		pComp = new ADSRComponent();
	}

	return pComp;
}
