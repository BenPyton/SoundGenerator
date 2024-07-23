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

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "NodeItem.h"
#include "NodalView.h"
#include "WaveFormChunk.h"
#include "WaveFormView.h"
#include "PinInputItem.h"
#include "LineEditQReal.h"
#include "PushOrDragButton.h"
#include "NodalScene.h"
#include <QStyle>
#include <QTabBar>
#include "Utils.h"
#include "ActionCycle.h"
#include "AudioSettingsDialog.h"
#include "AudioSettings.h"
#include "ActionAudioDevice.h"
#include "Components/SuperComponentData.h"
#include "SuperComponentButton.h"
#include "SuperCompTabBar.h"
#include "SuperComponentDataManager.h"
#include "Components/ComponentFactory.h"
#include "FileManager.h"
#include "ProjectFile.h"

MainWindow::MainWindow(QWidget* _parent)
    : QMainWindow(_parent)
    , ui(new Ui::MainWindow)
    , m_autoGenerateMode(NO_GENERATION)
{
    ui->setupUi(this);


    m_project = new ProjectFile(this);
    ComponentFactory::SetDataManager(m_project->dataManager());

    m_fileManager = new FileManager(this);
    m_fileManager->setProject(m_project);

    // TODO: make components data-driven and load them from files instead of that hard coded list
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
    ui->waveFormView->setSignal(m_project->signal());
    connect(ui->waveFormView, &WaveFormView::zoomChanged, this, &MainWindow::onWaveFormViewZoomChanged);
    connect(ui->waveFormScrollBar, &QScrollBar::valueChanged, this, &MainWindow::onScrollbarValueChanged);
    connect(ui->timeRuler, &TimeRuler::onTimeSelected, this, &MainWindow::onTimeCursorChanged);
    connect(ui->waveFormScrollBar, &WaveFormScrollBar::pageStepChanged, this, &MainWindow::onScrollbarValueChanged);
    ui->waveFormScrollBar->setMaximum(5000);
    ui->waveFormScrollBar->setPageStep(1000);

    ui->sceneTabs->setMovable(true);
    ui->sceneTabs->setTabsClosable(true);
    ui->sceneTabs->setExpanding(false);
    ui->sceneTabs->setDrawBase(false);
    ui->sceneTabs->setElideMode(Qt::ElideRight);
    ui->sceneTabs->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    connect(ui->sceneTabs, &QTabBar::currentChanged, this, &MainWindow::displayScene);
    connect(ui->sceneTabs, &SuperCompTabBar::tabClosed, this, &MainWindow::tabClosed);
    connect(m_project->dataManager(), &SuperComponentDataManager::dataDeleted, ui->sceneTabs, &SuperCompTabBar::closeTab);

    connect(ui->newSuperCompButton, &QPushButton::clicked, this, &MainWindow::createSuperComponentData);

    connect(m_project->scene()->getOutput(), &NodeItem::outputChanged, this, &MainWindow::onOutputChanged);
    connect(m_project->signal(), &Signal::handleError, this, &MainWindow::onSignalError);

    // ===== Edit Menu =====

    QAction* act_selectAll = new QAction("Select All", this);
    act_selectAll->setShortcut(QKeySequence::SelectAll);
    connect(act_selectAll, &QAction::triggered, m_project->scene(), &NodalScene::selectAll);

    QAction* act_copy = new QAction("Copy", this);
    act_copy->setShortcut(QKeySequence::Copy);
    connect(act_copy, &QAction::triggered, m_project->scene(), &NodalScene::copyComponents);

    QAction* act_paste = new QAction("Paste", this);
    act_paste->setShortcut(QKeySequence::Paste);
    connect(act_paste, &QAction::triggered, m_project->scene(), &NodalScene::pasteComponents);

    QAction* act_undo = new QAction("Undo", this);
    act_undo->setShortcut(QKeySequence::Undo);
    connect(act_undo, &QAction::triggered, m_project->scene(), &NodalScene::undoCommand);

    QAction* act_redo = new QAction("Redo", this);
    act_redo->setShortcut(QKeySequence::Redo);
    connect(act_redo, &QAction::triggered, m_project->scene(), &NodalScene::redoCommand);

    ui->menuEdit->addAction(act_copy);
    ui->menuEdit->addAction(act_paste);
    ui->menuEdit->addAction(act_selectAll);
    ui->menuEdit->addAction(act_undo);
    ui->menuEdit->addAction(act_redo);

    // ===== Component Menu =====

    QMenu* menu_Add = new QMenu("Add", this);
    ui->menuComponents->addMenu(menu_Add);
    ui->nodalView->setMenuAdd(menu_Add);

    // Create commponents buttons and actions
    for(QString component : componentList)
    {
        QAction* action = new QAction(component, this);
        connect(action, &QAction::triggered, [this, component]() { currentScene()->createComponent(component); });

        PushOrDragButton* button = new PushOrDragButton(QIcon(Utils::GetComponentIconPath(component)), component);
        button->setProperty("class", "component");
        ui->buttonLayout->addWidget(button);
        connect(button, &QPushButton::clicked, action, &QAction::trigger);

        menu_Add->addAction(action);
    }

    // Create the "Add Input Component" action (it is visible only when editing Super Components)
    m_act_createInputComponent = new QAction("Input", this);
    connect(m_act_createInputComponent, &QAction::triggered, [this]() { currentScene()->createComponent("Input"); });
    menu_Add->addAction(m_act_createInputComponent);

    // ===== Audio Menu =====

    QAction* act_generate = new QAction(QIcon(":icons/tools/generate"), "Generate", this);
    act_generate->setShortcut(QKeySequence::Refresh);
    connect(act_generate, &QAction::triggered, m_project, &ProjectFile::generate);

    ActionCycle* act_play = new ActionCycle(this);
    act_play->addAction("Play", QIcon(":/icons/tools/play"));
    act_play->addAction("Pause", QIcon(":/icons/tools/pause"));
    act_play->setShortcut(QKeySequence("Space"));
    act_play->reset();
    connect(act_play, SIGNAL(triggered(int)), this, SLOT(playPause(int)));
    connect(m_project->signal(), SIGNAL(stopped()), act_play, SLOT(reset()));

    QAction* act_stop = new QAction(QIcon(":/icons/tools/stop"), "Stop", this);
    connect(act_stop, SIGNAL(triggered()), m_project->signal(), SLOT(stop()));

    QAction* act_loop = new QAction("Loop", this);
    act_loop->setCheckable(true);
    connect(act_loop, SIGNAL(triggered(bool)), m_project->signal(), SLOT(loop(bool)));

    m_act_autoGenerateGroup = new QActionGroup(this);
    m_act_autoGenerateGroup->addAction("No auto-generation")->setCheckable(true);
    m_act_autoGenerateGroup->addAction("Auto-generate on play")->setCheckable(true);
    m_act_autoGenerateGroup->addAction("Auto-generate on change")->setCheckable(true);
    m_act_autoGenerateGroup->actions()[0]->setChecked(true);
    connect(m_act_autoGenerateGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeAutoGenerate(QAction*)));

    QAction* act_audioSettings = new QAction("Settings", this);
    connect(act_audioSettings, SIGNAL(triggered()), this, SLOT(openAudioSettings()));
    m_audioSettingsDialog = new AudioSettingsDialog(this);
    connect(m_audioSettingsDialog, &AudioSettingsDialog::finished, this, &MainWindow::onAudioSettingsFinished);

    // ===== File Edit =====

    QAction* act_newFile = new QAction(QIcon(":/icons/tools/new"), "New", this);
    act_newFile->setShortcut(QKeySequence::New);
    connect(act_newFile, &QAction::triggered, m_fileManager, &FileManager::newFile);

    QAction* act_openFile = new QAction(QIcon(":/icons/tools/open"), "Open", this);
    act_openFile->setShortcut(QKeySequence::Open);
    connect(act_openFile, &QAction::triggered, m_fileManager, &FileManager::open);

    QAction* act_saveFile = new QAction(QIcon(":/icons/tools/save"), "Save", this);
    act_saveFile->setShortcut(QKeySequence::Save);
    connect(act_saveFile, &QAction::triggered, m_fileManager, &FileManager::save);

    QAction* act_saveAsFile = new QAction("Save as...", this);
    act_saveAsFile->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(act_saveAsFile, &QAction::triggered, m_fileManager, &FileManager::saveAs);

    QAction* act_exportWAV = new QAction(QIcon(":/icons/tools/export"), "Export to WAV", this);
    act_exportWAV->setShortcut(QKeySequence("Ctrl+E"));
    connect(act_exportWAV, &QAction::triggered, m_fileManager, &FileManager::exportWAV);

    QAction* act_quit = new QAction("Quit", this);
    act_quit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(act_quit, SIGNAL(triggered()), this, SLOT(quit()));

    QAction* act_about = new QAction("About", this);
    act_about->setShortcut(QKeySequence::HelpContents);
    connect(act_about, SIGNAL(triggered()), this, SLOT(about()));

    ui->menuBar->addAction(act_about);

    ui->menuFile->addAction(act_newFile);
    ui->menuFile->addAction(act_openFile);
    ui->menuFile->addMenu(m_fileManager->recentFileMenu());
    ui->menuFile->addAction(act_saveFile);
    ui->menuFile->addAction(act_saveAsFile);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(act_exportWAV);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(act_quit);

    ui->menuAudio->addAction(act_generate);
    ui->menuAudio->addAction(act_play);
    ui->menuAudio->addAction(act_stop);
    ui->menuAudio->addAction(act_loop);

    QMenu* menu_autoGenerate = ui->menuAudio->addMenu("Auto-Generation");
    menu_autoGenerate->addActions(m_act_autoGenerateGroup->actions());

    ActionAudioDevice* act_audioDevice = new ActionAudioDevice("Output Device", ui->menuAudio);
    connect(act_audioDevice, &ActionAudioDevice::deviceChanged, this, &MainWindow::onAudioDeviceChanged);
    ui->menuAudio->addMenu(act_audioDevice);

    ui->menuAudio->addAction(act_audioSettings);

    ui->mainToolBar->clear();
    ui->mainToolBar->addAction(act_newFile);
    ui->mainToolBar->addAction(act_openFile);
    ui->mainToolBar->addAction(act_saveFile);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(act_generate);
    ui->mainToolBar->addAction(act_play);
    ui->mainToolBar->addAction(act_stop);

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(spacer);

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

    // Initialize nodal view
    displayScene(-1); // show the main scene, and initialize all correctly
    ui->nodalView->setZoomLimits(0.1, 5.0);
    ui->nodalView->init();
    ui->nodalView->show();

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

    updateWindowTitle(m_project->fileName());

    m_project->setDirtyable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* _event)
{
    if (m_fileManager->close(m_project, "closing"))
    {
        QSettings settings;
        settings.setValue("geometry", saveGeometry());
        settings.setValue("windowState", saveState());
        _event->accept();
    }
    else
    {
        _event->ignore();
    }
}

