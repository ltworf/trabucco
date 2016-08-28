TEMPLATE = app
VERSION = 1.0

QT += qml widgets gui sql network
CONFIG += c++11

LIBS        += -lX11
QMAKE_LFLAGS += -Wl,--as-needed

SOURCES += main.cpp \
    settings.cpp \
    singleinstance.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    settings.h \
    singleinstance.h

FORMS += \
    settings.ui

INSTALL += searchshortcuts

DISTFILES += main.qml trabucco.gif extras/*
# searchproviders/* 
