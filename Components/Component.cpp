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

#include "Component.h"

Component::Component()
{
	m_name = "Component";
}

Component::~Component()
{
}

void Component::init()
{
	for (ComponentInput input : m_inputs)
	{
		Component* c = input.getComponent();
		if (nullptr != c)
		{
			c->init();
		}
	}
}

ComponentInput * Component::getInput(int _index)
{
    if (_index >= 0 &&_index < m_inputs.size())
	{
		return &m_inputs[_index];
	}
	return nullptr;
}

ComponentInput * Component::getInput(QString _name)
{
	ComponentInput* input = nullptr;
    for (QVector<ComponentInput>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it)
	{
		if ((*it).getName() == _name)
		{
			input = &(*it);
			break;
		}
	}
	return input;
}

ComponentInput::ComponentInput()
    : ComponentInput("DEFAULT", nullptr)
{
}

ComponentInput::ComponentInput(QString _name, Component* _parent)
    : m_pParent(_parent), m_name(_name), m_editable(true), m_linkable(true)
{
}

bool ComponentInput::canSetComponent(Component * _comp)
{
    return _comp != nullptr && !_comp->_hasAlreadyInput(m_pParent);
}

qreal ComponentInput::getValue(qreal _time)
{
	if (m_pComponent != nullptr)
	{
		return m_pComponent->getOutput(_time);
	}
	return m_defaultValue;
}

bool Component::_hasAlreadyInput(Component * _comp)
{
	bool hasComp = false;
	if (_comp != nullptr)
	{
        for (QVector<ComponentInput>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it)
		{
			hasComp |= (it->m_pComponent != nullptr) && ((it->m_pComponent == _comp) || it->m_pComponent->_hasAlreadyInput(_comp));
		}
	}
	return hasComp;
}
