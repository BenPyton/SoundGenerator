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

#include "SuperComponentDataManager.h"
#include "Components/SuperComponentData.h"
#include <QVariant>
#include <QJsonArray>
#include <QUuid>
#include <QtCore>

SuperComponentDataManager::SuperComponentDataManager(QObject *_parent)
    : QObject(_parent)
{
}

SuperComponentData* SuperComponentDataManager::getData(int _index)
{
    Q_ASSERT(_index >= 0 && _index < m_dataList.count());
    return m_dataList.at(_index);
}

SuperComponentData* SuperComponentDataManager::getData(QUuid _uuid)
{
    SuperComponentData* foundData = nullptr;
    int index = getDataIndex(_uuid);
    if(index >= 0)
    {
        foundData = m_dataList[index];
    }
    return foundData;
}

SuperComponentData* SuperComponentDataManager::createData()
{
    QString dataName = "Super Component";
    SuperComponentData* data = new SuperComponentData(dataName, this);
    m_dataList.push_back(data);
    return data;
}

void SuperComponentDataManager::deleteData(QUuid _uuid)
{
    int index = getDataIndex(_uuid);
    if(index >= 0)
    {
        SuperComponentData* data = m_dataList[index];
        m_dataList.remove(index);
        emit dataDeleted(data);
        delete data;
    }
}

QJsonArray SuperComponentDataManager::toJson()
{
    return SuperComponentData::DataArrayToJson(m_dataList);
}

void SuperComponentDataManager::fromJson(QJsonArray &_json)
{
    SuperComponentData::JsonToDataArray(_json, m_dataList);
}

int SuperComponentDataManager::getDataIndex(QUuid _uuid)
{
    int index = -1;
    for(int i = 0; index < 0 && i < m_dataList.count(); ++i)
    {
        qDebug() << "Comparing " << _uuid << " with " << m_dataList[i]->uuid();
        if(m_dataList[i]->uuid() == _uuid)
        {
            index = i;
        }
    }
    return index;
}
