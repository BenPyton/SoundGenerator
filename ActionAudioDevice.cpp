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

#include "ActionAudioDevice.h"
#include <QtCore>
#include <QAudioDeviceInfo>

void DebugLogDeviceInfo(const QAudioDeviceInfo& _info)
{
    qDebug() << "Device: " << _info.deviceName()
             << " | samples rates: " << _info.supportedSampleRates()
             << " | channel count: " << _info.supportedChannelCounts()
             << " | samples size: " << _info.supportedSampleSizes()
             << " | codecs: " << _info.supportedCodecs()
             << " | byte orders: " << _info.supportedByteOrders()
             << " | sample types: " << _info.supportedSampleTypes();
}

ActionAudioDevice::ActionAudioDevice(const QString& _title, QWidget* _parent)
    : QMenu(_title, _parent)
{
    connect(this, &QMenu::aboutToShow, this, &ActionAudioDevice::beforeShowing);
    connect(this, &QMenu::triggered, this, &ActionAudioDevice::onTriggered);

    m_currentInfo = QAudioDeviceInfo::defaultOutputDevice();
    DebugLogDeviceInfo(m_currentInfo);
}

void ActionAudioDevice::beforeShowing()
{
    clear();

    const QList<QAudioDeviceInfo> deviceList = _listAvalaibleDevices();
    for(QAudioDeviceInfo device : deviceList)
    {
        QVariant data;
        data.setValue(device);

        QAction* act = new QAction(device.deviceName(), this);
        act->setCheckable(true);
        act->setChecked(device == m_currentInfo);
        if(device == m_currentInfo)
        {
            DebugLogDeviceInfo(device);
        }

        act->setData(data);
        addAction(act);
    }
}

void ActionAudioDevice::onTriggered(QAction* _action)
{
    QAudioDeviceInfo info = qvariant_cast<QAudioDeviceInfo>(_action->data());
    qDebug() << "Clicked on action!";
    DebugLogDeviceInfo(info);
    if(info != m_currentInfo)
    {
        m_currentInfo = info;
        emit deviceChanged(info);
    }
}

const QList<QAudioDeviceInfo> ActionAudioDevice::_listAvalaibleDevices()
{
    QMap<QString, QAudioDeviceInfo> infoMap;
    QList<QAudioDeviceInfo> deviceList = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(QAudioDeviceInfo device : deviceList)
    {
        QMap<QString, QAudioDeviceInfo>::iterator it = infoMap.find(device.deviceName());
        if(it != infoMap.end())
        {
            int score = 0;
            score += (device.supportedSampleRates().count() > it->supportedSampleRates().count())
                    ? 1 : -1;
            score += (device.supportedSampleSizes().count() > it->supportedSampleSizes().count())
                    ? 1 : -1;
            score += (device.supportedChannelCounts().count() > it->supportedChannelCounts().count())
                    ? 1 : -1;

            if(score > 0)
            {
                infoMap[device.deviceName()] = device;
            }
        }
        else
        {
            infoMap.insert(device.deviceName(), device);
        }

    }
    return infoMap.values();
}
