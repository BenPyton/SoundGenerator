/*
* @author PELLETIER Benoit
*
* @file OutputComponent.cpp
*
* @date 12/02/2019
*
* @brief Define the output that generate the sound
*
*/

#include "stdafx.h"
#include "OutputComponent.h"

OutputComponent::OutputComponent()
{
	m_name = "Output";

	m_inputs.push_back(ComponentInput("Output", this));

	m_inputs[0].setDefaultValue(0.0f);
}

OutputComponent::~OutputComponent()
{
}

float OutputComponent::getOutput(float _time)
{
	return m_inputs[0].getValue(_time);
}
