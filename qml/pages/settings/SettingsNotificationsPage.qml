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
                title: qsTr("Notifications")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can manage notifications from your Home Assistant server.")
                      + "\n"
                      + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }


            TextSwitch {
                text: qsTr("Updates")
                description: qsTr("When active you are notified about Home Assistant updates for your server.")

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
                text: qsTr("Notifications")
                description: qsTr("When active the application will receive notifications from Home Assistant server over a websocket connection.")
                             + "\n"
                             + qsTr("The data of service calls from the notify integration are used. For details, refer to the Home Assistant's documentation!")

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
                width: parent.width
                visible: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) !== ServerConfig.ComponentWebsocketApi

                text: qsTr("Websocket component is not enabled in Home Assistant!")
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
            }
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) App.saveSettings()
}
