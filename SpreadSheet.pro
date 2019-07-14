#-------------------------------------------------
#
# Project created by QtCreator 2019-06-21T21:23:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpreadSheet
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

CONFIG += c++11

SOURCES += \
        Cell/cell.cpp \
        FindDialog/finddialog.cpp \
        GoToCell/gotocelldialog.cpp \
        Sort/sortdialog.cpp \
        Spreadsheet/spreadsheet.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        Cell/cell.h \
        FindDialog/finddialog.h \
        GoToCell/gotocelldialog.h \
        Sort/sortdialog.h \
        Spreadsheet/spreadsheet.h \
        mainwindow.h

FORMS += \
        Sort/sortdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc \
    resources/resources.qrc

DISTFILES += \
    resources/icons/aboutHelp.png \
    resources/icons/avoutQtHelp.png \
    resources/icons/copyEdit.png \
    resources/icons/cutEdit.png \
    resources/icons/deleteEdit.png \
    resources/icons/exitFile.png \
    resources/icons/findEdit.png \
    resources/icons/goToCellEdit.png \
    resources/icons/newFile.png \
    resources/icons/openFile.png \
    resources/icons/pasteEdit.png \
    resources/icons/recalculateTools.png \
    resources/icons/saveAsFile.png \
    resources/icons/saveFile.png \
    resources/icons/selectEdit.png \
    resources/icons/showGridOptions.png \
    resources/icons/sortTools.png \
    resources/icons/title.png
