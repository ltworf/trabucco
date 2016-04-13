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
QString Action::getCornerIcon() {
    return "";
}
bool Action::hasCornerIcon() {
    return false;
}

bool Action::isPrefix() {
    return false;
}

void Action::runAction() {}

void Action::runAction(QString _) {}
