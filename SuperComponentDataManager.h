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

#ifndef SUPERCOMPONENTDATAMANAGER_H
#define SUPERCOMPONENTDATAMANAGER_H

#include <QObject>
#include <QVector>
#include <QUuid>

class SuperComponentData;

class SuperComponentDataManager : public QObject
{
    Q_OBJECT

public:
    SuperComponentDataManager(QObject* _parent = nullptr);

    SuperComponentData* getData(int _index);
    SuperComponentData* getData(QUuid _uuid);
    inline int count() { return m_dataList.count(); }

    SuperComponentData* createData();
    void deleteData(SuperComponentData* _data);
    void deleteData(QUuid _uuid);

    QJsonArray toJson();
    void fromJson(QJsonArray& _json);

protected:
    int getDataIndex(QUuid _uuid);

signals:
    void dataDeleted(SuperComponentData* _data);

private:
    QVector<SuperComponentData*> m_dataList;
};

#endif // SUPERCOMPONENTDATAMANAGER_H
