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

Copyright (C) 2016-2022  Salvo "LtWorf" Tomaselli
Copyright (C) 2016 Giuseppe Bilotta
*/

#include "shortcutactivator.h"

#include <QDebug>
#include <QSettings>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>

bool grab_error = False;

int xerrorhandler(Display *, XErrorEvent * ev) {
    if (ev->error_code == BadAccess) {
        qDebug() << "Unable to grab global shortcut";
        grab_error = True;
    }
    return 0;
}

ShortcutActivator::ShortcutActivator(QObject *parent) : QThread(parent) {
    QSettings settings;

    const char* k = settings.value("Shortcut/keycode","space").toString().toStdString().c_str();
    this->key = XStringToKeysym(k);
    this->modifier = settings.value("Shortcut/modifier",Mod1Mask).toUInt();
}


void ShortcutActivator::end() {
    this->terminate = true;
}

QString ShortcutActivator::shortcut_name() {
    QSettings settings;
    QString r;
    switch (this->modifier) {
    case Mod1Mask:
        r = "Alt";
        break;
    case ControlMask:
        r = "Ctrl";
        break;
    default:
        break;
    }
    r += "+" + settings.value("Shortcut/keycode","space").toString();
    return r;
}

void ShortcutActivator::run() {
    XSetErrorHandler(xerrorhandler);
    Display* dpy = XOpenDisplay(getenv("DISPLAY"));
    Window root_window = DefaultRootWindow(dpy);
    XEvent ev;
    unsigned int keycode = XKeysymToKeycode(dpy, this->key);

    {
        /*
         * Caps lock and num lock count as modifiers,
         * so different masks are needed for all their
         * statuses.
         */
        unsigned int mods[] = {
            modifier,
            modifier | Mod2Mask, //Num lock
            modifier | LockMask, // caps lock
            modifier | Mod2Mask | LockMask,
        };

        Bool owner_events = False;
        int pointer_mode = GrabModeAsync;
        int keyboard_mode = GrabModeAsync;

        for (unsigned int i=0; i<(sizeof(mods)/sizeof(int)); i++) {
            XGrabKey(
                dpy,
                keycode,
                mods[i],
                root_window,
                owner_events,
                pointer_mode,
                keyboard_mode
            );
        }
    }

    XSync(dpy, false);

    if (grab_error) {
        grab_error = false;
        emit grab_failed();
    }

    while(true) {
        XNextEvent(dpy, &ev);
        if (ev.type == KeyPress) {
            qDebug() << "Key pressed";
            emit this->activated();
        }

        if(this->terminate)
            break;
    }

    XCloseDisplay(dpy);
}
