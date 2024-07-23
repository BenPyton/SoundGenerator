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

#include "ProjectFile.h"
#include <QtCore>
#include "Signal.h"
#include "NodalScene.h"
#include "NodeItem.h"
#include "SuperComponentDataManager.h"
#include "Utils.h"
#include "AudioSettings.h"

// JSON keys
#define VERSION "version"
#define SETTINGS "settings"
#define COMPONENTS "components"
#define SUPERCOMPONENTS "super-component-data"

// Compatibility
#define DURATION "duration"

ProjectFile::ProjectFile(QObject* _parent)
    : QObject(_parent), m_fileName()
{
    m_signal = new Signal(this);

    m_superCompManager = new SuperComponentDataManager(this);

    m_scene = new NodalScene(this);
    m_scene->setBackgroundBrush(Qt::black);
    connect(m_scene, &NodalScene::dirtyChanged, this, &ProjectFile::setDirty);
}

ProjectFile::~ProjectFile()
{
}

QJsonObject ProjectFile::toJson() const
{
    QJsonObject json;
    json[VERSION] = Utils::GetJSONVersion();
    json[COMPONENTS] = m_scene->toJson();
    json[SETTINGS] = m_signal->settings().toJson();
    json[SUPERCOMPONENTS] = m_superCompManager->toJson();
    return json;
}

void ProjectFile::fromJson(const QJsonObject& _json)
{
    m_dirtyable = false;

    // ============== VERSION =============
    if(!Utils::CheckJsonValue(_json, VERSION, QJsonValue::Double, 100))
        return;

    int version = _json[VERSION].toInt();
    if(version > Utils::GetJSONVersion())
    {
        Utils::ErrorMsg(101, "Wrong version: file version is " + QString::number(version) + " and should be " + QString::number(Utils::GetJSONVersion()));
        return;
    }

    // ============== SETTINGS =============
    if(!Utils::CheckJsonValue(_json, SETTINGS, QJsonValue::Object, 115))
        return;

    m_signal->settings().fromJson(_json[SETTINGS].toObject(), version);

    if(version <= 1) // compatibility
    {
        if(!Utils::CheckJsonValue(_json, DURATION, QJsonValue::Double, 110))
            return;

        m_signal->settings().setDuration(_json[DURATION].toDouble());
    }

    // ============== SUPER COMPONENTS =============
    if(!Utils::CheckJsonValue(_json, SUPERCOMPONENTS, QJsonValue::Array, 125))
        return;

    QJsonArray superCompArray = _json[COMPONENTS].toArray();
    m_superCompManager->fromJson(superCompArray);

    // ============== COMPONENTS =============
    if(!Utils::CheckJsonValue(_json, COMPONENTS, QJsonValue::Array, 120))
        return;

    QJsonArray componentArray = _json[COMPONENTS].toArray();
    m_scene->fromJson(componentArray);

    m_signal->updateAudioOutput(m_signal->deviceInfo());
    m_dirtyable = true;
}

void ProjectFile::setDirty()
{
    bool wasPristine = false;
    if(!m_dirty && m_dirtyable)
    {
        m_dirty = true;
        wasPristine = true;
    }
    emit dirtyChanged(wasPristine);
}

void ProjectFile::generate()
{
    m_signal->generate(m_scene->getOutput()->component());
}

