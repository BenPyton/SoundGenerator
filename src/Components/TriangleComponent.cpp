/*
* @author PELLETIER Benoit
*
* @file TriangleComponent.cpp
*
* @date 19/01/2019
*
* @brief Implementation of trangle signal generator component
*
*/

#include "stdafx.h"
#include "TriangleComponent.h"

TriangleComponent::TriangleComponent()
{
	m_name = "Triangle";
}

TriangleComponent::~TriangleComponent()
{
}

float TriangleComponent::getOutput(float _time)
{
	float frequency = m_inputs[0].getValue(_time);
	float amplitude = m_inputs[1].getValue(_time);
	float offset = m_inputs[2].getValue(_time);

	m_temp += (_time - m_prevTime) * frequency;
	m_prevTime = _time;

	return offset + amplitude * (2.0f * abs(2.0f * fmod(m_temp, 1.0f) - 1.0f) - 1.0f);
}
