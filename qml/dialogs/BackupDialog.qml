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
            //% "Create Backup"
            title: qsTrId("id-create-backup")
            //% "Create"
            acceptText: qsTrId("id-create")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            //% "Select options from which a backup should be made. The backup is saved in the Documents folder with the current time stamp."
            text: qsTrId("id-backup-desc")
            wrapMode: Text.WordWrap
            color: Theme.highlightColor
        }

        TextSwitch {
            id: wifiBackupSwitch
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            //% "Access point settings"
            text: qsTrId("id-access-point-settings")
            //% "Backup access point settings of zones"
            description: qsTrId("id-backup-access-points-desc")

            onCheckedChanged: {
                if (checked)
                    options |= BackupTool.BackupAccessPoints
                else
                    options &= ~BackupTool.BackupAccessPoints
            }
            Component.onCompleted: checked = (options & BackupTool.BackupAccessPoints) === BackupTool.BackupAccessPoints
        }
    }
}
