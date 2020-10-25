import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            title: qsTr("Restore Backup")

            acceptText: qsTr("Restore")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            wrapMode: Text.WordWrap

            text: qsTr("Do you want to restore the backup? Current settings will be replaced!")
            color: Theme.highlightColor
        }
    }
}
