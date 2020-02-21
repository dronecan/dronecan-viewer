#-------------------------------------------------
#
# Project created by QtCreator 2020-02-21T15:50:41
#
#-------------------------------------------------

QT       += core gui serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DroneCANViewer
TEMPLATE = app

# Automatically generate version information on each build
system(python script/build_version.py --prefix DCV --output src/build_info.hpp)

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

INCLUDEPATH += \
        src/ \
        src/can/ \
        src/widget/ \
        ui_tmp/

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        src/can/adapter.cpp \
        src/directory.cpp \
        src/widget/about_widget.cpp \
        src/widget/can_monitor_widget.cpp \
        src/widget/dc_widget.cpp \
        src/widget/device_list_widget.cpp

HEADERS += \
        mainwindow.hpp \
        src/build_info.hpp \
        src/can/adapter.hpp \
        src/debug.hpp \
        src/directory.hpp \
        src/version.hpp \
        src/widget/about_widget.hpp \
        src/widget/can_monitor_widget.hpp \
        src/widget/dc_widget.hpp \
        src/widget/device_list_widget.hpp

FORMS += \
        ui/can_viewer.ui \
        ui/device_list.ui \
        ui/mainwindow.ui \
        ui/about.ui

#Set the location for the generated ui_xxxx.h files
UI_DIR = ui_tmp/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Copy required files into the output directory
win32{
    CONFIG(release, debug|release){
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$quote($$shadowed($$PWD)\release\DroneCANViewer.exe)) $$shell_path($$quote($$PWD\wininstall\DroneCANViewer.exe)) $$escape_expand(\n\t)
        QMAKE_POST_LINK += $$[QT_INSTALL_BINS]\windeployqt -opengl -printsupport $$shell_path($$quote($$PWD\wininstall\DroneCANViewer.exe)) $$escape_expand(\n\t)
    }
}

RESOURCES += \
    resources.qrc
