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
Copyright (C) 2016 Giuseppe Bilotta
*/
#include <unistd.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QtQml>
#include <QSystemTrayIcon>

#include "cache.h"
#include "clipboard.h"
#include "iconfinder.h"
#include "tree.h"
#include "shortcutactivator.h"
#include "settings.h"
#include "singleinstance.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Trabucco");
    app.setOrganizationDomain("SiegeSoftware");
    bool wait = argc == 2 && QString(argv[1]) == "--wait";

    QString icon_path = IconFinder::FindIcon("trabucco");
    QSystemTrayIcon tray;
    tray.setIcon(QIcon(icon_path));
    tray.show();

    //Ensure unique instance, but allow for previous self to die
    if (!SingleInstance::unique("trabucco", SESSION)) {
        int retry = wait ? 30 : 0;

        while (retry > 0) {
            qDebug() << "wait..";
            if (SingleInstance::unique("trabucco", SESSION)) {
                goto is_unique;
            }
            retry--;
            sleep(1);
        }
        qDebug() << "Already running";
        tray.showMessage("Trabucco","Trabucco is already running.", QSystemTrayIcon::Warning, 4000);
        return 1;
    }
    is_unique:

    Cache::create_cache_dir();
    Tree t;

    ShortcutActivator shortcut;
    Settings settings;
    Clipboard clipboard;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("clipboard", &clipboard);
    engine.rootContext()->setContextProperty("tree", &t);
    engine.rootContext()->setContextProperty("ShortcutX11", &shortcut);
    engine.rootContext()->setContextProperty("trabucco_icon", QVariant::fromValue("file://" + icon_path));
    qmlRegisterType<Action>("trabucco.siegesoftware", 1, 0, "Action");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    Settings::connect(
        &tray,
        &QSystemTrayIcon::activated,
        &settings,
        &Settings::show
    );

    Settings::connect(
        &shortcut,
        &ShortcutActivator::grab_failed,
        &settings,
        &Settings::grab_error
    );

    shortcut.start();
    tray.showMessage("Trabucco","Trabucco is running. Press " + shortcut.shortcut_name(), QSystemTrayIcon::Information, 2000);
    int r = app.exec();
    if (r == CONFIG_RELOAD) {
        QStringList args;
        args.append("--wait");
        QProcess::startDetached(QApplication::applicationFilePath(), args);
    }
    return r;
}
