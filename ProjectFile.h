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

#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QObject>
#include "JsonInterface.h"
#include <QString>

class NodalScene;
class SuperComponentDataManager;
class Signal;

class ProjectFile : public QObject, public JsonInterface
{
    Q_OBJECT

public:
    ProjectFile(QObject* _parent = nullptr);
    virtual ~ProjectFile();

    inline QString fileName() const { return m_fileName; }
    inline void setFileName(QString _fileName) { m_fileName = _fileName; }

    inline bool dirty() { return m_dirty; }

    inline bool dirtyable() const { return m_dirtyable; }
    inline void setDirtyable(bool _value) { m_dirtyable = _value; }

    inline NodalScene* scene() const { return m_scene; }
    inline Signal* signal() const { return m_signal; }
    inline SuperComponentDataManager* dataManager() const { return m_superCompManager; }

    // JsonInterface
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& _json);

public slots:
    void setDirty();
    void generate();

signals:
    void dirtyChanged(bool _wasPristine);

private:
    QString m_fileName;
    bool m_dirty;
    bool m_dirtyable;

    NodalScene* m_scene;
    Signal* m_signal;
    SuperComponentDataManager* m_superCompManager;
};

#endif // PROJECTFILE_H
