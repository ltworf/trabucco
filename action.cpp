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

void Action::runAction(QString /* unused */) {}
