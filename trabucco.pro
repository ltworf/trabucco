TEMPLATE = app
VERSION = 1.2

QT += qml widgets gui sql network
CONFIG += c++11 strict_c++

LIBS        += -lX11
QMAKE_LFLAGS += -Wl,--as-needed
QMAKE_CXXFLAGS += -Wextra

RESOURCES += qml.qrc

HEADERS += \
    src/settings.h \
    src/singleinstance.h \
    src/action.h \
    src/desktopaction.h \
    src/tree.h \
    src/node.h \
    src/iconfinder.h \
    src/shortcutactivator.h \
    src/btree.h \
    src/btreeiterator.h \
    src/bookmarkaction.h \
    src/cache.h \
    src/downloader.h \
    src/searchaction.h \
    src/clipboard.h \
    src/ynewaction.h

SOURCES += \
    src/main.cpp \
    src/settings.cpp \
    src/singleinstance.cpp \
    src/action.cpp \
    src/desktopaction.cpp \
    src/tree.cpp \
    src/node.cpp \
    src/iconfinder.cpp \
    src/shortcutactivator.cpp \
    src/btree.cpp \
    src/btreeiterator.cpp \
    src/bookmarkaction.cpp \
    src/cache.cpp \
    src/downloader.cpp \
    src/searchaction.cpp \
    src/clipboard.cpp \
    src/ynewaction.cpp

FORMS += \
    settings.ui

INSTALL += searchshortcuts

DISTFILES += main.qml extras/* \
    CHANGELOG \
    README.md \
    CODE_OF_CONDUCT.md \
    COPYING \
#    searchproviders/* \
    ynew/ynew \
    ynew/autocomplete/ynew \
    ynew/README.md

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
