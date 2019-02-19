/*
* @author PELLETIER Benoit
*
* @file AddComponent.cpp
*
* @date 18/02/2019
*
* @brief Define a component that return the addition of two signals
*
*/

#include "stdafx.h"
#include "AddComponent.h"

AddComponent::AddComponent()
{
	m_name = "Add";

	m_inputs.push_back(ComponentInput("Signal A", this));
	m_inputs.push_back(ComponentInput("Signal B", this));

	m_inputs[0].setDefaultValue(0.0f);
	m_inputs[1].setDefaultValue(0.0f);
}

AddComponent::~AddComponent()
{
}

float AddComponent::getOutput(float _time)
{
	float signalA = m_inputs[0].getValue(_time);
	float signalB = m_inputs[1].getValue(_time);
	return signalA + signalB;
}
