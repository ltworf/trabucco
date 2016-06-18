isEmpty(target.path) {
    target.path = $${DESTDIR}/usr/bin
    export(target.path)
}
INSTALLS += target

searchproviders.path = $${DESTDIR}/usr/share/kservices5/searchproviders/
searchproviders.files = searchproviders/*
INSTALLS += searchproviders


export(INSTALLS)

HEADERS += \
    $$PWD/action.h \
    $$PWD/desktopaction.h \
    $$PWD/tree.h \
    $$PWD/node.h \
    $$PWD/iconfinder.h \
    $$PWD/shortcutactivator.h \
    $$PWD/btree.h \
    $$PWD/btreeiterator.h \
    $$PWD/bookmarkaction.h \
    $$PWD/cache.h \
    $$PWD/downloader.h \
    $$PWD/searchaction.h

SOURCES += \
    $$PWD/action.cpp \
    $$PWD/desktopaction.cpp \
    $$PWD/tree.cpp \
    $$PWD/node.cpp \
    $$PWD/iconfinder.cpp \
    $$PWD/shortcutactivator.cpp \
    $$PWD/btree.cpp \
    $$PWD/btreeiterator.cpp \
    $$PWD/bookmarkaction.cpp \
    $$PWD/cache.cpp \
    $$PWD/downloader.cpp \
    $$PWD/searchaction.cpp
