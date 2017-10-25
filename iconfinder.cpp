/*
This file is part of Trabucco.

Trabucco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Trabucco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Trabucco.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2016  Salvo "LtWorf" Tomaselli
Copyright (C) 2016 Giuseppe Bilotta
*/

#include "iconfinder.h"

#include <QFileInfo>
#include <QIcon>
#include <QDebug>
#include <QDir>
#include <QProcessEnvironment>

static QStringList* search_paths() {
    static bool init = false;
    static QStringList dirs;

    if (!init) {
        init = true;
        QStringList themes;
        QStringList themes_paths = QIcon::themeSearchPaths();

        /* icons and pixmaps under XDG_DATA_HOME (~/.local/share/) seem to be missing from Qt's default
         * search paths, while XDG_DATA_DIR ones are present. Add the HOME ones
         */
        {
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            QString home_local_share = env.value("XDG_DATA_HOME", QDir::homePath() + "/.local/share/");
            QString home_icons = QDir::cleanPath(home_local_share + "/icons");
            QString home_pixmaps = QDir::cleanPath(home_local_share + "/pixmaps");

            qDebug() << home_local_share;
            qDebug() << home_icons;
            qDebug() << home_pixmaps;

            QFileInfo info(home_icons);
            if (info.exists() && info.isDir()) {
                themes_paths.prepend(home_icons);
            }
            info = QFileInfo(home_pixmaps);
            if (info.exists() && info.isDir()) {
                themes_paths.prepend(home_pixmaps);
            }
            themes_paths.append("/usr/share/pixmaps");
            themes_paths.removeDuplicates();
            QIcon::setThemeSearchPaths(themes_paths);
        }

        qDebug() << themes_paths;

        for (int i=0; i<themes_paths.length(); i++) {
            themes.append(themes_paths.at(i) + "/" + QIcon::themeName() + "/");
            themes.append(themes_paths.at(i) + "/" + QIcon::themeName() + "/base/");
            themes.append(themes_paths.at(i) + "/breeze/");
            themes.append(themes_paths.at(i) + "/oxygen/");
            themes.append(themes_paths.at(i) + "/oxygen/base/");
            themes.append(themes_paths.at(i) + "/hicolor/");
            themes.append(themes_paths.at(i) + "/");
        }

        static const char * paths[] = {
            "scalable/apps/",
            "1024x1024/apps/",
            "apps/1024/",
            "512x512/apps/",
            "apps/512/",
            "256x256/apps/",
            "apps/256/",
            "192x192/apps/",
            "128x128/apps/",
            "apps/128/",
            "64x64/apps/",
            "apps/64/",
            "48x48/apps/",
            "apps/48/",
            "32x32/apps/",
            "apps/32/",
            "24x24/apps/",
            "apps/24/",
            "22x22/apps/",
            "apps/22/",
            "16x16/apps/",
            "apps/16/",
            "",
            "scalable/devices/",
            "1024x1024/devices/",
            "devices/1024/",
            "512x512/devices/",
            "devices/512/",
            "256x256/devices/",
            "devices/256/",
            "192x192/devices/",
            "128x128/devices/",
            "devices/128/",
            "64x64/devices/",
            "devices/64/",
            "48x48/devices/",
            "devices/48/",
            "32x32/devices/",
            "devices/32/",
            "24x24/devices/",
            "devices/24/",
            "22x22/devices/",
            "devices/22/",
            "16x16/devices/",
            "devices/16/",
            "scalable/places/",
            "512x512/places/",
            "256x256/places/",
            "192x192/places/",
            "128x128/places/",
            "96x96/places/",
            "72x72/places/",
            "64x64/places/",
            "48x48/places/",
            "36x36/places/",
            "32x32/places/",
            "24x24/places/",
            "22x22/places/",
            "16x16/places/",
        };

        for (int i=0; i<themes.length(); i++) {
            for (unsigned int j=0;j<sizeof(paths)/sizeof(int*); j++) {
                QString dir = themes.at(i) + QString(paths[j]);
                QFileInfo info(dir);
                if (info.exists() && info.isDir() && !dirs.contains(dir) && QDir(dir).count() > 2) {
                    qDebug() << "Adding" << dir << "to icon search path";
                    dirs.append(dir);
                }
            }
        }
        qDebug() << dirs << "\n";
    }
    return &dirs;
}


QString IconFinder::FindIcon(QString icon) {
    static const char * formats[]= {
        ".svg",
        ".svgz",
        ".png",
    };

    qDebug() << "finding " << icon;

    qDebug() << "discoverable as theme icon: " << QIcon::hasThemeIcon(icon);

    //In case a full path is provided
    QFileInfo i(icon);
    if (icon.startsWith('/') && i.exists() && i.isReadable()) {
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
            qDebug() << "Attempting icon" << attempt;
            if (i.exists() && i.isReadable()) {
                return attempt;
            }
        }
    }

    qDebug() << icon << " not found" << "\n";

    return "";
}

