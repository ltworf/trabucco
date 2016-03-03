#include "bookmarkaction.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QFileInfo>

/**
 * @brief load_from_chromium
 * @param tree: add the Actions to this tree
 * @param value
 *
 * Recursively load bookmarks from chromium json file.
 *
 * The json has many levels, some can be done with objects
 * and some can be done with arrays.
 *
 * This function iterates the entire document until it finds
 * actual bookmarks and adds them.
 */
static void load_from_chromium(BTree *tree, QJsonValue *value) {
    if (value->isUndefined())
        return;

    QJsonObject obj;
    if (value->isObject()) {
        obj = value->toObject();
        QJsonValue type = obj.value("type");
        if (type.toString() == "url") {
            //Found a URL.
            QString name = obj.value("name").toString();
            QString url = obj.value("url").toString();

            if (name.length()!=0 && url.length() != 0) {
                tree->add(new BookmarkAction(name,url,""));
            }

        } else {
            QStringList keys = obj.keys();
            for (int i = 0; i < keys.length(); i++) {
                QJsonValue tmp = obj.value(keys[i]);
                load_from_chromium(tree, &tmp);
            }
        }
    } else if (value->isArray()) {
        QJsonArray array = value->toArray();
        for (int i=0; i< array.size(); i++) {
            QJsonValue tmp = array.at(i);
            load_from_chromium(tree, &tmp);
        }
    }
}

/**
 * @brief load_from_chromium
 * @param tree
 * @param chromium_bookmarks
 *
 * Load bookmarks actions from a chromium-like json file into the tree.
 */
static void load_from_chromium(BTree* tree, QFileInfo chromium_bookmarks) {
    if (chromium_bookmarks.exists() && chromium_bookmarks.isReadable()) {
        QFile bookmarks(chromium_bookmarks.absoluteFilePath());
        bookmarks.open(QIODevice::ReadOnly);
        QByteArray content = bookmarks.readAll();
        QJsonDocument document = QJsonDocument::fromJson(content);
        if (document.isObject()) {
            QJsonValue obj = document.object();
            load_from_chromium(tree, &obj);
        }

    }
}

/**
 * @brief BookmarkAction::LoadBookmarkActions
 * @param tree
 *
 * Loads bookmarks actions inside the tree
 */
void BookmarkAction::LoadBookmarkActions(BTree* tree) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QFileInfo chromium_bookmarks(env.value("HOME") + "/.config/chromium/Default/Bookmarks");

    load_from_chromium(tree, chromium_bookmarks);

}

QStringList* BookmarkAction::GetPaths() {
    return NULL;
}

BookmarkAction::BookmarkAction(QString name, QUrl url, QString icon, QObject* parent): Action(parent) {
    this->name = name;
    this->url = url;
}

void BookmarkAction::runAction() {
    QDesktopServices::openUrl(url);
}

QString BookmarkAction::getIcon() {
    return "";
}
