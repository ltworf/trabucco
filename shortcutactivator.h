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

#ifndef SHORTCUTACTIVATOR_H
#define SHORTCUTACTIVATOR_H

#include <QThread>

class ShortcutActivator : public QThread {
    Q_OBJECT
public:
    explicit ShortcutActivator(QObject *parent = 0);
    void run();
signals:
    void activated();
    void grab_failed();

public slots:
    void end();
private:
    bool terminate = false;
    unsigned char key;
    unsigned int modifier;
};

#endif // SHORTCUTACTIVATOR_H
