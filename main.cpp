#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QtQml>

#include "tree.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Tree t;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tree", &t);
    qmlRegisterType<Action>("random.namespace", 1, 0, "Action");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
