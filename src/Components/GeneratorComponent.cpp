/*
* @author PELLETIER Benoit
*
* @file GeneratorComponent.cpp
*
* @date 03/03/2019
*
* @brief Base class for all generator component
*
*/

#include "stdafx.h"
#include "GeneratorComponent.h"

GeneratorComponent::GeneratorComponent()
{
	m_inputs.push_back(ComponentInput("Frequency", this));
	m_inputs.push_back(ComponentInput("Amplitude", this));
	m_inputs.push_back(ComponentInput("Offset", this));

	m_inputs[0].setDefaultValue(400.0f);
	m_inputs[1].setDefaultValue(1.0f);
	m_inputs[2].setDefaultValue(0.0f);
}

GeneratorComponent::~GeneratorComponent()
{
}

void GeneratorComponent::init()
{
	Component::init();
	m_temp = 0.0f;
	m_prevTime = 0.0f;
}