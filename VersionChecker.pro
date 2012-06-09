QT       += network

QT       -= gui

TARGET = VersionChecker
TEMPLATE = lib
CONFIG += staticlib

SOURCES += VersionChecker.cpp

HEADERS += VersionChecker.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
