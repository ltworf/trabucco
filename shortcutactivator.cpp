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

    unsigned int    modifiers       = Mod1Mask; // AnyModifier; // ControlMask | ShiftMask | AnyModifier;
    int             keycode         = XKeysymToKeycode(dpy,XK_space);
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    XGrabKey(dpy, keycode, modifiers, root_window, owner_events, pointer_mode, keyboard_mode);
    XGrabKey(dpy, keycode, modifiers | Mod2Mask , root_window, owner_events, pointer_mode, keyboard_mode);
    XGrabKey(dpy, keycode, modifiers | LockMask, root_window, owner_events, pointer_mode, keyboard_mode);
    XGrabKey(dpy, keycode, modifiers | LockMask | Mod2Mask, root_window, owner_events, pointer_mode, keyboard_mode);

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