void MainWindow::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

NodalScene* MainWindow::currentScene()
{
    return dynamic_cast<NodalScene*>(ui->nodalView->scene());
}

void MainWindow::mousePressEvent(QMouseEvent* _event)
{
    QWidget* focusedWidget = qApp->focusWidget();
    if(focusedWidget != nullptr && focusedWidget != childAt(_event->x(), _event->y()))
    {
        focusedWidget->clearFocus();
    }

    QMainWindow::mousePressEvent(_event);
}

void MainWindow::about()
{
    QMessageBox box(QMessageBox::NoIcon, "About " + Utils::AppName,
                    "Alpha " + Utils::GetAppVersion() + "<br><br>Copyright 2019-2020 " + Utils::CompanyName + "<br><br>\
                    This software is free and under <a style=\"color:#00FFFF\" href=\"https://www.gnu.org/licenses/gpl-3.0.txt\">GPLv3</a> license. <br>This software is dynamically linked to Qt5.<br>\
                    The source code of Qt SDK can be found <a style=\"color:#00FFFF\" href=\"http://download.qt.io/official_releases/qt/5.12/5.12.2/\">here</a>.<br>\
                    The full text for the GPLv3 license was distributed as a text file with this software.<br><br> \
                    Check the <a style=\"color:#00FFFF\" href=\"https://github.com/BenPyton/SoundGenerator\">github repository</a> for more information.",
                    QMessageBox::StandardButton::Ok, this);

    box.setTextFormat(Qt::RichText);
    box.exec();
}

