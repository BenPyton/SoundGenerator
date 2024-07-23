/*
 * Copyright 2019-2021 Benoit Pelletier
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <Signal.h>

class NodeItem;
class NodalScene;
class AudioSettingsDialog;
class ActionAudioDevice;
class SuperComponentData;
class SuperComponentButton;
class ProjectFile;
class FileManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum AutoGenerationMode
    {
        NO_GENERATION,
        GENERATE_ON_PLAY,
        GENERATE_ON_CHANGE,
        AUTO_GENERATION_MODE_COUNT // this value if error
    };

public:
    explicit MainWindow(QWidget* _parent = nullptr);
    virtual ~MainWindow() override;

    void readSettings();
    NodalScene* currentScene();

protected:
    virtual void closeEvent(QCloseEvent* _event) override;
    virtual void mousePressEvent(QMouseEvent* _event) override;

public slots:
    // actions
    void about();
    void quit();
    void playPause(int _index);
    void openAudioSettings();
    // other
    void setDirty(bool _asPristine);
    void changeVolume(int _value);
    void changeAutoGenerate(QAction* _action);
    void onOutputChanged();
    void onSignalError(Signal::Error _error);
    void onAudioSettingsFinished(int _result);
    void onAudioDeviceChanged(const QAudioDeviceInfo& _info);

    void onWaveFormViewZoomChanged();
    void onScrollbarValueChanged();
    void onTimeCursorChanged(qreal _newTime);

    // super components
    void createSuperComponentData();
    void onDeleteSuperComponent(SuperComponentButton* _button);
    void onEditSuperComponent(SuperComponentData* _data);
    void displayScene(int _tabIndex);
    void tabClosed(QUuid* _uuid);

private:
    void createActions();
    void createMenus();
    void updateWindowTitle(QString _fileName);
    //void updateRecentFileActions();

private:
    //static const int maxRecentFile = 10;

    Ui::MainWindow *ui;
    QString m_windowTitle;
    AutoGenerationMode m_autoGenerateMode;

    QActionGroup* m_act_autoGenerateGroup;

    AudioSettingsDialog* m_audioSettingsDialog;
    QAction* m_act_createInputComponent;

    ProjectFile* m_project;
    FileManager* m_fileManager;
};

#endif // MAINWINDOW_H
