#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QtQml>

#include "tree.h"
#include "shortcutactivator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Tree t;

    ShortcutActivator shortcut;

    shortcut.start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tree", &t);
    engine.rootContext()->setContextProperty("ShortcutX11", &shortcut);
    qmlRegisterType<Action>("random.namespace", 1, 0, "Action");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
