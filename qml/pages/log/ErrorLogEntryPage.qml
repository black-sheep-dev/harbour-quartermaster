import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    property string header
    property string message

    id: page

    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            spacing: Theme.paddingMedium

            PageHeader {
                //% "Log entry"
                title: qsTrId("id-log-entry")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: header
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                font.pixelSize: Theme.fontSizeSmall

                text: message
            }
        }
    }
}
