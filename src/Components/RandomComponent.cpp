/*
* @author PELLETIER Benoit
*
* @file RandomComponent.cpp
*
* @date 22/01/2019
*
* @brief Implementation of a random signal generator
*
*/

#include "stdafx.h"
#include "RandomComponent.h"
#include "../Core/Random.h"

RandomComponent::RandomComponent()
{
	m_name = "Random";
	m_inputs.push_back(ComponentInput("Amplitude", this));
	m_inputs.push_back(ComponentInput("Offset", this));

	m_inputs[0].setDefaultValue(1.0f);
	m_inputs[1].setDefaultValue(0.0f);
}

RandomComponent::~RandomComponent()
{
}

float RandomComponent::getOutput(float _time)
{
	float amplitude = m_inputs[0].getValue(_time);
	float offset = m_inputs[1].getValue(_time);
	return amplitude * (2 * (Random::Range(0, INT16_MAX) / (float)INT16_MAX) - 1) + offset;
}