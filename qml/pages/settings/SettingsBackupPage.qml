import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    BackupTool {
        id: backupTool
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                //% "New backup"
                text: qsTrId("id-new-backup")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../../dialogs/BackupDialog.qml"))

                    dialog.accepted.connect(function() {
                        backupTool.backup(dialog.options)
                    })
                }
            }
        }

        id: listView

        header: PageHeader {
            //% "Available backups"
            title: qsTrId("id-available-backups")
        }

        anchors.fill: parent

        model: backupTool.availableBackups

        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: Theme.itemSizeMedium

            menu: ContextMenu {
                MenuItem {
                    //% "Delete"
                    text: qsTrId("id-delete")
                    //% "Deleting backup"
                    onClicked: remorse.execute(delegate, qsTrId("id-deleting-backup"), function() {
                        backupTool.deleteBackup(backupTool.availableBackups[index])
                    })
                }
            }

            ListView.onRemove: animateRemoval(delegate)

            RemorseItem { id: remorse }

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                spacing: Theme.paddingMedium

                anchors.verticalCenter: parent.verticalCenter

                Image {
                    height: parent.height - 2 * Theme.paddingSmall
                    width: parent.height - 2 * Theme.paddingSmall
                    anchors.verticalCenter: parent.verticalCenter

                    source: "image://theme/icon-m-backup"
                }

                Label{
                    id: text
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    text: backupTool.availableBackups[index]
                    color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                    font.pixelSize: Theme.fontSizeMedium
                }

            }
            onClicked: {
                var dialog = pageStack.push(Qt.resolvedUrl("../../dialogs/RestoreBackupDialog.qml"))

                dialog.accepted.connect(function() {
                    if (backupTool.restoreBackup(backupTool.availableBackups[index]))
                        App.locationTracker.reloadConfig();
                })
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            //% "No backups available"
            text: qsTrId("id-no-backups-available")
            //% "Pull down to create a new one"
            hintText: qsTrId("id-no-backups-available-desc")
        }
        VerticalScrollDecorator {}
    }
}

