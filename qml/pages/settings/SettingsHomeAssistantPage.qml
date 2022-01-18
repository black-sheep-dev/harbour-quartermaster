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
                //% "Home Assistant"
                title: qsTrId("id-home-assistant")
            }

            DetailItem {
                //% "Location name"
                label: qsTrId("id-location-name")
                value: App.api().serverConfig().locationName
            }

            DetailItem {
                //% "Version"
                label: qsTrId("id-version")
                value: App.api().serverConfig().version
            }

            DetailItem {
                //% "Compatibility"
                label: qsTrId("id-compatibility")
                value: App.api().serverConfig().versionCompatibility ?
                           //% "compatible"
                           qsTrId("id-compatible") :
                           //% "icompatible"
                           qsTrId("id-incompatible")
            }

            DetailItem {
                //% "Internal URL"
                label: qsTrId("id-internal-url")
                value: App.api().serverConfig().internalUrl
            }

            DetailItem {
                //% "External URL"
                label: qsTrId("id-external-url")
                value: App.api().serverConfig().externalUrl
            }

            SectionHeader {
                //% "Components"
                text: qsTrId("id-components")
            }

            DetailItem {
                label: "mobile_app"
                value: (App.api().serverConfig().components & ServerConfig.ComponentMobileApp) === ServerConfig.ComponentMobileApp ?
                           //% "active"
                           qsTrId("id-active") :
                           //% "inactive"
                           qsTrId("id-inactive")
            }

            DetailItem {
                label: "webhook"
                value: (App.api().serverConfig().components & ServerConfig.ComponentWebhook) === ServerConfig.ComponentWebhook ?
                           //% "active"
                           qsTrId("id-active") :
                           //% "inactive"
                           qsTrId("id-inactive")
            }

            DetailItem {
                label: "websocket_api"
                value: (App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi) === ServerConfig.ComponentWebsocketApi ?
                           //% "active"
                           qsTrId("id-active") :
                           //% "inactive"
                           qsTrId("id-inactive")
            }

            DetailItem {
                label: "config"
                value: (App.api().serverConfig().components & ServerConfig.ComponentConfig) === ServerConfig.ComponentConfig ?
                           //% "active"
                           qsTrId("id-active") :
                           //% "inactive"
                           qsTrId("id-inactive")
            }
        }
    }
}

