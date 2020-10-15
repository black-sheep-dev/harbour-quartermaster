import QtQuick 2.0
import Sailfish.Silica 1.0

Row {
    property string name
    property string value

    width: parent.width
    height: Theme.fontSizeMedium * 1.2

    Label {
        width: parent.width * 0.5
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.highlightColor

        text: name
        font.pixelSize: Theme.fontSizeMedium
    }

    Label {
        anchors.verticalCenter: parent.verticalCenter
        text: value
        font.pixelSize: Theme.fontSizeSmall
        wrapMode: Label.WordWrap
    }
}
