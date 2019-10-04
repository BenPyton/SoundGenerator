/*
 * Copyright 2019 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "ADSRComponent.h"
#include "utils.h"

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

    m_inputs[0].setDefaultValue(0.0);
    m_inputs[1].setDefaultValue(0.1);
    m_inputs[1].setLinkable(false);
    m_inputs[2].setDefaultValue(0.1);
    m_inputs[2].setLinkable(false);
    m_inputs[3].setDefaultValue(0.7);
    m_inputs[3].setLinkable(false);
    m_inputs[4].setDefaultValue(0.5);
    m_inputs[4].setLinkable(false);
    m_inputs[5].setDefaultValue(0.1);
    m_inputs[5].setLinkable(false);
}

qreal ADSRComponent::getOutput(qreal _time)
{
    qreal input = m_inputs[0].getValue(_time);
    qreal attackTime = m_inputs[1].getValue(_time);
    qreal decayTime = attackTime + m_inputs[2].getValue(_time);
    qreal sustainTime = decayTime + m_inputs[3].getValue(_time);
    qreal sustainAmp = m_inputs[4].getValue(_time);
    qreal releaseTime = sustainTime + m_inputs[5].getValue(_time);

    qreal value = 0;
	if (_time > 0 && _time <= attackTime)
	{
        value = Utils::MapValue(_time, 0, attackTime, 0, 1);
	}
	else if (_time > attackTime && _time <= decayTime)
	{
        value = Utils::MapValue(_time, attackTime, decayTime, 1, sustainAmp);
	}
	else if (_time > decayTime && _time <= sustainTime)
	{
		value = sustainAmp;
	}
	else if (_time > sustainTime && _time <= releaseTime)
	{
        value = Utils::MapValue(_time, sustainTime, releaseTime, sustainAmp, 0);
	}

	return input * value;
}
