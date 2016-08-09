#include <QLockFile>
#include <QProcessEnvironment>

#include "singleinstance.h"

/**
 * @brief filename
 * @param key
 * @param scope
 * @return a fully qualified filename
 *
 * Generates an appropriate filename for the lock
 */
static QString filename(QString key, scope_t scope) {

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString tmp = env.value("TEMP", "/tmp") + "/";
    QString user = env.value("USER", "alfio");


    QString r;
    switch (scope) {
        case SYSTEM:
            r = tmp;
            break;
        case SESSION:
            //TODO this will prevent trabucco to run in multiple sessions
            r = env.value("XDG_RUNTIME_DIR", tmp + user) + "/";
            break;
    }
    return r + key + ".lock";
}

/**
 * @brief SingleInstance::unique
 * @param key the unique name of the program
 * @param scope wether it needs to be system-wide or session-wide
 * @return true if this is the only instance
 *
 * Make sure that this instance is unique.
 */
bool SingleInstance::unique(QString key, scope_t scope) {
    QLockFile* lock = new QLockFile(filename(key, scope));
    bool r = lock->tryLock();
    if (!r)
        delete lock;
    return r;
}
