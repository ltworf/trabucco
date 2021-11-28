ynew
====

This is a rather simple script to open a new tab in yakuake, set the name of
the tab, run a script, and close the tab when the script terminates.

For example, if you want to ssh to a server called `alfio`, you can run
`ynew alfio`.

An editor will open, and you can type your command, for example `ssh alfio`.

After saving the script, running `ynew alfio` again, will open a new tab named
*alfio* which will contain the ssh session.

Script for bash completion is also provided.
