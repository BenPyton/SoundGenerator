/*
* @author PELLETIER Benoit
*
* @file SquareComponent.cpp
*
* @date 19/01/2019
*
* @brief Implementation of squared signal generator component
*
*/

#include "stdafx.h"
#include "SquareComponent.h"

SquareComponent::SquareComponent()
{
	m_inputs.push_back(ComponentInput("Frequency", this));
	m_inputs.push_back(ComponentInput("Amplitude", this));
	m_inputs.push_back(ComponentInput("Offset", this));

	m_inputs[0].setDefaultValue(400.0f);
	m_inputs[1].setDefaultValue(1.0f);
	m_inputs[2].setDefaultValue(0.0f);
}

SquareComponent::~SquareComponent()
{
}

float SquareComponent::getOutput(float _time)
{
	float frequency = m_inputs[0].getValue(_time);
	float amplitude = m_inputs[1].getValue(_time);
	float offset = m_inputs[2].getValue(_time);
	float value = offset + amplitude * (2.0f * round(fmod(frequency * _time, 1.0f)) - 1.0f);
	return value;
}
