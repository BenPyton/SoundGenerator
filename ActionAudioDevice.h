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

#ifndef ACTIONAUDIODEVICE_H
#define ACTIONAUDIODEVICE_H

#include <QMenu>
#include <QAudioDeviceInfo>

class ActionAudioDevice : public QMenu
{
    Q_OBJECT

public:
    ActionAudioDevice(const QString& _title, QWidget* _parent = nullptr);

signals:
    void deviceChanged(const QAudioDeviceInfo& _info);

private slots:
    void beforeShowing();
    void onTriggered(QAction* _action);

private:
    static const QList<QAudioDeviceInfo> _listAvalaibleDevices();

private:
    QAudioDeviceInfo m_currentInfo;
};

#endif // ACTIONAUDIODEVICE_H
