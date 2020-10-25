import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    property var options: BackupTool.BackupWifi

    id: dialog

    anchors.fill: parent

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            title: qsTr("Create Backup")

            acceptText: qsTr("Create")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            text: qsTr("Select options from which a backup should be made. The backup is saved in the Documents folder with the current time stamp.")
            wrapMode: Text.WordWrap
            color: Theme.highlightColor
        }

        TextSwitch {
            id: wifiBackupSwitch
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            text: qsTr("Wifi settings")
            description: qsTr("Backup wifi network settings of zones")

            onCheckedChanged: {
                if (checked)
                    options |= BackupTool.BackupWifi
                else
                    options &= ~BackupTool.BackupWifi
            }
            Component.onCompleted: checked = (options & BackupTool.BackupWifi) === BackupTool.BackupWifi
        }
    }
}
