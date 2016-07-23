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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QtQml>
#include <QSystemTrayIcon>

#include "cache.h"
#include "tree.h"
#include "shortcutactivator.h"
#include "settings.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Trabucco");
    app.setOrganizationDomain("SiegeSoftware");

    Cache::create_cache_dir();

    Tree t;

    ShortcutActivator shortcut;


    shortcut.start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tree", &t);
    engine.rootContext()->setContextProperty("ShortcutX11", &shortcut);
    qmlRegisterType<Action>("trabucco.siegesoftware", 1, 0, "Action");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    QSystemTrayIcon tray;
    tray.setIcon(QIcon(":/trabucco.gif"));
    tray.show();
    tray.showMessage("Trabucco","Trabucco is running. Press Alt+Space", QSystemTrayIcon::Information, 2000);

    Settings settings;

    Settings::connect(
        &tray,
        &QSystemTrayIcon::activated,
        &settings,
        &Settings::show
    );

    return app.exec();
}
