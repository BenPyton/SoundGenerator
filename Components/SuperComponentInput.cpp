/*
 * Copyright 2021 Benoit Pelletier
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

#include "SuperComponentInput.h"
#include "ComponentInput.h"
#include "InputComponent.h"
#include "NodeItem.h"

SuperComponentInput::SuperComponentInput(InputComponent* _input, Component *_parent)
    : ComponentInput("NULL", _parent), m_inputComponent(_input)
{
    Q_ASSERT(m_inputComponent != nullptr);
    m_cachedUuid = m_inputComponent->uuid();
    m_defaultValue = _input->getInput(0)->getDefaultValue();
}

qreal SuperComponentInput::getValue(qreal _time)
{
    Q_ASSERT(m_inputComponent != nullptr);
    return m_inputComponent->getOutput(_time);
}

QString SuperComponentInput::getName() const
{
    Q_ASSERT(m_inputComponent != nullptr);
    return m_inputComponent->getName();
}