void MainWindow::quit()
{
    if (m_fileManager->close(m_project, "quitting the application"))
    {
        qApp->quit();
    }
}

void MainWindow::playPause(int _index)
{
    if(m_autoGenerateMode == GENERATE_ON_PLAY)
    {
        m_project->signal()->generate(m_project->scene()->getOutput()->component());
    }

    switch(_index)
    {
    case 0:
        m_project->signal()->play();
        break;
    case 1:
        m_project->signal()->pause();
        break;
    default:
        Q_ASSERT(true);
        break;
    }
}

void MainWindow::setDirty(bool _wasPristine)
{
    if(_wasPristine)
    {
        updateWindowTitle(m_project->fileName());
    }
}

void MainWindow::changeVolume(int _value)
{
   m_project->signal()->setVolume(qreal(_value) / 100.0);
}

void MainWindow::changeAutoGenerate(QAction* _action)
{
    int actionIndex = -1;
    for(int i = 0; i < m_act_autoGenerateGroup->actions().size(); ++i)
    {
        if(_action == m_act_autoGenerateGroup->actions()[i])
        {
            actionIndex = i;
        }
    }

    Q_ASSERT(actionIndex >= 0 && actionIndex < AUTO_GENERATION_MODE_COUNT);
    m_autoGenerateMode = static_cast<AutoGenerationMode>(actionIndex);
}

