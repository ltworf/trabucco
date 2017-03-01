TEMPLATE = app
VERSION = 1.0

QT += qml widgets gui sql network
CONFIG += c++11

LIBS        += -lX11
QMAKE_LFLAGS += -Wl,--as-needed

RESOURCES += qml.qrc

HEADERS += \
    settings.h \
    singleinstance.h \
    action.h \
    desktopaction.h \
    tree.h \
    node.h \
    iconfinder.h \
    shortcutactivator.h \
    btree.h \
    btreeiterator.h \
    bookmarkaction.h \
    cache.h \
    downloader.h \
    searchaction.h \
    clipboard.h

SOURCES += main.cpp \
    settings.cpp \
    singleinstance.cpp \
    action.cpp \
    desktopaction.cpp \
    tree.cpp \
    node.cpp \
    iconfinder.cpp \
    shortcutactivator.cpp \
    btree.cpp \
    btreeiterator.cpp \
    bookmarkaction.cpp \
    cache.cpp \
    downloader.cpp \
    searchaction.cpp \
    clipboard.cpp

FORMS += \
    settings.ui

INSTALL += searchshortcuts

DISTFILES += main.qml extras/* \
    CHANGELOG
# searchproviders/*

isEmpty(target.path) {
    target.path = $${DESTDIR}/usr/bin
    export(target.path)
}
INSTALLS += target

searchproviders.path = $${DESTDIR}/usr/share/kservices5/searchproviders/
searchproviders.files = searchproviders/*
INSTALLS += searchproviders

launcher.path = $${DESTDIR}/usr/share/applications/
launcher.files = extras/trabucco.desktop
INSTALLS += launcher

manpage.path = $${DESTDIR}/usr/share/man/man1/
manpage.files = extras/trabucco.1
INSTALLS += manpage

icon.path = $${DESTDIR}/usr/share/icons/hicolor/512x512/apps
icon.files = extras/trabucco.png
INSTALLS += icon

export(INSTALLS)
