import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Dialog {
    property bool busy: true
    property bool connected: false
    property string error

    id: dialog

    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardDeviceRegistrationPage.qml")

    canAccept: false

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: busy
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: header.height + column.height

        DialogHeader {
            id: header
            //% "Continue"
            acceptText: qsTrId("id-continue")
            //% "Back"
            cancelText: qsTrId("id-back")
        }

        Column {
            id: column
            anchors.top: header.bottom
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            spacing: Theme.paddingSmall

            opacity: busy ? 0.2 : 1

            Behavior on opacity {
                FadeAnimation {}
            }

            Label {
                visible: !busy
                width: parent.width

                //% "Home Assistant"
                text: qsTrId("id-home-assistant")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            // error
            SectionHeader {
                visible: !connected && !busy
                //% "Connection error"
                text: qsTrId("id-connection-error")
            }

            Label {
                visible: !connected && !busy
                width: parent.width
                //% "Error connecting to Home Assistant API:"
                text: qsTrId("id-connection-error-desc")
                font.pixelSize: Theme.fontSizeMedium
            }

            Item {
                visible: !connected && !busy
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: !connected && !busy
                width: parent.width
                text: error;
                font.pixelSize: Theme.fontSizeMedium
            }

            // success
            SectionHeader {
                visible: connected && !busy
                //% "Information"
                text: qsTrId("id-information")
            }

            Label {
                visible: connected && !busy
                width: parent.width

                //% "Location name"
                text: qsTrId("id-location-name")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: connected && !busy
                width: parent.width
                text: App.api().serverConfig().locationName

                color: Theme.secondaryColor
            }

            Item {
                visible: connected && !busy
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: connected && !busy
                width: parent.width

                //% "Version"
                text: qsTrId("id-version")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: connected && !busy
                width: parent.width
                text: App.api().serverConfig().version

                color: Theme.secondaryColor
            }

            Item {
                visible: connected && !busy
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: connected && !busy
                width: parent.width

                //% "Internal URL"
                text: qsTrId("id-internal-url")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: connected && !busy
                width: parent.width
                text: App.api().serverConfig().internalUrl

                color: Theme.secondaryColor
            }

            Item {
                visible: connected && !busy
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: connected && !busy
                width: parent.width

                //% "External URL"
                text: qsTrId("id-external-url")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: connected && !busy
                width: parent.width
                text: App.api().serverConfig().externalUrl

                color: Theme.secondaryColor
            }

            SectionHeader {
                visible: connected && !busy
                //% "Configuration checks"
                text: qsTrId("id-configuration-checks")
            }

            TestResultItem {
                visible: connected && !busy
                //% "Version compatibility"
                title: qsTrId("id-version-compatibility")
                //% "Version of Home Assistant is incompatible with this app!"
                description: qsTrId("id-version-compatibility-desc")
                result: App.api().serverConfig().versionCompatibility
            }

            TestResultItem {
                visible: connected && !busy
                //% "Mobile app component"
                title: qsTrId("id-mobile-app-component")
                //% "Mobile app component is not activated!"
                description: qsTrId("id-mobile-app-component-not-activated") + " " +
                                //% "Please check your Home Assistant configuration.yaml!"
                                qsTrId("id-check-yaml-config")
                result: App.api().serverConfig().components & ServerConfig.ComponentMobileApp
            }

            TestResultItem {
                visible: connected && !busy
                //% "Webhook component"
                title: qsTrId("id-webhook-component")
                //% "Webhook component is not activated!"
                description: qsTrId("id-webhook-component-not-activated") + " " +
                             //% "Please check your Home Assistant configuration.yaml!"
                             qsTrId("id-check-yaml-config")
                result: App.api().serverConfig().components & ServerConfig.ComponentWebhook
            }

            TestResultItem {
                visible: connected && !busy
                //% "Websocket component"
                title: qsTrId("id-websocket-component")
                //% "Optional Websocket component is not activated!"
                description: qsTrId("id-websocket-component-not-activated") + " " +
                             //% "Please check your Home Assistant configuration.yaml!"
                             qsTrId("id-check-yaml-config")
                result: App.api().serverConfig().components & ServerConfig.ComponentWebsocketApi
                //% "MISSING"
                resultTextFalse: qsTrId("id-missing")
            }
        }

        VerticalScrollDecorator {}
    }

    Connections {
        target: App.api()
        onRequestFinished: {
            if (requestType !== Api.RequestGetApiConfig) return;

            dialog.busy = false
            dialog.connected = true
            canAccept = App.api().serverConfig().isCompatible()
        }
        onRequestError: {
            if (requestType !== Api.RequestGetApiConfig) return;

            dialog.error = msg
            dialog.busy = false
        }
    }
}
