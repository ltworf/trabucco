#ifndef CACHE_H
#define CACHE_H

#include <QString>
#include <QUrl>

class Cache {
public:
    static void create_cache_dir();
    static QString cached_icon(QUrl);
    static void download_favicon(QUrl, QString, QObject* parent, QString default_icon = NULL);
};

#endif // CACHE_H
