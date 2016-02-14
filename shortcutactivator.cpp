#include "shortcutactivator.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

void ShortcutActivator::end() {
    this->terminate = true;
}

void ShortcutActivator::run() {
    Display*    dpy     = XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    XEvent      ev;

    unsigned int    modifiers       = Mod1Mask; // AnyModifier; // ControlMask | ShiftMask | AnyModifier;
    int             keycode         = XKeysymToKeycode(dpy,XK_space);
    Window          grab_window     =  root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    XGrabKey(dpy, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);
    XGrabKey(dpy, keycode, modifiers | Mod2Mask , grab_window, owner_events, pointer_mode, keyboard_mode);
    XGrabKey(dpy, keycode, modifiers | LockMask, grab_window, owner_events, pointer_mode, keyboard_mode);
    XGrabKey(dpy, keycode, modifiers | LockMask | Mod2Mask, grab_window, owner_events, pointer_mode, keyboard_mode);

    XSelectInput(dpy, root, KeyPressMask );
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
    XUngrabKey(dpy,keycode,modifiers,grab_window);
    XUngrabKey(dpy,keycode,modifiers | Mod2Mask,grab_window);
    XUngrabKey(dpy,keycode,modifiers| LockMask,grab_window);
    XUngrabKey(dpy,keycode,modifiers | LockMask | Mod2Mask,grab_window);
}
