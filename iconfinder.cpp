#include "iconfinder.h"

#include <QFileInfo>



QString IconFinder::FindIcon(QString icon) {
    static const char * dirs[] = {
        "/usr/share/icons/oxygen/",
        "/usr/share/icons/hicolor/",
        "/usr/share/icons/breeze/",
        "/usr/share/icons/breeze-dark/",
        "/usr/share/pixmaps/",
    };

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

    static const char * formats[]= {
        ".svg",
        ".svgz",
        ".png",
    };

    for (unsigned int d=0; d< sizeof(dirs)/sizeof(int*); d++) {
        for (unsigned int p=0; p<sizeof(paths)/sizeof(int*); p++) {
            for (unsigned int f=0; f<sizeof(formats)/sizeof(int*); f++) {
                QString attempt = QString(dirs[d]) + QString(paths[p]) + icon + QString(formats[f]);
                printf("Attempting %s\n", attempt.toStdString().c_str());
                QFileInfo i(attempt);
                if (i.exists() && i.isReadable()) {
                    printf("Succeeded\n");
                    return attempt;
                }
            }
        }
    }



    return "";
}

