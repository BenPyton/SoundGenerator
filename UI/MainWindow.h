/*
 * Copyright 2019-2020 Benoit Pelletier
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
#include <QWidget>
#include <QtWidgets>
#include "Signal.h"

class NodeItem;
class NodalScene;

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

    QString fileName() { return m_fileName; }
    void setFileName(QString _fileName);
    void readSettings();

protected:
    virtual void closeEvent(QCloseEvent* _event) override;
    virtual void mousePressEvent(QMouseEvent* _event) override;

public slots:
    // actions
    void newFile();
    void save();
    void saveAs();
    void open();
    void openRecent();
    void clearRecentFiles();
    void about();
    void exportWAV();
    void quit();
    void playPause(int _index);
    // other
    void setDirty();
    void changeVolume(int _value);
    void changeDuration(qreal _value);
    void changeAutoGenerate(QAction* _action);
    void onOutputChanged();

    void onWaveFormViewZoomChanged();
    void onScrollbarValueChanged();
    void onTimeCursorChanged(qreal _newTime);

private:
    void createActions();
    void createMenus();
    void openFile(QString _fileName);
    void saveFile(QString _fileName);
    QMessageBox::StandardButton askSaveChanges(QString _action);
    void updateWindowTitle();
    void updateRecentFileActions();

private:
    static const int maxRecentFile = 10;

    Ui::MainWindow *ui;
    NodalScene* m_scene;

    Signal m_signal;

    QString m_fileName;
    bool m_dirty;
    bool m_dirtyable;

    QString m_windowTitle;
    AutoGenerationMode m_autoGenerateMode;

    QAction* recentFileActs[maxRecentFile];
    QMenu* menu_openRecentFile;
    QActionGroup* m_act_autoGenerateGroup;
};

#endif // MAINWINDOW_H
