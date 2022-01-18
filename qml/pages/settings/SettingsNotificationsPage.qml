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
                //% "Notifications"
                title: qsTrId("id-notifications")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                //% "Here you can manage notifications from your Home Assistant server."
                //% "<br><br>"
                //% "Activating these options leads to higher battery consumption and network traffic."
                text: qsTrId("id-notifcations-desc");
            }


            TextSwitch {
                //% "Updates"
                text: qsTrId("id-updates")
                //% "When active you are notified about Home Assistant updates for your server."
                description: qsTrId("id-switch-updates-desc")

                onCheckedChanged: {
                    var options = App.notificationService().options;
                    if (checked)
                        options |= NotificationService.ServerUpdates
                    else
                        options &= ~NotificationService.ServerUpdates

                    App.notificationService().options = options
                }
                Component.onCompleted: checked = (App.notificationService().options & NotificationService.ServerUpdates)
            }

            TextSwitch {
                enabled: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) === ServerConfig.ComponentWebsocketApi

                //% "Notifications"
                text: qsTrId("id-notifications")
                //% "When active the application will receive notifications from Home Assistant server over a websocket connection."
                //% "The data of service calls from the notify integration are used. For details, refer to the Home Assistant's documentation!"
                description: qsTrId("id-switch-notifications-desc")

                onCheckedChanged: {
                    var options = App.notificationService().options;
                    if (checked)
                        options |= NotificationService.WebsocketEvents
                    else
                        options &= ~NotificationService.WebsocketEvents

                    App.notificationService().options = options
                }
                Component.onCompleted: checked = (App.notificationService().options & NotificationService.WebsocketEvents)
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
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) App.saveSettings()
}
