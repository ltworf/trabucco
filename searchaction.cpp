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

#include "searchaction.h"

#include <QProcessEnvironment>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>
#include <QUrl>

#include "cache.h"
#include "iconfinder.h"

static QString get_url(QString action, QString query) {
    action.replace("\\{@}", QUrl::toPercentEncoding(query));
    return action;
}

SearchAction::SearchAction(QString name, QString query, bool hidden, QObject *parent): Action(parent) {
    this->name = name + ":";
    this->action = query;
    this->show = ! hidden;

    //Hide invalid entries
    if (!name.size() || !query.size())
        this->show = false;
    if (this->show == false)
        return;


    QUrl url = get_url(this->action, "");
    this->icon = Cache::cached_icon(url);
    QFileInfo cached_icon(this->icon);
    if (!cached_icon.exists())
        Cache::download_favicon(url, this->icon, this);
}

QList<SearchAction*> SearchAction::LoadFile(QString file, QObject* parent) {
    QList<SearchAction*> r;
    QSettings settings(file, QSettings::IniFormat);
    QString charset = settings.value("Desktop Entry/Charset", "").toString();
    if (charset == "")
        charset = "UTF-8";
    settings.setIniCodec(charset.toStdString().c_str());

    //Some validation
    {
        QString type = settings.value("Desktop Entry/Type","").toString();

        if (type != "Service")
            return r;
    }

    QString query = settings.value("Desktop Entry/Query","").toString();
    bool hidden = settings.value("Desktop Entry/Hidden","").toBool();
    QStringList keys = settings.value("Desktop Entry/Keys","").toStringList();

    for (int i = 0; i < keys.size(); i++) {
        SearchAction* action = new SearchAction(keys.at(i), query, hidden, parent);
        r.append(action);
    }

    return r;
}

QStringList* SearchAction::GetPaths() {
    static QStringList* result = NULL;
    if (result)
        return result;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();


    result = new QStringList();
    result->append("/usr/share/kservices5/searchproviders/");

    if (env.contains("HOME")) {
        QString home = env.value("HOME");
        QString xdg_data_home = env.value("XDG_DATA_HOME", home + "/.local/share/");
        result->append(home + "/.kde/share/kde4/services/searchproviders");
        result->append(xdg_data_home + "/kservices5/searchproviders");
    }

    return result;
}

//TODO de-duplicate this code with the one in desktopaction.cpp
static void iterate_dir(BTree* tree, QString dir, QObject* parent) {

    // Include subdirectories and follow links
    QDirIterator i(dir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

    while (i.hasNext()) {
        QString path = i.next();

        if (!path.endsWith(".desktop")) {
            continue;
        }

        QFileInfo info(path);

        if (info.isFile() && info.isReadable()) {

            QList<SearchAction*> actions = SearchAction::LoadFile(path, parent);
            for (int i=0; i < actions.size(); i++) {
                SearchAction* action = actions.at(i);
                if (!action->mustShow() || !tree->add(action))
                    delete action;
            }
        }

    }
}

bool SearchAction::mustShow() {
    return this->show;
}

void SearchAction::LoadSearchActions(BTree* tree, QObject *parent) {
    QStringListIterator i(*SearchAction::GetPaths());

    while (i.hasNext()) {
        iterate_dir(tree, i.next(), parent);
    }
}

bool SearchAction::isPrefix() {
    return true;
}

bool SearchAction::hasCornerIcon() {
    return true;
}

QString SearchAction::getCornerIcon() {
    QFileInfo cached_icon(this->icon);

    //If the downloader managed to get an icon return it
    if (cached_icon.size() != 0)
        return this->icon;

    //Return the default icon
    return "";
}

QString SearchAction::getIcon() {
    static QString browser = IconFinder::FindIcon("internet-web-browser");
    return browser;
}

void SearchAction::runAction(QString query) {
    QDesktopServices::openUrl(get_url(this->action, query));
}
