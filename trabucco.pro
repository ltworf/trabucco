TEMPLATE = app

QT += qml quick widgets gui sql
CONFIG += c++11

LIBS        += -lX11

SOURCES += main.cpp \
    settings.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    settings.h

FORMS += \
    settings.ui
