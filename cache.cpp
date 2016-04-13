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
