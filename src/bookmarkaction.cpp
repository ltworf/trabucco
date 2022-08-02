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

Copyright (C) 2016-2022  Salvo "LtWorf" Tomaselli
Copyright (C) 2016 Giuseppe Bilotta
*/

#include "bookmarkaction.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QFileInfo>
#include <QtSql/QtSql>
#include <QSettings>

#include "cache.h"
#include "downloader.h"
#include "iconfinder.h"

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
static void load_from_chromium(BTree *tree, QJsonValue *value, QObject* parent) {
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
                BookmarkAction* bookmark = new BookmarkAction(name,url,"", parent);
                if (!tree->add(bookmark))
                    delete bookmark;
            }

        } else {
            QStringList keys = obj.keys();
            for (int i = 0; i < keys.length(); i++) {
                QJsonValue tmp = obj.value(keys[i]);
                load_from_chromium(tree, &tmp, parent);
            }
        }
    } else if (value->isArray()) {
        QJsonArray array = value->toArray();
        for (int i=0; i< array.size(); i++) {
            QJsonValue tmp = array.at(i);
            load_from_chromium(tree, &tmp, parent);
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
static void load_from_chromium(BTree* tree, QFileInfo chromium_bookmarks, QObject* parent) {
    if (chromium_bookmarks.exists() && chromium_bookmarks.isReadable()) {
        QFile bookmarks(chromium_bookmarks.absoluteFilePath());
        bookmarks.open(QIODevice::ReadOnly);
        QByteArray content = bookmarks.readAll();
        QJsonDocument document = QJsonDocument::fromJson(content);
        if (document.isObject()) {
            QJsonValue obj = document.object();
            load_from_chromium(tree, &obj, parent);
        }

    }
}

/**
 * @brief load_from_firefox_places
 *
 * Load bookmark actions from a Firefox-like `places` SQLite database
 */
static void load_from_firefox_places(
    BTree *tree, //< [out] action tree to add actions to
    QString dbfile, //< path to a Firefox places database
    QObject* parent
) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dbfile);
    if (!db.open()) {
        qDebug() << "Failed to open Firefox bookmarks @ " << dbfile << "\n";
    }

    qDebug() << "Loading Firefox bookmarks @ " << dbfile << "\n";

    QSqlQuery query(
        "select moz_bookmarks.title, moz_places.url, moz_favicons.url "
        "from moz_bookmarks, moz_places left join moz_favicons "
        "on moz_places.favicon_id = moz_favicons.id "
        "where moz_bookmarks.fk = moz_places.id;",
        db);

    while (query.next()) {
        QString title = query.value(0).toString();
        QString url = query.value(1).toString();

        // skip entry if it has no title or it's a special place or a bookmarklet
        if (title.isEmpty() || url.startsWith("place:") || url.startsWith("javascript:"))
            continue;

        QString icon_url = query.value(2).toString();
        BookmarkAction* bookmark = new BookmarkAction(title, url, icon_url, parent);
        if (!tree->add(bookmark))
            delete bookmark;
    }

    db.close();
}

/**
 * @brief load_from_firefox
 *
 * Load bookmark actions from the default Firefox profile found in a given directory
 */
static void load_from_firefox(
    BTree *tree, //< [out] action tree to add actions to
    QString path,//< path to a Firefox configuration directory
    QObject* parent
) {
    QSettings profiles(path + "profiles.ini", QSettings::IniFormat);

    QStringList groups(profiles.childGroups());
    groups.removeAll("General");

    QStringListIterator group(groups);
    QString section;

    // If there are multiple profiles defined, then we want the one with
    // Default=1; otherwise, we just want the first one. We could do conditionals
    // based on the number of elements in groups, but it's simpler to just
    // over the whole list, break if we meet one with Default=1, and
    // just leave the last one selected.
    while (group.hasNext()) {
        section = group.next();
        if (profiles.value(section + "/Default").toInt() == 1)
            break;
    }

    if (section.isEmpty()) {
        qDebug() << "No Firefox profile found\n";
        return;
    }

    profiles.beginGroup(section);

    qDebug() << "Loading from profile " << profiles.value("Name").toString() << "\n";

    // TODO FIXME handle IsRelative
    QString places_path = path + profiles.value("Path").toString() + "/places.sqlite";
    load_from_firefox_places(tree, places_path, parent);
    profiles.endGroup();

}

/**
 * @brief BookmarkAction::LoadBookmarkActions
 * @param tree
 *
 * Loads bookmarks actions inside the tree
 */
void BookmarkAction::LoadBookmarkActions(BTree* tree, QObject *parent) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString config_root = env.value("XDG_CONFIG_HOME",
                                    env.value("HOME") + "/.config");

    QFileInfo chromium_bookmarks(config_root + "/chromium/Default/Bookmarks");
    load_from_chromium(tree, chromium_bookmarks, parent);

    QFileInfo opera_bookmarks(config_root + "/opera/Bookmarks");
    load_from_chromium(tree, opera_bookmarks, parent);

    QFileInfo google_chrome(config_root + "/google-chrome/Default/Bookmarks");
    load_from_chromium(tree, google_chrome, parent);

    QString ff_config_root = env.value("HOME") + "/.mozilla/firefox/";

    load_from_firefox(tree, ff_config_root, parent);
}

QStringList* BookmarkAction::GetPaths() {
    static QStringList* result = NULL;
    if (result)
        return result;
    result = new QStringList();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString config_root = env.value("XDG_CONFIG_HOME",
                                    env.value("HOME") + "/.config");

    result->append(config_root + "/chromium/Default/Bookmarks");
    result->append(config_root + "/opera/Bookmarks");
    result->append(config_root + "/google-chrome/Default/Bookmarks");
    result->append(env.value("HOME") + "/.opera/bookmarks.adr");
    //TODO firefox path for bookmarks
    return result;
}

bool BookmarkAction::hasCornerIcon() {
    return true;
}

BookmarkAction::BookmarkAction(QString name, QUrl url, QString icon, QObject* parent): Action(parent) {
    this->name = name;
    this->url = url;
    this->icon = Cache::cached_icon(url);

    QFileInfo cached_icon(this->icon);
    if (!cached_icon.exists())
        Cache::download_favicon(url, this->icon, this, icon);

}

void BookmarkAction::runAction() {
    QDesktopServices::openUrl(url);
}

QString BookmarkAction::getIcon() {
    static QString browser = IconFinder::FindIcon("internet-web-browser");
    return browser;
}

QString BookmarkAction::getCornerIcon() {
    QFileInfo cached_icon(this->icon);

    //If the downloader managed to get an icon return it
    if (cached_icon.size() != 0)
        return this->icon;

    //Return the default icon
    return "";
}
