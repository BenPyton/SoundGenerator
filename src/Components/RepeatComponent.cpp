/*
* @author PELLETIER Benoit
*
* @file RepeatComponent.cpp
*
* @date 18/02/2019
*
* @brief Define a component that repeat a signal
*
*/

#include "stdafx.h"
#include "RepeatComponent.h"

RepeatComponent::RepeatComponent()
{
	m_name = "Repeat";

	m_inputs.push_back(ComponentInput("Signal", this));
	m_inputs.push_back(ComponentInput("Duration", this));

	m_inputs[0].setDefaultValue(0.0f);
	m_inputs[1].setDefaultValue(1.0f);
}

RepeatComponent::~RepeatComponent()
{
}

float RepeatComponent::getOutput(float _time)
{
	float t = fmod(_time, m_inputs[1].getValue(_time));
	float value = m_inputs[0].getValue(t);
	return value;
}
