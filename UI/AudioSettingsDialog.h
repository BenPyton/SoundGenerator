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

#ifndef AUDIOSETTINGSDIALOG_H
#define AUDIOSETTINGSDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include "AudioSettings.h"

namespace Ui {
class AudioSettingsDialog;
}

class AudioSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    enum ReturnCode { CANCEL, APPLY };

private:
    struct Setting
    {
        QString label;
        int value;
    };

public:
    explicit AudioSettingsDialog(QWidget* _parent = nullptr);
    ~AudioSettingsDialog();

    void setSettings(const AudioSettings& _settings);
    void getSettings(AudioSettings& _settings);

public slots:
    void onApply();
    void onCancel();

private:
    int _findSettingIndex(const QList<Setting>& _settings, const int _settingValue) const;

private:
    Ui::AudioSettingsDialog *ui;

    static const QList<Setting> s_frequencySettings;
    static const QList<Setting> s_bitPerSampleSettings;
    static const QList<Setting> s_channelCountSettings;
};

#endif // AUDIOSETTINGSDIALOG_H
