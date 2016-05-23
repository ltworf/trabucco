#include "iconfinder.h"

#include <QFileInfo>
#include <QIcon>
#include <QDebug>

static QStringList* search_paths() {
    static bool init = false;
    static QStringList dirs;

    if (!init) {
        init = true;
        QStringList themes;
        QStringList themes_paths = QIcon::themeSearchPaths();
        for (int i=0; i<themes_paths.length(); i++) {
            themes.append(themes_paths.at(i) + "/" + QIcon::themeName() + "/");
            themes.append(themes_paths.at(i) + "/hicolor/");
            themes.append(themes_paths.at(i) + "/breeze/");
            themes.append(themes_paths.at(i) + "/oxygen/");
        }
        themes.append("/usr/share/pixmaps/");

        static const char * paths[] = {
            "scalable/apps/",
            "512x512/apps/",
            "apps/512/",
            "256x256/apps/",
            "apps/256/",
            "192x192/apps/",
            "128x128/apps/",
            "apps/128/",
            "64x64/apps/",
            "apps/64/",
            "apps/48/",
            "32x32/apps/",
            "apps/32/",
            "apps/22/",
            "16x16/apps/",
            "apps/16/",
            "",
            "scalable/devices/",
            "512x512/devices/",
            "devices/512/",
            "256x256/devices/",
            "devices/256/",
            "192x192/devices/",
            "128x128/devices/",
            "devices/128/",
            "64x64/devices/",
            "devices/64/",
            "devices/48/",
            "32x32/devices/",
            "devices/32/",
            "devices/22/",
            "16x16/devices/",
            "/16/",
        };

        for (int i=0; i<themes.length(); i++) {
            for (unsigned int j=0;j<sizeof(paths)/sizeof(int*); j++) {
                QString dir = themes.at(i) + QString(paths[j]);
                QFileInfo info(dir);
                if (info.exists() && info.isDir() && !dirs.contains(dir)) {
                    dirs.append(dir);
                }
            }
        }
    }
    qDebug() << dirs;
    return &dirs;
}


QString IconFinder::FindIcon(QString icon) {
        static const char * formats[]= {
        ".svg",
        ".svgz",
        ".png",
    };

    //In case a full path is provided
    QFileInfo i(icon);
    if (i.exists() && i.isReadable()) {
        return icon;
    }

    //Strip format from icon field, if it has one
    for (unsigned int f=0; f<sizeof(formats)/sizeof(int*); f++) {
        if (icon.endsWith(formats[f])) {
            icon = icon.left(icon.length() - strlen(formats[f]));
        }
    }

    QStringList * dirs = search_paths();

    for (int d=0; d<dirs->length(); d++) {
        for (unsigned int f=0; f<sizeof(formats)/sizeof(int*); f++) {
            QString attempt = dirs->at(d) + icon + QString(formats[f]);
            QFileInfo i(attempt);
            if (i.exists() && i.isReadable()) {
                return attempt;
            }
        }
    }

    return "";
}

