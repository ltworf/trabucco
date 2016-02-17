#include "action.h"

#include <QQmlEngine>

Action::Action(QObject *parent) : QObject(parent) {
    QQmlEngine::setObjectOwnership(
        this,
        QQmlEngine::CppOwnership
    );
}

Action::Action(QString name, QString action, QString icon, QObject *parent): QObject(parent) {
    QQmlEngine::setObjectOwnership(
        this,
        QQmlEngine::CppOwnership
    );

    this->name = name;
    this->action = action;
    this->icon = icon;
}

QString Action::getName() {
    return this->name;
}
QString Action::getAction() {
    return this->action;
}
QString Action::getIcon() {
    return this->icon;
}

void Action::runAction() {}
