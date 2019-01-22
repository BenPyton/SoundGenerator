/*
* @author PELLETIER Benoit
*
* @file SinusComponent.cpp
*
* @date 19/01/2019
*
* @brief Implementation of sinusoidal generator component
*
*/

#include "stdafx.h"
#include "SinusComponent.h"

#define _PI 3.1415926f
#define _2PI 6.2831852f

SinusComponent::SinusComponent()
{
	m_name = "Sinusoidal";

	m_inputs.push_back(ComponentInput("Frequency", this));
	m_inputs.push_back(ComponentInput("Amplitude", this));
	m_inputs.push_back(ComponentInput("Offset", this));

	m_inputs[0].setDefaultValue(400.0f);
	m_inputs[1].setDefaultValue(1.0f);
	m_inputs[2].setDefaultValue(0.0f);
}

SinusComponent::~SinusComponent()
{
}

float SinusComponent::getOutput(float _time)
{
	float frequency = m_inputs[0].getValue(_time);
	float amplitude = m_inputs[1].getValue(_time);
	float offset = m_inputs[2].getValue(_time);

	return offset + amplitude * sinf(frequency * _2PI * _time);
}
