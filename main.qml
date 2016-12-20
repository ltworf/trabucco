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

import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import Qt.labs.settings 1.0

ApplicationWindow {
    MouseArea {
        acceptedButtons: Qt.MiddleButton
        anchors.fill: parent
        onClicked: {
            search.insert(search.length, clipboard.getSelection())
        }
    }

    function appear() {
        fadeIn.start()
        window.show()
        window.raise()
        window.requestActivate()
    }
    function disappear() {
        fadeOut.start();
    }

    function reset() {
        icon.source = settings.defaultimg
        search.text = ""
        name.text = "Trabucco!"
        cornerIcon.visible = false
    }

    function startsWith(long_string, header){
        return long_string.substr(0, header.length) == header
    }

    Settings {
        id: settings;
        property int duration: 250;
        property double opacity: 0.8;
        property string defaultimg: trabucco_icon;
    }

    Connections {
        target: ShortcutX11
        onActivated: {
            if (window.visible)
                disappear();
            else
                appear();
        }
    }

    onActiveChanged: {
        if (!active)
            disappear();
    }

    id: window
    visible: false
    title: "Trabucco!"
    width: Screen.width /6 > 300? Screen.width /6: 300
    height: width + name.height
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
    flags: Qt.WindowStaysOnTopHint | Qt.X11BypassWindowManagerHint
    color: "transparent"

    Rectangle
    {
        NumberAnimation on opacity {
            id: fadeIn
            from: 0
            to: settings.opacity;
            duration: settings.duration;
        }
        NumberAnimation on opacity {
            id: fadeOut
            from: settings.opacity;
            to: 0
            duration: settings.duration;
            onStopped: {
                window.setVisible(false);
                reset();
            }
        }

        smooth: true
        radius: width / 15
        color:"black"
        anchors.fill: parent

        Image {
            id: icon
            opacity: 1
            source: settings.defaultimg

            //Make the icon area slightly smaller than the window
            width: parent.width * 0.95
            height: parent.width * 0.95
            x: (parent.width / 2 - width / 2)
            y: parent.width - width

            Image {
                id: cornerIcon
                visible: false
                opacity: 1
                width: parent.width / 4
                height: parent.height / 4
                anchors.bottom: parent.bottom
                anchors.right: parent.right
            }
        }

        Text {
            id: name
            anchors.top: icon.bottom
            width: parent.width
            font.pointSize: 40
            fontSizeMode: Text.HorizontalFit
            text: "Trabucco!"
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }

        TextInput {
            id: search
            focus: true
            visible: false
            text: ''
            Shortcut {
                sequence: "Esc"
                onActivated: {
                    disappear();
                }
            }

            onAccepted: {
                tree.runAction()
                disappear();
            }

            onTextChanged: {
                if (!search.text.length) {
                    reset()
                    return
                }

                var a = tree.search(search.text)
                var action_name = ''
                var is_prefix = false
                if (a !== null) {
                    action_name = a.getName()
                    is_prefix = a.isPrefix()
                }


                if (startsWith(action_name.toLowerCase(),text.toLowerCase())) {
                    name.text = '<font color="blue">' + action_name.substr(0,search.text.length) + '</font>'
                    name.text += action_name.substr(text.length, action_name.length)
                } else if (is_prefix) {
                    name.text = '<font color="blue">' + search.text + '</font>'
                } else {
                    name.text = '<font color="blue">' + search.text + '</font>'
                    icon.source = settings.defaultimg
                    cornerIcon.visible = false
                    return
                }

                var icon_path = a.getIcon()
                if (icon_path) {
                    icon.source = 'file://' + icon_path
                    //Some magic to make svg images show with the proper resolution
                    icon.sourceSize.width = icon.width
                    icon.sourceSize.height = icon.height
                } else
                    icon.source = settings.defaultimg
                if (a.hasCornerIcon() && a.getCornerIcon()) {
                    cornerIcon.visible = true
                    cornerIcon.source = 'file://' + a.getCornerIcon()
                    cornerIcon.sourceSize.width = cornerIcon.width
                    cornerIcon.sourceSize.height = cornerIcon.height
                } else {
                    cornerIcon.visible = false
                }
            }

            onCursorVisibleChanged: cursorVisible = false
         }

    }
}
