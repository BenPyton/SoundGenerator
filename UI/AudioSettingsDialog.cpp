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

#include "AudioSettingsDialog.h"
#include "ui_audiosettingsdialog.h"

const QList<AudioSettingsDialog::Setting> AudioSettingsDialog::s_frequencySettings
{
    { "8000 Hz", 8000 },
    { "11025 Hz", 11025 },
    { "16000 Hz", 16000 },
    { "22050 Hz", 22050 },
    { "32000 Hz", 32000 },
    { "44100 Hz", 44100 },
    { "48000 Hz", 48000 }
};

const QList<AudioSettingsDialog::Setting> AudioSettingsDialog::s_bitPerSampleSettings
{
    { "8", 8 },
    { "16", 16 },
    //{ "24", 24 }, // TODO
    { "32", 32 }
};

const QList<AudioSettingsDialog::Setting> AudioSettingsDialog::s_channelCountSettings
{
    { "Mono", 1 },
    // { "Stereo", 2 } // TODO: Manage multiple channels
};

AudioSettingsDialog::AudioSettingsDialog(QWidget* _parent) :
    QDialog(_parent),
    ui(new Ui::AudioSettingsDialog)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);

    for(Setting setting : s_frequencySettings)
    {
        ui->box_sampleRate->addItem(setting.label);
    }

    for(Setting setting : s_bitPerSampleSettings)
    {
        ui->box_bitPerSample->addItem(setting.label);
    }

    for(Setting setting : s_channelCountSettings)
    {
        ui->box_channelCount->addItem(setting.label);
    }

    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    assert(applyButton != nullptr);
    applyButton->setAutoDefault(false);
    applyButton->setDefault(false);
    connect(applyButton, &QPushButton::clicked, this, &AudioSettingsDialog::onApply);

    QPushButton* cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    assert(cancelButton != nullptr);
    cancelButton->setAutoDefault(false);
    cancelButton->setDefault(false);
    connect(cancelButton, &QPushButton::clicked, this, &AudioSettingsDialog::onCancel);
}

AudioSettingsDialog::~AudioSettingsDialog()
{
    delete ui;
}

void AudioSettingsDialog::setSettings(const AudioSettings& _settings)
{
    ui->box_duration->setValue(_settings.duration());
    ui->box_sampleRate->setCurrentIndex(_findSettingIndex(s_frequencySettings, _settings.sampleRate()));
    ui->box_bitPerSample->setCurrentIndex(_findSettingIndex(s_bitPerSampleSettings, _settings.sampleSize()));
    ui->box_channelCount->setCurrentIndex(_findSettingIndex(s_channelCountSettings, _settings.channelCount()));
    qDebug() << "Get from settings!";
}

void AudioSettingsDialog::getSettings(AudioSettings& _settings)
{
    _settings.setDuration(ui->box_duration->value());
    _settings.setSampleRate(s_frequencySettings[ui->box_sampleRate->currentIndex()].value);
    _settings.setSampleSize(s_bitPerSampleSettings[ui->box_bitPerSample->currentIndex()].value);
    _settings.setChannelCount(s_channelCountSettings[ui->box_channelCount->currentIndex()].value);
    qDebug() << "Apply to settings!";
}

void AudioSettingsDialog::onApply()
{
    done(ReturnCode::APPLY);
}

void AudioSettingsDialog::onCancel()
{
    done(ReturnCode::CANCEL);
}

int AudioSettingsDialog::_findSettingIndex(const QList<Setting>& _settings, const int _settingValue) const
{
    for(int i = 0; i < _settings.length(); ++i)
    {
        if(_settings[i].value == _settingValue)
        {
            return i;
        }
    }
    return -1;
}
