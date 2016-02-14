import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    function reset() {
        icon.source = "trabucco.gif"
        search.text = ""
        name.text = "Trabucco!"
    }
    title: "Trabucco!"
    width: Screen.width /6 > 300? Screen.width /6: 300
    height: width + search.height + name.height
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
    flags: Qt.FramelessWindowHint
    color: "transparent"
    visible: true
    Rectangle
    {
        smooth: true
        radius: 15
        opacity: 0.8
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
        }

        Text {
           id: name
           anchors.top: icon.bottom
           width: parent.width
           font.pointSize: 40
           text: "Trabucco!"
           color: "white"
           horizontalAlignment: Text.AlignHCenter

        }

        TextInput {
            id: search
            focus: true
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            anchors.top: name.bottom
            text: ''
            color: 'white'
            font.pointSize: 35
            onTextChanged: {
                if (!search.text.length)
                    reset()

                var a = tree.search(search.text)
                var action_name = a.getName()
                if (action_name)
                    name.text = action_name
                var icon_path = a.getIcon()
                console.log(icon_path)
                if (icon_path) {
                    icon.source = 'file://' + icon_path
                    icon.sourceSize.width = icon.width
                    icon.sourceSize.height = icon.height
                } else
                    icon.source = "trabucco.gif"
            }

            onCursorVisibleChanged: cursorVisible = false
         }



    }
}
