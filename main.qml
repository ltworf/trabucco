import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    title: qsTr("Hello World")
    width: 256
    height: 256 + 40 + 45
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
        opacity: 0.6
        color:"black"
        anchors.fill: parent

        Rectangle {
            id: icon
            opacity: 0
            color: "white"
            width: 256
            height: 256
            anchors.top: parent.top
        }

        Text {
           id: name
           anchors.top: icon.bottom
           width: parent.width
           font.pixelSize: 40
           text: "Trabucco!"
           color: "white"
           horizontalAlignment: Text.AlignHCenter

        }

        TextInput {
            id: depth
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            anchors.top: name.bottom
            text: 'AA'
            color: 'white'
            font.pixelSize: 35
            onTextChanged: {
                var a = tree.search(depth.text)
                console.log(a.getName())
                name.text = a.getName()
            }
         }



    }
}
