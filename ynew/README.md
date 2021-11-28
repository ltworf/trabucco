ynew
====

This is a rather simple script to open a new tab in yakuake, set the name of
the tab, run a script, and close the tab when the script terminates.

For example, if you want to ssh to a server called `pippo`, you can run
`ynew pippo`.

An editor will open, and you can type your command, for example `ssh pippo`.

After saving the script, running `ynew pippo` again, will open a new tab named
*pippo* which will contain the ssh session.

Script for bash completion is also provided.


Install
=======

`make install`


Debian package
==============

You can create a Debian package running `dpkg-buildpackage`.
