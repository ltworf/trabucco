import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import Qt.labs.settings 1.0

ApplicationWindow {
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
        icon.source = "trabucco.gif"
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
    id: window
    visible: false
    title: "Trabucco!"
    width: Screen.width /6 > 300? Screen.width /6: 300
    height: width + name.height
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
    flags: Qt.Tool | Qt.FramelessWindowHint
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
            source: "trabucco.gif"

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
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            anchors.top: name.bottom
            text: ''
            color: 'white'
            font.pointSize: 35
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
                    icon.source = "trabucco.gif"
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
                    icon.source = "trabucco.gif"
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