void MainWindow::onOutputChanged()
{
    if(m_autoGenerateMode == GENERATE_ON_CHANGE)
    {
        m_project->signal()->generate(m_project->scene()->getOutput()->component());
    }
}

void MainWindow::onSignalError(Signal::Error _error)
{
    switch(_error)
    {
    case Signal::Error::NO_OUTPUT_DEVICE:
        QMessageBox::warning(this, "Audio Output Warning", "There is no output device: the sound will not play.");
        break;
    case Signal::Error::AUDIO_FORMAT_NOT_SUPPORTED:
        QMessageBox::warning(this, "Audio Output Warning", "The format of the sound is not suported by the output device: the sound will not play.");
        break;
    default:
        QMessageBox::critical(this, "Audio Output Error", "Unkown error: the sound will not play.");
        break;
    }
}

void MainWindow::openAudioSettings()
{
    qDebug() << "Open window";
    m_audioSettingsDialog->setSettings(m_project->signal()->settings());
    m_audioSettingsDialog->open();
}

void MainWindow::onAudioSettingsFinished(int _result)
{
    qDebug() << "Close window";
    if(_result == AudioSettingsDialog::APPLY)
    {
        m_audioSettingsDialog->getSettings(m_project->signal()->settings());
        m_project->signal()->updateAudioOutput(m_project->signal()->deviceInfo());
        m_project->signal()->generate(m_project->scene()->getOutput()->component());
    }
}

void MainWindow::onAudioDeviceChanged(const QAudioDeviceInfo& _info)
{
    qDebug() << "Device changed!!!";
    m_project->signal()->updateAudioOutput(_info);
}

void MainWindow::onWaveFormViewZoomChanged()
{
    int total = ui->waveFormView->getNbTotalSample();
    int offset = ui->waveFormView->getSampleOffset();
    int viewSize = ui->waveFormView->getNbSampleViewed();

    // update scrollbar
    ui->waveFormScrollBar->setMinimum(0);
    ui->waveFormScrollBar->setMaximum(qMax(0, total - viewSize));
    ui->waveFormScrollBar->setPageStep(viewSize);
    ui->waveFormScrollBar->setValue((viewSize < total) ? offset : 0);

    // update time ruler
    qreal sampleDuration = 1.0 / static_cast<qreal>(m_project->signal()->settings().sampleRate());
    ui->timeRuler->setTimeWindow(offset * sampleDuration, (offset + viewSize) * sampleDuration);
}

