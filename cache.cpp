#include <QString>
#include <QStandardPaths>
#include <QDir>

#include "cache.h"

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
