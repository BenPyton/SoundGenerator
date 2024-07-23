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

#include "FileManager.h"
#include <QFileDialog>
#include <QtCore>
#include "Utils.h"
#include "ProjectFile.h"
#include "NodalScene.h"
#include "Signal.h"

FileManager::FileManager(QWidget* _owner)
    : QObject(_owner), m_owner(_owner)
{
    QAction* act_clearRecentFiles = new QAction("Clear recent files", this);
    connect(act_clearRecentFiles, &QAction::triggered, this, &FileManager::clearRecentFiles);

    m_recentFileMenu = new QMenu("Open Recent", m_owner);
    for (int i = 0; i < MAX_RECENT_FILE; ++i)
    {
        m_recentFileActs[i] = new QAction(this);
        m_recentFileActs[i]->setVisible(false);
        connect(m_recentFileActs[i], &QAction::triggered, this, &FileManager::openRecent);
        m_recentFileMenu->addAction(m_recentFileActs[i]);
    }
    m_recentFileMenu->addSeparator();
    m_recentFileMenu->addAction(act_clearRecentFiles);
}

void FileManager::newFile()
{
    if(close(m_project, "creating a new project"))
    {
        m_project->scene()->reset();
        m_project->setFileName("");
    }
}

void FileManager::save()
{
    if(!m_project->fileName().isEmpty() && QFile::exists(m_project->fileName()))
    {
        saveFile(m_project->fileName(), m_project);
    }
    else // no file opened or file name is invalid
    {
        saveAs();
    }
}

void FileManager::saveAs()
{
    QString fileName = "";
    fileName = QFileDialog::getSaveFileName(m_owner, "Save Project", ".", "Project (*.proj)");

    if(!fileName.isEmpty()&& !fileName.isNull())
    {
        saveFile(fileName, m_project);
    }
}

void FileManager::open()
{
    if(close(m_project, "opening another project"))
    {
        QString fileName = QFileDialog::getOpenFileName(m_owner, "Open Project", ".", "Project (*.proj)");
        openFile(fileName, m_project);
    }
}

bool FileManager::close(ProjectFile* _project, QString _reason)
{
    bool success = false;
    QMessageBox::StandardButton reply = askSaveChanges(_reason, _project);
    if (reply != QMessageBox::Cancel)
    {
        if(reply == QMessageBox::Save)
        {
            save();
        }
        success = true;
    }
    return success;
}

void FileManager::openRecent()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
    {
        openFile(action->data().toString(), m_project);
    }
}

void FileManager::clearRecentFiles()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.clear();
    settings.setValue("recentFileList", files);

    updateRecentFileActions();
}

void FileManager::exportWAV()
{
    bool valid = true;
    QString fileName = "";
    do
    {
        fileName = QFileDialog::getSaveFileName(m_owner, "Export WAV", ".", "WAV (*.wav)");

        valid = true;
        if(QFile::exists(fileName))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(m_owner, "Confirm Save", "The file \"" + fileName + "\" already exists.\n Do you want to overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);

            if(reply != QMessageBox::Yes)
            {
                valid = false;
            }
        }
    } while (!valid);

    if(!fileName.isEmpty()&& !fileName.isNull())
    {
        m_project->signal()->exportWAV(fileName);
    }
}

bool FileManager::openFile(QString _fileName, ProjectFile* _project)
{
    bool success = false;
    if(QFile::exists(_fileName))
    {
        QFile file(_fileName);
        file.open(QIODevice::Text | QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument json = QJsonDocument::fromJson(data);
        _project->fromJson(json.object());
        _project->setFileName(_fileName);

        addToRecentFiles(_fileName);
        success = true;
    }
    return success;
}

void FileManager::saveFile(QString _fileName, ProjectFile* _project)
{
    _project->setFileName(_fileName);
    QJsonObject root = _project->toJson();
    QJsonDocument json(root);
    QByteArray data = json.toJson();

    QFile file(_fileName);
    file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(data);
    file.close();

    addToRecentFiles(_fileName);
}

void FileManager::addToRecentFiles(QString _fileName)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(_fileName);
    files.prepend(_fileName);
    while (files.size() > MAX_RECENT_FILE)
       files.removeLast();

    settings.setValue("recentFileList", files);

    updateRecentFileActions();
}

void FileManager::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), MAX_RECENT_FILE);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        m_recentFileActs[i]->setText(text);
        m_recentFileActs[i]->setData(files[i]);
        m_recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MAX_RECENT_FILE; ++j)
        m_recentFileActs[j]->setVisible(false);

    m_recentFileMenu->setEnabled(numRecentFiles > 0);
}

QMessageBox::StandardButton FileManager::askSaveChanges(QString _action, ProjectFile* _project)
{
    QMessageBox::StandardButton returnCode = QMessageBox::Ignore;
    if(_project->dirty())
    {
        returnCode = QMessageBox::question(m_owner, "Save your work!", "Some changes in your work are not saved.\nDo you want to save before " + _action + "?",
                                              QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
    }
    return returnCode;
}
