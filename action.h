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

#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QString>

class Action : public QObject {
    Q_OBJECT
public:
    explicit Action(QObject * parent=NULL);
    explicit Action(QString name, QString action, QString icon, QObject * parent=NULL);

signals:

public slots:
    QString getName();
    QString getAction();
    virtual QString getIcon();
    virtual void runAction();
    virtual void runAction(QString);
    virtual bool hasCornerIcon();
    virtual bool isPrefix();
    virtual QString getCornerIcon();

protected:
    QString name;
    QString action;
    QString icon;
};

#endif // ACTION_H
