#include "searchaction.h"

#include <QProcessEnvironment>
#include <QDirIterator>
#include <QFileInfo>

SearchAction::SearchAction(QString file, QObject *parent): Action(parent) {

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
            SearchAction * action = new SearchAction(path);
            if (action->mustShow())
                tree->add(action);
        }

    }
}

bool SearchAction::mustShow() {
    return true;
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
