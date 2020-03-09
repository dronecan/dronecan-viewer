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
system(python3 script/build_version.py --prefix DCV --output src/build_info.hpp)

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
        src/helpers/ \
        src/node/ \
        ../protocol/src/ \
        ../protocol/static/ \
        ui_tmp/

SOURCES += \
        ../protocol/src/dronecan_parameter_packets.c \
        ../protocol/static/dronecan_id.c \
        ../protocol/src/DroneCANProtocol.c \
        ../protocol/src/dronecan_packets.c \
        ../protocol/src/dronecan_system_packets.c \
        ../protocol/src/fielddecode.c \
        ../protocol/src/fieldencode.c \
        ../protocol/src/scaleddecode.c \
        ../protocol/src/scaledencode.c \
        src/node/dronecan_node.cpp \
        main.cpp \
        mainwindow.cpp \
        src/can/adapter.cpp \
        src/can/dronecan_packet.cpp \
        src/can/dronecan_transfer.cpp \
        src/debug.cpp \
        src/directory.cpp \
        src/helpers/msg_box.cpp \
        src/node/node_decode.cpp \
        src/widget/about_widget.cpp \
        src/widget/can_connect_widget.cpp \
        src/widget/can_monitor_widget.cpp \
        src/widget/dc_widget.cpp \
        src/widget/device_list_widget.cpp

HEADERS += \
        ../protocol/src/dronecan_parameter_packets.h \
        ../protocol/static/dronecan_id.h \
        ../protocol/src/DroneCANProtocol.h \
        ../protocol/src/dronecan_packets.h \
        ../protocol/src/dronecan_system_packets.h \
        ../protocol/src/fielddecode.h \
        ../protocol/src/fieldencode.h \
        ../protocol/src/scaleddecode.h \
        ../protocol/src/scaledencode.h \
        ../protocol/static/dronecan_param.h \
        src/node/dronecan_node.hpp \
        mainwindow.hpp \
        src/build_info.hpp \
        src/can/adapter.hpp \
        src/can/dronecan_packet.hpp \
        src/can/dronecan_transfer.hpp \
        src/debug.hpp \
        src/directory.hpp \
        src/helpers/msg_box.hpp \
        src/version.hpp \
        src/widget/about_widget.hpp \
        src/widget/can_connect_widget.hpp \
        src/widget/can_monitor_widget.hpp \
        src/widget/dc_widget.hpp \
        src/widget/device_list_widget.hpp

FORMS += \
        ui/can_connect.ui \
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
