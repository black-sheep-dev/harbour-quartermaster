import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    property string title
    property string description
    property bool result: false
    //% "PASSED"
    property string resultTextTrue: qsTrId("id-passed")
    //% "FAILED"
    property string resultTextFalse: qsTrId("id-failed")

    width: parent.width
    spacing: Theme.paddingMedium

    Row {
        width: parent.width

        Label {
            width: parent.width * 0.7
            anchors.verticalCenter: parent.verticalCenter

            text: title

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeMedium
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: result ? resultTextTrue : resultTextFalse
            wrapMode: Text.WordWrap
            width: parent.width * 0.3

            horizontalAlignment: Text.AlignRight

            color: result ? "#76ff03" : "#d50000"
            font.pixelSize: Theme.fontSizeMedium
        }
    }

    Label {
        visible: !result && description.length > 0
        width: parent.width
        wrapMode: Text.Wrap

        text: description
        font.pixelSize: Theme.fontSizeSmall
    }
}
