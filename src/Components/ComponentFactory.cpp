/*
* @author PELLETIER Benoit
*
* @file ComponentFactory.cpp
*
* @date 17/02/2019
*
* @brief Factory to create components from string names
*
*/

#include "stdafx.h"
#include "../Components.h"

Component * ComponentFactory::CreateComponent(const string & _compName)
{
	Component* pComp = nullptr;

	if (_compName == "Sinusoidal")
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

	return pComp;
}
