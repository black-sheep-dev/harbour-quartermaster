import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x

            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Automatic Updates")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can activate / deactivate automatic updates for your entities.")
                      + "\n"
                      + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }

            SectionHeader {
                text: qsTr("Websocket")
            }

            TextSwitch {
                id: websocketStatesSwitch
                text: qsTr("Live updates")
                description: qsTr("When active the entities will be updated live using websocket connection.")
                             + "\n"
                             + qsTr("This can lead to very high data consumption, especially with many entities that often change their state!")

                onCheckedChanged: {
                    if (checked)
                        Client.updateModes |= Client.UpdateModeWebsocket
                    else
                        Client.updateModes &= ~Client.UpdateModeWebsocket
                }
                Component.onCompleted: checked = (Client.updateModes & Client.UpdateModeWebsocket) === Client.UpdateModeWebsocket
            }

            SectionHeader {
                text: qsTr("REST API")
            }

            TextSwitch {
                id: entityUpdateSwitch
                text: qsTr("Single entity")
                description: qsTr("When active the entity will be updated when his property page is shown.")

                onCheckedChanged: {
                    if (checked)
                        Client.updateModes |= Client.UpdateModeSingleEntity
                    else
                        Client.updateModes &= ~Client.UpdateModeSingleEntity
                }
                Component.onCompleted: checked = (Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity
            }

            TextSwitch {
                id: modelUpdateSwitch
                text: qsTr("Entity list")
                description: qsTr("When active all entities of one type will be updated when the corresponding list view is shown.")

                onCheckedChanged: {
                    if (checked)
                        Client.updateModes |= Client.UpdateModeEntityModel
                    else
                        Client.updateModes &= ~Client.UpdateModeEntityModel
                }
                Component.onCompleted: checked = (Client.updateModes & Client.UpdateModeEntityModel) === Client.UpdateModeEntityModel
            }

        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) Client.saveSettings()
}
