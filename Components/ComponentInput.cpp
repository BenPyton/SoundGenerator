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

#include "ComponentInput.h"
#include "Component.h"

ComponentInput::ComponentInput(QString _name, Component* _parent)
    : m_pParent(_parent), m_name(_name), m_editable(true), m_linkable(true)
{
}

ComponentInput::~ComponentInput()
{
}

QString ComponentInput::getName() const
{
    return m_name;
}

bool ComponentInput::canSetComponent(Component* _comp)
{
    return _comp != nullptr && !_comp->_hasAlreadyInput(m_pParent);
}

qreal ComponentInput::getValue(qreal _time)
{
    if (nullptr != m_pComponent)
	{
		return m_pComponent->getOutput(_time);
	}
	return m_defaultValue;
}
