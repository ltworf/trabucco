Trabucco
========

Can launch your 90kg applications for 300m (It's a launcher, like katapult, but better).

This is a launcher for people that are nostalgic about katapult.

![Screenshot](http://i.imgur.com/sh166aS.jpg)

It is licensed under GPLv3.

It uses Qt6.2 and QML.

Supported actions
=================

Right now it can search:

 * Installed applications
 * Bookmarks
 * Web shortcuts (Those KDE things to search on google by typing gg:string)

Build
=====

Requires Qt6.4 and libx11.


```
mkdir build
cd build
qmake6 ..
make -j
./trabucco
```

To compile with debug symbols, add `CONFIG+=debug` after `qmake6`.

Why
===

I got a bit nostalgic of katapult, because it was much better than krunner.

Trabucco means trebuchet in Italian, which is a siege weapon, like a catapult.

When KDE4 was released, katapult was replaced by krunner, which has several
drawbacks.

 * Fuzzy. A certain sequence of letters doesn't always result in the same
   suggestions.
 * It sometimes crashes.
 * Uses a timeout to show suggestions.
   This means that if the system is under heavy load, no suggestion is shown.
 * Most plugins don't work, or trigger heavy CPU load.

Trabucco aims to solve these problems by showing only one suggestion per
search string, and being deterministic about it.

I will add plugins for things that I need.

Contributing
============

Feel free to send patches to tiposchi@tiscali.it or to open github pull requests.

[Donate to LtWorf](https://liberapay.com/ltworf/donate)
