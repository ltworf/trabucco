#include "shortcutactivator.h"

#include <QSettings>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>


ShortcutActivator::ShortcutActivator(QObject *parent) : QThread(parent) {
    QSettings settings;

    const char* k = settings.value("Shortcut/keycode","space").toString().toStdString().c_str();
    this->key = XStringToKeysym(k);
    this->modifier = settings.value("Shortcut/modifier",Mod1Mask).toUInt();
}


void ShortcutActivator::end() {
    this->terminate = true;
}

void ShortcutActivator::run() {
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

    while(true) {
        XNextEvent(dpy, &ev);
        if (ev.type == KeyPress) {
            printf("Key pressed\n");
            emit this->activated();
        }

        if(this->terminate)
            break;
    }

    XCloseDisplay(dpy);
}
