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
 * @brief load_from_firefox_places
 *
 * Load bookmark actions from a Firefox-like `places` SQLite database
 */
static void load_from_firefox_places(
	BTree *tree, //< [out] action tree to add actions to
	QString dbfile) //< path to a Firefox places database
{
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
		tree->add(new BookmarkAction(title, url, icon_url));
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
	QString path) //< path to a Firefox configuration directory
{
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
	load_from_firefox_places(tree, places_path);
	profiles.endGroup();

}

/**
 * @brief BookmarkAction::LoadBookmarkActions
 * @param tree
 *
 * Loads bookmarks actions inside the tree
 */
void BookmarkAction::LoadBookmarkActions(BTree* tree) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString config_root = env.value("XDG_CONFIG_HOME",
	    env.value("HOME") + "/.config");

    QFileInfo chromium_bookmarks(config_root + "/chromium/Default/Bookmarks");
    load_from_chromium(tree, chromium_bookmarks);

    QFileInfo opera_bookmarks(config_root + "/opera/Bookmarks");
    load_from_chromium(tree, opera_bookmarks);

    QFileInfo google_chrome(config_root + "/google-chrome/Default/Bookmarks");
    load_from_chromium(tree, google_chrome);

    QString ff_config_root = env.value("HOME") + "/.mozilla/firefox/";

    load_from_firefox(tree, ff_config_root);

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
