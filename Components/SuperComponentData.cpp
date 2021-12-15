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

#include "SuperComponentData.h"
#include "Component.h"
#include "NodalScene.h"
#include "NodeItem.h"
#include <QJsonArray>

SuperComponentData::SuperComponentData(QString _name, QObject* _parent)
    : QObject(_parent), m_name(_name)
{
    m_scene = new NodalScene(this);
    m_uuid = QUuid::createUuid();
    qDebug() << "Created UUID: " << m_uuid;
}

SuperComponentData::~SuperComponentData()
{
    qDebug() << "Super Component Deleted: " << m_name;
    emit onDeleted(this);
}

void SuperComponentData::setName(QString _name)
{
    m_name = _name;
    emit onNameEdited(this);
}

Component* SuperComponentData::output()
{
    return m_scene->getOutput()->component();
}

QJsonArray SuperComponentData::DataArrayToJson(const QVector<SuperComponentData*>& _dataArray)
{
    Q_UNUSED(_dataArray)

    QJsonArray jsonArray;
    // TODO: save data
    return jsonArray;
}

void SuperComponentData::JsonToDataArray(const QJsonArray& _jsonArray, QVector<SuperComponentData*>& _dataArray)
{
    Q_UNUSED(_jsonArray)

    _dataArray.clear();
    // TODO: load data
}
