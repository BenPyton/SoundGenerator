/*
 * Copyright 2019 Benoit Pelletier
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodeitem.h"
#include "nodalview.h"
#include "waveformchunk.h"
#include "waveformview.h"
#include "pininputitem.h"
#include "lineeditqreal.h"
#include <QStyle>
#include <utils.h>

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define BUGFIX_VERSION 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dirty(false),
    m_dirtyable(false)
{
    ui->setupUi(this);

    QString componentList[] = {
        "Sinusoidal",
        "Square",
        "Triangle",
        "Saw Tooth",
        "Random",
        "Add",
        "Multiply",
        "Repeat",
        "Delay",
        "ADSR"
    };


    ui->buttonLayout->setAlignment(Qt::Alignment(Qt::AlignTop | Qt::AlignHCenter));
    ui->waveFormView->setScrollBar(ui->waveFormScrollBar);
    ui->waveFormView->setSignal(&m_signal);

    scene = new QGraphicsScene(ui->nodalView);
    scene->setBackgroundBrush(Qt::black);

    ui->nodalView->setScene(scene);
    ui->nodalView->setZoomLimits(0.1, 5.0);
    ui->nodalView->init();
    ui->nodalView->show();
    connect(ui->nodalView, SIGNAL(dirtyChanged()), this, SLOT(setDirty()));

    m_signal.setComponent(ui->nodalView->getOutput()->component());

    // ===== Edit Menu =====

    QAction* act_selectAll = new QAction("Select All", this);
    act_selectAll->setShortcut(QKeySequence::SelectAll);
    connect(act_selectAll, SIGNAL(triggered()), ui->nodalView, SLOT(selectAll()));

    QAction* act_copy = new QAction("Copy", this);
    act_copy->setShortcut(QKeySequence::Copy);
    connect(act_copy, SIGNAL(triggered()), ui->nodalView, SLOT(copyComponents()));

    QAction* act_paste = new QAction("Paste", this);
    act_paste->setShortcut(QKeySequence::Paste);
    connect(act_paste, SIGNAL(triggered()), ui->nodalView, SLOT(pasteComponents()));

    ui->menuEdit->addAction(act_copy);
    ui->menuEdit->addAction(act_paste);
    ui->menuEdit->addAction(act_selectAll);

    // ===== Component Menu =====

    QMenu* menu_Add = new QMenu("Add", this);
    ui->menuComponents->addMenu(menu_Add);

    // Create commponents buttons and actions
    for(QString component : componentList)
    {
        QAction* action = new QAction(component, this);
        connect(action, &QAction::triggered, [this, component]() { ui->nodalView->createComponent(component); });

        QPushButton* button = new QPushButton(component);
        button->setProperty("class", "component");
        ui->buttonLayout->addWidget(button);
        connect(button, SIGNAL(clicked()), action, SLOT(trigger()));

        menu_Add->addAction(action);
    }

    // ===== Audio Menu =====

    QAction* act_generate = new QAction("Generate", this);
    act_generate->setShortcut(QKeySequence::Refresh);
    connect(act_generate, SIGNAL(triggered()), &m_signal, SLOT(generate()));

    QAction* act_play = new QAction("Play", this);
    act_play->setShortcut(QKeySequence("Space"));
    connect(act_play, SIGNAL(triggered()), &m_signal, SLOT(play()));

    QAction* act_loop = new QAction("Loop", this);
    act_loop->setCheckable(true);


    QAction* act_newFile = new QAction("New", this);
    act_newFile->setShortcut(QKeySequence::New);
    connect(act_newFile, SIGNAL(triggered()), this, SLOT(newFile()));

    QAction* act_openFile = new QAction("Open", this);
    act_openFile->setShortcut(QKeySequence::Open);
    connect(act_openFile, SIGNAL(triggered()), this, SLOT(open()));

    QAction* act_saveFile = new QAction("Save", this);
    act_saveFile->setShortcut(QKeySequence::Save);
    connect(act_saveFile, SIGNAL(triggered()), this, SLOT(save()));

    QAction* act_saveAsFile = new QAction("Save as...", this);
    act_saveAsFile->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(act_saveAsFile, SIGNAL(triggered()), this, SLOT(saveAs()));

    QAction* act_exportWAV = new QAction("Export to WAV", this);
    act_exportWAV->setShortcut(QKeySequence("Ctrl+E"));
    connect(act_exportWAV, SIGNAL(triggered()), this, SLOT(exportWAV()));

    QAction* act_quit = new QAction("Quit", this);
    act_quit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(act_quit, SIGNAL(triggered()), this, SLOT(quit()));


    QAction* act_about = new QAction("About", this);
    act_about->setShortcut(QKeySequence::HelpContents);
    connect(act_about, SIGNAL(triggered()), this, SLOT(about()));

    ui->menuBar->addAction(act_about);


    ui->menuFile->addAction(act_newFile);
    ui->menuFile->addAction(act_openFile);
    ui->menuFile->addAction(act_saveFile);
    ui->menuFile->addAction(act_saveAsFile);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(act_exportWAV);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(act_quit);

    ui->menuAudio->addAction(act_generate);
    ui->menuAudio->addAction(act_play);
    //ui->menuAudio->addAction(act_loop);

    ui->mainToolBar->clear();
    ui->mainToolBar->addAction(act_newFile);
    ui->mainToolBar->addAction(act_openFile);
    ui->mainToolBar->addAction(act_saveFile);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(act_generate);
    ui->mainToolBar->addAction(act_play);

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(spacer);


    QLabel* lbl_duration = new QLabel("Duration", this);
    lbl_duration->setStyleSheet("margin:5px;");
    LineEditQReal* edit_duration = new LineEditQReal(this);
    edit_duration->setMaximumWidth(100);
    connect(edit_duration, &QLineEdit::editingFinished, this, [this, edit_duration](){ edit_duration->clearFocus(); changeDuration(edit_duration->value()); });
    edit_duration->setValue(1.);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(lbl_duration);
    ui->mainToolBar->addWidget(edit_duration);

    QLabel* lbl_volume = new QLabel("Volume", this);
    lbl_volume->setStyleSheet("margin:5px;");
    QSlider* slider_volume = new QSlider(Qt::Horizontal);
    slider_volume->setRange(0, 100);
    slider_volume->setMaximumWidth(200);
    QLabel* lbl_volumePercent = new QLabel("100%", this);
    lbl_volumePercent->setMinimumWidth(50);
    lbl_volumePercent->setAlignment(Qt::AlignCenter);
    connect(slider_volume, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));
    connect(slider_volume, &QSlider::valueChanged, this, [lbl_volumePercent](int value){ lbl_volumePercent->setText(QString::number(value) + "%"); });
    slider_volume->setValue(30);
    ui->mainToolBar->addWidget(lbl_volume);
    ui->mainToolBar->addWidget(slider_volume);
    ui->mainToolBar->addWidget(lbl_volumePercent);

    // need to be done here for line edits in graphics scene
    qApp->setStyleSheet("\
    QLineEdit{\
        background-color: rgb(60, 60, 60);\
        border: 1px solid rgb(120, 120, 120);\
        color: rgb(200, 200, 200);\
        font-family: consolas;\
    }\
    QLineEdit:disabled{\
        color: rgb(100, 100, 100);\
    }");



    setFileName("");

    m_dirtyable = true;
}

QMenu* MainWindow::getMenu(QString name)
{
    QMenu* menu = nullptr;

    QList<QMenu*> menus = ui->menuBar->findChildren<QMenu*>();
    for(int i = 0; i < menus.size() && menu == nullptr; i++)
    {
        if(menus[i]->title() == name)
        {
            menu = menus[i];
        }
    }

    return menu;
}

void MainWindow::setFileName(QString fileName)
{
    m_fileName = fileName;
    qDebug() << "FileName:" << fileName;
    if(fileName.isNull() || fileName.isEmpty())
    {
        m_windowTitle = "Untitled";
    }
    else
    {
        m_windowTitle = fileName.mid(fileName.lastIndexOf('/') + 1);
    }
    m_dirty = false;
    updateWindowTitle();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = askSaveChanges("closing");

    if (reply != QMessageBox::Cancel)
    {
        if(reply == QMessageBox::Save)
        {
            save();
        }
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget* focusedWidget = qApp->focusWidget();
    if(focusedWidget != nullptr && focusedWidget != childAt(event->x(), event->y()))
    {
        focusedWidget->clearFocus();
    }

    QMainWindow::mousePressEvent(event);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::newFile()
{
    QMessageBox::StandardButton reply = askSaveChanges("creating a new project");
    if(reply != QMessageBox::Cancel)
    {
        if(reply == QMessageBox::Save)
        {
            save();
        }

        ui->nodalView->reset();

        setFileName("");
    }
}

void MainWindow::save()
{
    if(!m_fileName.isEmpty() && QFile::exists(m_fileName))
    {
        saveFile(m_fileName);
    }
    else // no file opened or file name is invalid
    {
        saveAs();
    }
}

void MainWindow::saveAs()
{
    QString fileName = "";
    fileName = QFileDialog::getSaveFileName(this, "Save Project", ".", "Project (*.proj)");

    if(!fileName.isEmpty()&& !fileName.isNull())
    {
        saveFile(fileName);
    }
}

void MainWindow::open()
{
    QMessageBox::StandardButton reply = askSaveChanges("opening another project");
    if(reply != QMessageBox::Cancel)
    {
        if(reply == QMessageBox::Save)
        {
            save();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Open Project", ".", "Project (*.proj)");

        if(QFile::exists(fileName))
        {
            m_dirtyable = false;
            qreal duration;
            ui->nodalView->load(fileName, duration);
            m_signal.setDuration(duration);
            m_dirtyable = true;
            setFileName(fileName);
        }
    }
}

void MainWindow::about()
{
    QString version = QString::number(MAJOR_VERSION) + "." +  QString::number(MINOR_VERSION) + "." + QString::number(BUGFIX_VERSION);

    QMessageBox box(QMessageBox::NoIcon, "About Sound Generator",
                    "Alpha " + version + "<br><br>Copyright 2019 Pelletier Benoit<br><br>\
                    This software is free and under <a style=\"color:#00FFFF\" href=\"https://www.gnu.org/licenses/gpl-3.0.txt\">GPLv3</a> license. <br>This software is dynamically linked to Qt5.<br>\
                    The source code of Qt SDK can be found <a style=\"color:#00FFFF\" href=\"http://download.qt.io/official_releases/qt/5.12/5.12.2/\">here</a>.<br>\
                    The full text for the GPLv3 license was distributed as a text file with this software.<br><br> \
                    Check the <a style=\"color:#00FFFF\" href=\"https://github.com/BenPyton/SoundGenerator\">github repository</a> for more information.",
                    QMessageBox::StandardButton::Ok, this);

    box.setTextFormat(Qt::RichText);
    box.exec();

}

void MainWindow::exportWAV()
{
    bool valid = true;
    QString fileName = "";
    do{
        fileName = QFileDialog::getSaveFileName(this, "Export WAV", ".", "WAV (*.wav)");

        valid = true;
        if(QFile::exists(fileName))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "Confirm Save", "The file \"" + fileName + "\" already exists.\n Do you want to overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);

            if(reply != QMessageBox::Yes)
            {
                valid = false;
            }
        }
    }while (!valid);

    if(!fileName.isEmpty()&& !fileName.isNull())
    {
        m_signal.exportWAV(fileName);
    }
}

void MainWindow::quit()
{
    QMessageBox::StandardButton reply = askSaveChanges("closing");

    if (reply != QMessageBox::Cancel)
    {
        if(reply == QMessageBox::Save)
        {
            save();
        }
        qApp->quit();
    }
}

void MainWindow::setDirty()
{
    if(!m_dirty && m_dirtyable)
    {
        m_dirty = true;
        updateWindowTitle();
    }
}

void MainWindow::changeVolume(int value)
{
   m_signal.setVolume(qreal(value) / 100.0);
}

void MainWindow::changeDuration(qreal value)
{
    m_signal.setDuration(value);
}

void MainWindow::createActions()
{

}

void MainWindow::createMenus()
{

}

void MainWindow::saveFile(QString fileName)
{
    setFileName(fileName);
    m_dirtyable = false;
    ui->nodalView->save(fileName, m_signal.duration());
    m_dirtyable = true;
}

QMessageBox::StandardButton MainWindow::askSaveChanges(QString action)
{
    return (!m_dirty) ? QMessageBox::Ignore
              : QMessageBox::question(this, "Save your work!", "Some changes in your work are not saved.\nDo you want to save before " + action + "?",
                                      QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle("Sound Generator [" + m_windowTitle + (m_dirty ? " *" : "") + "]");
}
