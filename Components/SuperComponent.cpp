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
#include "NodalScene.h"
#include "InputComponent.h"
#include "NodeItem.h"
#include "SuperComponentInput.h"
#include <QJsonValue>

SuperComponent::SuperComponent(SuperComponentData* _data)
    : Component(), m_data(_data)
{
    Q_ASSERT(nullptr != m_data);
    m_name = m_data->name();
}

void SuperComponent::init()
{
    Q_ASSERT(nullptr != m_data);
    m_data->output()->init();
}

qreal SuperComponent::getOutput(qreal _time)
{
    Q_ASSERT(nullptr != m_data);
    return m_data->output()->getOutput(_time);
}

void SuperComponent::updateInputs()
{
    // Get all input components in the data's scene
    QVector<NodeItem*> inputItems = m_data->scene()->getInputs();
    QVector<InputComponent*> comps(inputItems.count());
    QVector<QUuid> uuids(inputItems.count());
    for(int i = 0; i < inputItems.count(); ++i)
    {
        InputComponent* input = dynamic_cast<InputComponent*>(inputItems[i]->component());
        Q_ASSERT(nullptr != input);
        comps[i] = input;
        uuids[i] = input->uuid();
    }

    // array filled with the new inputs
    QVector<ComponentInput*> newInputs;
    newInputs.reserve(comps.count());

    // swap the input array to work on it
    QVector<ComponentInput*> oldInputs;
    swapInputArrays(oldInputs);

    // compare old inputs uuid to input components, so keep them in the new array
    for(int i = 0; i < oldInputs.count(); ++i)
    {
        SuperComponentInput* superInput = dynamic_cast<SuperComponentInput*>(oldInputs[i]);
        Q_ASSERT(nullptr != superInput);

        int index = uuids.indexOf(superInput->uuid());
        if(index >= 0)
        {
            newInputs.push_back(oldInputs[i]);
            oldInputs[i] = nullptr;
            comps[index] = nullptr;
        }
    }

    // add the remaining comps to the new inputs
    for(int i = 0; i < comps.count(); ++i)
    {
        if(nullptr != comps[i])
        {
            newInputs.push_back(new SuperComponentInput(comps[i], this));
        }
    }

    // delete the remaining old inputs
    for(int i = 0; i < oldInputs.count(); ++i)
    {
        if(nullptr != oldInputs[i])
        {
            delete oldInputs[i];
        }
    }

    // swap back to the input array
    swapInputArrays(newInputs);
}

QJsonValue SuperComponent::toJson() const
{
    return m_data->uuid().toString();
}

void SuperComponent::fromJson(const QJsonValue& _json)
{
    QUuid uuid(_json.toString());

    // get data from uuid

    updateInputs();
}
