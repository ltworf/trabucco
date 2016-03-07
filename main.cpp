#include <QApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QtQml>
#include <QSystemTrayIcon>

#include "tree.h"
#include "shortcutactivator.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Trabucco");
    app.setOrganizationDomain("SiegeSoftware");

    Tree t;

    ShortcutActivator shortcut;


    shortcut.start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tree", &t);
    engine.rootContext()->setContextProperty("ShortcutX11", &shortcut);
    qmlRegisterType<Action>("trabucco.siegesoftware", 1, 0, "Action");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    QSystemTrayIcon tray;
    tray.setIcon(QIcon("qrc:/trabucco.gif"));
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