void MainWindow::onScrollbarValueChanged()
{
    int offset = ui->waveFormScrollBar->value();
    int viewSize = ui->waveFormScrollBar->pageStep();

    // update wave form view
    ui->waveFormView->setSampleOffset(offset);
    ui->waveFormView->setNbSampleViewed(viewSize);

    // update time ruler
    qreal sampleDuration = 1.0 / static_cast<qreal>(m_project->signal()->settings().sampleRate());
    ui->timeRuler->setTimeWindow(offset * sampleDuration, (offset + viewSize) * sampleDuration);
}

void MainWindow::onTimeCursorChanged(qreal _newTime)
{
    m_project->signal()->setCursorTime(_newTime);

    int cursorSample = qRound(_newTime * m_project->signal()->settings().sampleRate());
    ui->waveFormView->setCursorSample(cursorSample);
    ui->waveFormScrollBar->setCursorValue(cursorSample);
}

void MainWindow::createSuperComponentData()
{
    qDebug() << "Create Super Comp Data!";

    SuperComponentData* data = m_project->dataManager()->createData();
    connect(data, &SuperComponentData::onNameEdited, ui->sceneTabs, &SuperCompTabBar::renameTab);

    // Create super commponent button and its action
    QAction* action = new QAction(data->name(), this);
    connect(action, &QAction::triggered, [this, data]() { currentScene()->createComponent(data->uuid().toString()); });

    SuperComponentButton* button = new SuperComponentButton(data, data->name(), this);
    button->setProperty("class", "component");
    ui->superCompLayout->addWidget(button);
    connect(button, &QPushButton::clicked, action, &QAction::trigger);
    connect(button, &SuperComponentButton::onEditCalled, this, &MainWindow::onEditSuperComponent);
    connect(button, &SuperComponentButton::onDeleteCalled, this, &MainWindow::onDeleteSuperComponent);
}

void MainWindow::onDeleteSuperComponent(SuperComponentButton *_button)
{
    qDebug() << "Delete SuperComponent: " << _button->data()->name();
    QVector<NodeItem*> items = m_project->scene()->getSuperComponents(_button->data()->uuid());
    m_project->scene()->deleteNodes(items);

    m_project->dataManager()->deleteData(_button->data()->uuid());
    _button->deleteLater();
}

void MainWindow::onEditSuperComponent(SuperComponentData* _data)
{
    Q_ASSERT(_data != nullptr);

    qDebug() << "Edit SuperComponent: " << _data->name();
    ui->sceneTabs->editTab(_data);
    ui->nodalView->setScene(_data->scene());
}

void MainWindow::displayScene(int _tabIndex)
{
    qDebug() << "Index Changed: " << _tabIndex;
    if(_tabIndex < 0)
    {
        ui->nodalView->setScene(m_project->scene());
        m_act_createInputComponent->setVisible(false);
    }
    else
    {
        QUuid uuid = ui->sceneTabs->getTabDataUuid(_tabIndex);
        SuperComponentData* data = m_project->dataManager()->getData(uuid);
        if(data != nullptr)
        {
            qDebug() << "Data found: " << data->name();
            ui->nodalView->setScene(data->scene());
        }
        else
        {
            qDebug() << "No data found...";
        }
        m_act_createInputComponent->setVisible(true);
    }
}

void MainWindow::tabClosed(QUuid* _uuid)
{
    // Check if it is not triggered by data deletion
    if(nullptr != m_project->dataManager()->getData(*_uuid))
    {
        QVector<NodeItem*> items = m_project->scene()->getSuperComponents(*_uuid);
        for(int i = 0; i < items.count(); ++i)
        {
            items[i]->updateComponent();
        }
    }
}

void MainWindow::createActions()
{

}

void MainWindow::createMenus()
{

}

void MainWindow::updateWindowTitle(QString _fileName)
{
    qDebug() << "FileName:" << _fileName;
    if(_fileName.isNull() || _fileName.isEmpty())
    {
        m_windowTitle = "Untitled";
    }
    else
    {
        m_windowTitle = _fileName.mid(_fileName.lastIndexOf('/') + 1);
    }

    setWindowTitle(Utils::AppName + " [" + m_windowTitle + (m_project->dirty() ? " *" : "") + "]");
}
