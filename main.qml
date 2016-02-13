import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    flags: Qt.FramelessWindowHint
    color: "transparent"
    visible: true
    Rectangle
    {
        opacity: 0.4
        color:"red"
        width: parent.width/2
        height: parent.height/2;anchors.centerIn: parent
    }
}
