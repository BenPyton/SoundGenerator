/*
* @author PELLETIER Benoit
*
* @file SawToothComponent.cpp
*
* @date 22/01/2019
*
* @brief Implementation of a sawtooth signal generator
*
*/

#include "stdafx.h"
#include "SawToothComponent.h"

SawToothComponent::SawToothComponent()
{
	m_name = "Saw Tooth";
}

SawToothComponent::~SawToothComponent()
{
}

float SawToothComponent::getOutput(float _time)
{
	float frequency = m_inputs[0].getValue(_time);
	float amplitude = m_inputs[1].getValue(_time);
	float offset = m_inputs[2].getValue(_time);

	m_temp += (_time - m_prevTime) * frequency;
	m_prevTime = _time;

	return offset + amplitude * (2.0f * fmod(m_temp, 1.0f) - 1.0f);
}