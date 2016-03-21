Trabucco
========

This is a launcher for people that are nostalgic about katapult.

![Screenshot](http://i.imgur.com/sh166aS.jpg)

It is licensed under GPLv3.

It uses Qt5.5 and QML.

Supported actions
=================

Right now it can search:

 * Installed applications
 * Bookmarks

Build
=====

Requires Qt5.5 and libx11.


```
mkdir build
cd build
qmake ..
make
./trabucco
```

Why
===

I got a bit nostalgic of katapult, because it was much better than krunner.

Trabucco means trebuchet in Italian, which is a siege weapon, like a catapult.

When KDE4 was released, katapult was replaced by krunner, which has several
drawbacks.

 * It sometimes crashes.
 * Uses a timeout to show suggestions.
   This means that if the system is under heavy load, no suggestion is shown.
 * Fuzzy. A certain sequence of letters doesn't always result in the same
   suggestions.
 * Most plugins don't work, or trigger heavy CPU load.

Trabucco aims to solve these problems by showing only one suggestion per
search string, and being deterministic about it.

I will add plugins for things that I need.
