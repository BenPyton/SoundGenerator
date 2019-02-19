/*
* @author PELLETIER Benoit
*
* @file DelayComponent.cpp
*
* @date 18/02/2019
*
* @brief Define a component that delay a signal
*
*/

#include "stdafx.h"
#include "DelayComponent.h"

DelayComponent::DelayComponent()
{
	m_name = "Delay";

	m_inputs.push_back(ComponentInput("Signal", this));
	m_inputs.push_back(ComponentInput("Delay", this));

	m_inputs[0].setDefaultValue(0.0f);
	m_inputs[1].setDefaultValue(0.0f);
}

DelayComponent::~DelayComponent()
{
}

float DelayComponent::getOutput(float _time)
{
	float delay = m_inputs[1].getValue(_time);
	float t = _time - delay;
	float value = (t < 0) ? 0 : m_inputs[0].getValue(t);
	return value;
}
