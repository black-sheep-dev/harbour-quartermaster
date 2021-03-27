import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
//        PullDownMenu {
//            MenuItem {
//                enabled: App.api().serverConfig().components & ServerConfig.ComponentConfig
//                text: qsTr("Check Config")
//                //onClicked: pageQt.resolvedUrl("SettingsCheckConfigPage.qml")
//            }
//            MenuItem {
//                text: qsTr("Error Log")
//                onClicked: pageStack.push(Qt.resolvedUrl("../log/ErrorLogPage.qml"), { logModel: Client.errorLogModel() });
//            }
//        }

        anchors.fill: parent
        contentHeight: column.height

        RemorsePopup { id: remorse }

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
                value: App.api().serverConfig().locationName
            }

            DetailItem {
                label: qsTr("Version")
                value: App.api().serverConfig().version
            }

            DetailItem {
                label: qsTr("Compatibility")
                value: App.api().serverConfig().versionCompatibility ? qsTr("compatible") : qsTr("incompatible")
            }

            DetailItem {
                label: qsTr("Internal URL")
                value: App.api().serverConfig().internalUrl
            }

            DetailItem {
                label: qsTr("External URL")
                value: App.api().serverConfig().externalUrl
            }

            SectionHeader {
                text: qsTr("Components")
            }

            DetailItem {
                label: "mobile_app"
                value: (App.api().serverConfig().components & ServerConfig.ComponentMobileApp) === ServerConfig.ComponentMobileApp ? qsTr("active") : qsTr("inactive")
            }

            DetailItem {
                label: "webhook"
                value: (App.api().serverConfig().components & ServerConfig.ComponentWebhook) === ServerConfig.ComponentWebhook ? qsTr("active") : qsTr("inactive")
            }

            DetailItem {
                label: "websocket_api"
                value: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) === ServerConfig.ComponentWebsocketApi ? qsTr("active") : qsTr("inactive")
            }

            DetailItem {
                label: "config"
                value: (App.api().serverConfig().components & ServerConfig.ComponentConfig) === ServerConfig.ComponentConfig ? qsTr("active") : qsTr("inactive")
            }
        }
    }
}

