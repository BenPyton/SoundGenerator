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

#ifndef SUPERCOMPONENTDATA_H
#define SUPERCOMPONENTDATA_H

#include <QObject>
#include <QString>
#include <QUuid>

class NodalScene;
class Component;

class SuperComponentData : public QObject
{
    Q_OBJECT

public:
    SuperComponentData(QString _name, QObject* _parent = nullptr);
    virtual ~SuperComponentData();

    inline NodalScene* scene() { return m_scene; }
    inline QUuid uuid() { return m_uuid; }
    inline QString name() { return m_name; }
    void setName(QString _name);
    Component* output();

    static QJsonArray DataArrayToJson(const QVector<SuperComponentData*>& _dataArray);
    static void JsonToDataArray(const QJsonArray& _jsonArray, QVector<SuperComponentData*>& _dataArray);

signals:
    void onNameEdited(SuperComponentData* _data);
    void onSceneEdited(SuperComponentData* _data);
    void onDeleted(SuperComponentData* _data);

private:
    QUuid m_uuid;
    QString m_name;
    NodalScene* m_scene;
};

#endif // SUPERCOMPONENTDATA_H
