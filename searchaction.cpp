#include "searchaction.h"

#include <QProcessEnvironment>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>
#include <QUrl>

SearchAction::SearchAction(QString name, QString query, bool hidden, QObject *parent): Action(parent) {
    this->name = name + ":";
    this->action = query;
    this->show = ! hidden;

    //Hide invalid entries
    if (!name.size() || !query.size())
        this->show = false;
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
        result->append(home + "/.kde/share/kde4/services/searchproviders");
    }

    return result;
}

//TODO de-duplicate this code with the one in desktopaction.cpp
static void iterate_dir(BTree* tree, QString dir) {

    // Include subdirectories and follow links
    QDirIterator i(dir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

    while (i.hasNext()) {
        QString path = i.next();

        if (!path.endsWith(".desktop")) {
            continue;
        }

        QFileInfo info(path);

        if (info.isFile() && info.isReadable()) {

            QList<SearchAction*> actions = SearchAction::LoadFile(path);
            for (int i=0; i < actions.size(); i++) {
                SearchAction* action = actions.at(i);
                if (action->mustShow())
                    tree->add(action);
                else
                    delete action;
            }
        }

    }
}

bool SearchAction::mustShow() {
    return this->show;
}

void SearchAction::LoadSearchActions(BTree* tree) {
    QStringListIterator i(*SearchAction::GetPaths());

    while (i.hasNext()) {
        iterate_dir(tree, i.next());
    }
}


bool SearchAction::isPrefix() {
    return true;
}
QString SearchAction::getIcon() {
    return "";
}

void SearchAction::runAction(QString query) {
    QString url = this->action;
    QByteArray encoded = QUrl::toPercentEncoding(query);
    url.replace("\\{@}", encoded);

    QDesktopServices::openUrl(url);
}
