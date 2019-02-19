/*
* @author PELLETIER Benoit
*
* @file ADSRComponent.cpp
*
* @date 18/02/2019
*
* @brief Define an ADSR (Attack, Decay, Sustain, Release) that modify the envelop of the signal
*
*/

#include "stdafx.h"
#include "ADSRComponent.h"
#include "../Core/Utils.h"

// https://en.wikipedia.org/wiki/File:ADSR_parameter.svg
// https://www.youtube.com/watch?v=A6pp6OMU5r8&ab_channel=SynthSchool

ADSRComponent::ADSRComponent()
{
	m_name = "ADSR";

	m_inputs.push_back(ComponentInput("Signal", this));
	m_inputs.push_back(ComponentInput("Attack Time", this));
	m_inputs.push_back(ComponentInput("Decay Time", this));
	m_inputs.push_back(ComponentInput("Sustain Time", this));
	m_inputs.push_back(ComponentInput("Sustain Amp", this));
	m_inputs.push_back(ComponentInput("Release Time", this));

	m_inputs[0].setDefaultValue(0.0f);
	m_inputs[1].setDefaultValue(0.1f);
	m_inputs[2].setDefaultValue(0.1f);
	m_inputs[3].setDefaultValue(0.7f);
	m_inputs[4].setDefaultValue(0.5f);
	m_inputs[5].setDefaultValue(0.1f);
}

ADSRComponent::~ADSRComponent()
{
}

float ADSRComponent::getOutput(float _time)
{
	float input = m_inputs[0].getValue(_time);
	float attackTime = m_inputs[1].getValue(_time);
	float decayTime = attackTime + m_inputs[2].getValue(_time);
	float sustainTime = decayTime + m_inputs[3].getValue(_time);
	float sustainAmp = m_inputs[4].getValue(_time);
	float releaseTime = sustainTime + m_inputs[5].getValue(_time);

	float value = 0;
	if (_time > 0 && _time <= attackTime)
	{
		value = mapValue(_time, 0, attackTime, 0, 1);
	}
	else if (_time > attackTime && _time <= decayTime)
	{
		value = mapValue(_time, attackTime, decayTime, 1, sustainAmp);
	}
	else if (_time > decayTime && _time <= sustainTime)
	{
		value = sustainAmp;
	}
	else if (_time > sustainTime && _time <= releaseTime)
	{
		value = mapValue(_time, sustainTime, releaseTime, sustainAmp, 0);
	}

	return input * value;
}
