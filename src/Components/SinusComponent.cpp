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
}

SinusComponent::~SinusComponent()
{
}

float SinusComponent::getOutput(float _time)
{
	float frequency = m_inputs[0].getValue(_time);
	float amplitude = m_inputs[1].getValue(_time);
	float offset = m_inputs[2].getValue(_time);

	m_temp += (_time - m_prevTime) * frequency;
	m_prevTime = _time;

	return offset + amplitude * sinf(_2PI * m_temp);
}
