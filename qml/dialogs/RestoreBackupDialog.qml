import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            //% "Restore Backup"
            title: qsTrId("id-restore-backup")
            //% "Restore"
            acceptText: qsTrId("id-restore")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            wrapMode: Text.WordWrap

            //% "Do you want to restore the backup? Current settings will be replaced!"
            text: qsTrId("id-restore-backup-desc")
            color: Theme.highlightColor
        }
    }
}
