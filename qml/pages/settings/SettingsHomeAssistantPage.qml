import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Error Log")
                onClicked: pageStack.push(Qt.resolvedUrl("../log/ErrorLogPage.qml"), { logModel: Client.errorLogModel() });
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin
            width: page.width - 2 * x
            spacing: Theme.paddingSmall

            PageHeader {
                title: qsTr("Home Assistant")
            }

            DetailItem {
                label: qsTr("Location Name")
                value: Client.homeassistantInfo().locationName
            }

            DetailItem {
                label: qsTr("Version")
                value: Client.homeassistantInfo().version
            }

            DetailItem {
                label: qsTr("Compatibility")
                value: Client.homeassistantInfo().versionCompatibility ? qsTr("compatible") : qsTr("incompatible")
            }

            DetailItem {
                label: qsTr("Internal URL")
                value: Client.homeassistantInfo().internalUrl
            }

            DetailItem {
                label: qsTr("External URL")
                value: Client.homeassistantInfo().externalUrl
            }

            SectionHeader {
                text: qsTr("Components")
            }

            DetailItem {
                label: "mobile_app"
                value: (Client.homeassistantInfo().components & HomeassistantInfo.ComponentMobileApp) === HomeassistantInfo.ComponentMobileApp ? qsTr("active") : qsTr("inactive")
            }

            DetailItem {
                label: "webhook"
                value: (Client.homeassistantInfo().components & HomeassistantInfo.ComponentWebhook) === HomeassistantInfo.ComponentWebhook ? qsTr("active") : qsTr("inactive")
            }

            DetailItem {
                label: "websocket_api"
                value: (Client.homeassistantInfo().components & HomeassistantInfo.ComponentWebsocketApi) === HomeassistantInfo.ComponentWebsocketApi ? qsTr("active") : qsTr("inactive")
            }
        }
    }
}

