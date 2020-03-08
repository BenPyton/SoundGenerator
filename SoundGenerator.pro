#-------------------------------------------------
#
# Project created by QtCreator 2019-09-29T11:41:12
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoundGenerator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += UI

CONFIG += c++11

SOURCES += \
    NodalScene.cpp \
    UndoCommands/ChangeInputValueCommand.cpp \
    UndoCommands/CreateComponentCommand.cpp \
    UndoCommands/DeleteComponentCommand.cpp \
    UndoCommands/LinkPinCommand.cpp \
    UndoCommands/MoveComponentCommand.cpp \
    UndoCommands/UnlinkPinCommand.cpp \
    Utils.cpp \
    WAVFormat.cpp \
        main.cpp \
        Components/AddComponent.cpp \
        Components/ADSRComponent.cpp \
        Components/Component.cpp \
        Components/ComponentFactory.cpp \
        Components/DelayComponent.cpp \
        Components/GeneratorComponent.cpp \
        Components/MultiplyComponent.cpp \
        Components/OutputComponent.cpp \
        Components/RandomComponent.cpp \
        Components/RepeatComponent.cpp \
        Components/SawToothComponent.cpp \
        Components/SinusComponent.cpp \
        Components/SquareComponent.cpp \
        Components/TriangleComponent.cpp \
        UI/mainwindow.cpp \
        UI/linkitem.cpp \
        UI/nodalview.cpp \
        UI/nodeitem.cpp \
        UI/pinitem.cpp \
        UI/waveformchunk.cpp \
        UI/waveformview.cpp \
        Random.cpp \
        Signal.cpp \
	UI/pinoutputitem.cpp \
	UI/pininputitem.cpp \
    UI/lineeditqreal.cpp \
    UI/pushordragbutton.cpp \
    UndoCommands/SelectComponentCommand.cpp

HEADERS += \
        Components/AddComponent.h \
        Components/ADSRComponent.h \
        Components/Component.h \
        Components/ComponentFactory.h \
        Components/DelayComponent.h \
        Components/GeneratorComponent.h \
        Components/MultiplyComponent.h \
        Components/OutputComponent.h \
        Components/RandomComponent.h \
        Components/RepeatComponent.h \
        Components/SawToothComponent.h \
        Components/SinusComponent.h \
        Components/SquareComponent.h \
        Components/TriangleComponent.h \
    NodalScene.h \
    Types.h \
        UI/mainwindow.h \
        UI/linkitem.h \
        UI/nodalview.h \
        UI/nodeitem.h \
        UI/pinitem.h \
        UI/waveformchunk.h \
        UI/waveformview.h \
        Random.h \
        Signal.h \
    UndoCommands/ChangeInputValueCommand.h \
    UndoCommands/CreateComponentCommand.h \
    UndoCommands/DeleteComponentCommand.h \
    UndoCommands/LinkPinCommand.h \
    UndoCommands/MoveComponentCommand.h \
    UndoCommands/UnlinkPinCommand.h \
    Utils.h \
    WAVFormat.h \
	UI/pinoutputitem.h \
	UI/pininputitem.h \
    UI/lineeditqreal.h \
    UI/pushordragbutton.h \
    UndoCommands/SelectComponentCommand.h

FORMS += \
        UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
