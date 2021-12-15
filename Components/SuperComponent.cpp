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

#include "SuperComponent.h"
#include "SuperComponentData.h"

SuperComponent::SuperComponent(SuperComponentData* _data)
    : Component(), m_data(_data)
{
    Q_ASSERT(m_data != nullptr);
    m_name = m_data->name();
}

void SuperComponent::init()
{
    Q_ASSERT(m_data != nullptr);
    m_data->output()->init();
}

qreal SuperComponent::getOutput(qreal _time)
{
    Q_ASSERT(m_data != nullptr);
    return m_data->output()->getOutput(_time);
}

void SuperComponent::updateInputs()
{

}
