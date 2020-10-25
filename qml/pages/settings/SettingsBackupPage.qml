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
                text: qsTr("New Backup")
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
            title: qsTr("Available Backups")
        }

        anchors.fill: parent

        model: backupTool.availableBackups

        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: Theme.itemSizeMedium

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Delete")
                    onClicked: remorse.execute(delegate, qsTr("Deleting backup"), function() {
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

                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: zoneIcon

                    height: parent.height - 2 * Theme.paddingSmall
                    width: parent.height - 2 * Theme.paddingSmall
                    anchors.verticalCenter: parent.verticalCenter

                    source: "image://theme/icon-m-backup"
                }

                Item {
                    id: spacer

                    width:Theme.paddingMedium
                    height:1

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
                        Client.reloadConfig();
                })
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No backups available")
            hintText: qsTr("Pull down to create a new one")
        }
        VerticalScrollDecorator {}
    }
}

