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

    DialogHeader {
        id: header
        acceptText: qsTr("Continue")
        cancelText: qsTr("Back")
    }

    Column {
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

            text: qsTr("Home Assistant")

            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        // error
        SectionHeader {
            visible: !connected && !busy
            text: qsTr("Connection Error")
        }

        Label {
            visible: !connected && !busy
            width: parent.width
            text: qsTr("Error connecting to Home Assistant API:")
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
            text: qsTr("Informations")
        }

        Label {
            visible: connected && !busy
            width: parent.width

            text: qsTr("Location name")

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

            text: qsTr("Version")

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

            text: qsTr("Internal URL")

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

            text: qsTr("External URL")

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
            text: qsTr("Configuration checks")
        }

        TestResultItem {
            visible: connected && !busy
            title: qsTr("Version compatibility")
            description: qsTr("Version of Home Assistant is incompatible with this app!")
            result: App.api().serverConfig().versionCompatibility
        }

        TestResultItem {
            visible: connected && !busy
            title: qsTr("Mobile app component")
            description: qsTr("Mobile app component is not activated! Please check your Home Assistant configuration.yaml!")
            result: App.api().serverConfig().components & ServerConfig.ComponentMobileApp
        }

        TestResultItem {
            visible: connected && !busy
            title: qsTr("Webhook component")
            description: qsTr("Webhook component is not activated! Please check your Home Assistant configuration.yaml!")
            result: App.api().serverConfig().components & ServerConfig.ComponentWebhook
        }
    }

    Connections {
        target: App.api()
        onRequestFinished: {
            dialog.busy = false
            dialog.connected = true
            canAccept = App.api().serverConfig().isCompatible()
        }
        onApiError: {
            dialog.error = msg
            dialog.busy = false
        }
    }
}
