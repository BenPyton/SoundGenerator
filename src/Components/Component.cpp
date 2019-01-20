/*
* @author PELLETIER Benoit
*
* @file Component.cpp
*
* @date 16/01/2019
*
* @brief Define a component that modify a signal
*
*/

#include "stdafx.h"
#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

ComponentInput * Component::getInput(int _index)
{
	if (_index >= 0 && _index < m_inputs.size())
	{
		return &m_inputs[_index];
	}
	return nullptr;
}

ComponentInput * Component::getInput(string _name)
{
	ComponentInput* input = nullptr;
	for (vector<ComponentInput>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it)
	{
		if ((*it).getName() == _name)
		{
			input = &(*it);
			break;
		}
	}
	return input;
}

ComponentInput::ComponentInput(string _name)
	: m_name(_name)
{
}

float ComponentInput::getValue(float _time)
{
	if (m_pComponent != nullptr)
	{
		return m_pComponent->getOutput(_time);
	}
	return m_defaultValue;
}
