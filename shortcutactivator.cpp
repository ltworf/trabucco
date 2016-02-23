#include "shortcutactivator.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>

void ShortcutActivator::end() {
    this->terminate = true;
}

void ShortcutActivator::run() {
    Display*    dpy     = XOpenDisplay(getenv("DISPLAY"));
    Window      root_window    = DefaultRootWindow(dpy);
    XEvent      ev;

    unsigned int modifier = Mod1Mask; // AnyModifier; // ControlMask | ShiftMask | AnyModifier;
    unsigned int keycode = XKeysymToKeycode(dpy,XK_space);

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

        for (int i=0; i<(sizeof(mods)/sizeof(int)); i++) {
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

    XSelectInput(dpy, root_window, KeyPressMask );
    while(true)
    {
        XNextEvent(dpy, &ev);
        switch(ev.type)
        {
            case KeyPress:
                printf("Key pressed\n");
                emit this->activated();
            default:
                break;
        }

        if(this->terminate)
            break;
    }

    XCloseDisplay(dpy);
}
