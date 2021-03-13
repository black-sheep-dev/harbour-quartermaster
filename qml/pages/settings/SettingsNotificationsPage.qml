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
                font.pixelSize: Theme.fontSizeSmall

                text: qsTr("Here you can activate / deactivate notifications from your Home Assistant server.")
                      + "\n"
                      + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }


            TextSwitch {
                id: websocketNotifySwitch
                enabled: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) === ServerConfig.ComponentWebsocketApi
                text: qsTr("Notifications")
                description: qsTr("When active the application will receive notifications from Home Assistant server over websocket connection.")
                             + "\n"
                             + qsTr("The data of service calls from the notify integration are used. For details, refer to the Home Assistant's documentation!")

                onCheckedChanged: {
                    var subscriptions = App.api().subscriptions;
                    if (checked)
                        subscriptions |= Api.SubscriptionNotifyEvents
                    else
                        subscriptions &= ~Api.SubscriptionNotifyEvents

                    App.api().subscriptions = subscriptions
                }
                Component.onCompleted: checked = (App.api().subscriptions & Api.SubscriptionNotifyEvents)
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
