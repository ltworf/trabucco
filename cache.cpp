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
*/

#include <QCryptographicHash>
#include <QString>
#include <QStringList>
#include <QStandardPaths>
#include <QDir>

#include "cache.h"
#include "downloader.h"

/**
 * @brief Cache::create_cache_dir
 *
 * Make sure that the cache directory exists.
 */
void Cache::create_cache_dir() {
    QString cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir;
    if (!dir.exists(cache_path)) {
        dir.mkdir(cache_path);
    }
}

/**
 * @brief Create an hash path to store the favicon of a website
 * @param favicon
 * @return
 */
QString Cache::cached_icon(QUrl url) {
    url.setPath("/favicon.ico");
    url.setQuery("");

    QByteArray hash = QCryptographicHash::hash(
                          url.toString().toLatin1(),
                          QCryptographicHash::Md5
                      );

    QString cache_dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    return cache_dir + "/" + hash.toHex();
}

void Cache::download_favicon(QUrl url, QString destination, QObject* parent, QString default_icon) {
    url.setQuery("");

    //Create empty destination file
    QFile cache(destination);
    cache.open(QIODevice::WriteOnly);
    cache.close();

    QStringList icons;

    //The icon indicated in the bookmark
    if (default_icon.size() != 0 && ! default_icon.startsWith("blob:")) {
        icons.append(default_icon);
    }

    url.setPath("//apple-touch-icon.png");
    icons.append(url.toString());

    url.setPath("/favicon.ico");
    icons.append(url.toString());

    //Setting this as parent to free the memory of it
    new Downloader(icons, destination, parent);
}
