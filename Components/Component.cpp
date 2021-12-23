/*
 * Copyright 2019, 2021 Benoit Pelletier
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

#include "Component.h"
#include "ComponentInput.h"

Component::Component()
{
	m_name = "Component";
}

Component::~Component()
{
    for(QVector<ComponentInput*>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it)
    {
        delete *it;
    }
}

void Component::init()
{
    for (QVector<ComponentInput*>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it)
	{
        Component* c = (*it)->getComponent();
		if (nullptr != c)
		{
			c->init();
		}
	}
}

ComponentInput* Component::getInput(int _index)
{
    if (_index >= 0 && _index < m_inputs.size())
	{
        return m_inputs[_index];
	}
    return nullptr;
}

void Component::setName(QString _name)
{
    Q_ASSERT(m_renamable);
    m_name = _name;
}

void Component::addInput(ComponentInput* _input)
{
    m_inputs.push_back(_input);
}

void Component::swapInputArrays(QVector<ComponentInput*>& _other)
{
    m_inputs.swap(_other);
}

bool Component::_hasAlreadyInput(Component* _comp)
{
    bool hasComp = false;
    if (nullptr != _comp)
    {
        for (QVector<ComponentInput*>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it)
        {
            Component* comp = (*it)->getComponent();
            hasComp |= (nullptr != comp) && ((comp == _comp) || comp->_hasAlreadyInput(_comp));
        }
    }
    return hasComp;
}
