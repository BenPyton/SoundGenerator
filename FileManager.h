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

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QMessageBox>
#include <QObject>
#include <QMenu>

class ProjectFile;

class FileManager : public QObject
{
    Q_OBJECT

public:
    FileManager(QWidget* _owner);

    inline QMenu* recentFileMenu() const { return m_recentFileMenu; }
    inline void setProject(ProjectFile* _project) { m_project = _project; }

public slots:
    void newFile();
    void save();
    void saveAs();
    void open();
    bool close(ProjectFile* _project, QString _reason);
    void openRecent();
    void clearRecentFiles();
    void exportWAV();

signals:
    void onSaved();
    void onLoaded();

private:
    bool openFile(QString _fileName, ProjectFile* _project);
    void saveFile(QString _fileName, ProjectFile* _project);
    void addToRecentFiles(QString _fileName);
    void updateRecentFileActions();
    QMessageBox::StandardButton askSaveChanges(QString _action, ProjectFile* _project);

private:
    static const int MAX_RECENT_FILE = 10;

    QWidget* m_owner;
    QAction* m_recentFileActs[MAX_RECENT_FILE];
    QMenu* m_recentFileMenu;
    ProjectFile* m_project;
};

#endif // SAVEMANAGER_H
