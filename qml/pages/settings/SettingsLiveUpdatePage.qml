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
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Live updates"
                title: qsTrId("id-live-updates")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                //% "Here you can manage live updates for your entities state."
                //% "Activating these options leads to higher battery consumption and network traffic."
                text: qsTrId("id-live-updates-desc")
            }

            TextSwitch {
                id: websocketStatesSwitch
                enabled: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) === ServerConfig.ComponentWebsocketApi
                //% "Live updates"
                text: qsTrId("id-live-updates")
                //% "When active the entities will be updated live using websocket connection."
                //% "This can lead to very high data consumption, especially with many entities that often change their state!"
                description: qsTrId("id-switch-live-updates-desc")

                onCheckedChanged: App.entitiesService().liveUpdates = checked
                Component.onCompleted: checked = App.entitiesService().liveUpdates
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x

                visible: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) !== ServerConfig.ComponentWebsocketApi

                //% "Websocket component is not enabled in Home Assistant!"
                text: qsTrId("id-websocket-component-not-enabled")
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
            }

//            SectionHeader {
//                text: qsTr("REST API")
//            }

//            TextSwitch {
//                id: entityUpdateSwitch
//                text: qsTr("Single entity")
//                description: qsTr("When active the entity will be updated when his property page is shown.")

//                onCheckedChanged: {
//                    if (checked)
//                        Client.updateModes |= Client.UpdateModeSingleEntity
//                    else
//                        Client.updateModes &= ~Client.UpdateModeSingleEntity
//                }
//                Component.onCompleted: checked = (Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity
//            }

//            TextSwitch {
//                id: modelUpdateSwitch
//                text: qsTr("Entity list")
//                description: qsTr("When active all entities of one type will be updated when the corresponding list view is shown.")

//                onCheckedChanged: {
//                    if (checked)
//                        Client.updateModes |= Client.UpdateModeEntityModel
//                    else
//                        Client.updateModes &= ~Client.UpdateModeEntityModel
//                }
//                Component.onCompleted: checked = (Client.updateModes & Client.UpdateModeEntityModel) === Client.UpdateModeEntityModel
//            }

        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) App.saveSettings()
}
