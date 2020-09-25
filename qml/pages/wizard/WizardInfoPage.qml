import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Page {
    id: page

    allowedOrientations: Orientation.Portrait

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: Client.homeassistantInfo().loading
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x

            opacity: 0

            PageHeader {
                Row {
                    x: Theme.paddingMedium
                    width: parent.width - 2*x
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width / 2
                        text: qsTr("Back")

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }

                    Label {
                        id: labelProceed
                        visible: false
                        width: parent.width / 2
                        text: qsTr("Continue")

                        horizontalAlignment: Text.AlignRight

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }

            Label {
                width: parent.width

                text: qsTr("Homeassistant")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            // error
            SectionHeader {
                visible: !Client.homeassistantInfo().available
                text: qsTr("Connection Error")
            }

            Label {
                visible: !Client.homeassistantInfo().available
                width: parent.width
                text: qsTr("Error connecting to Homeassistant API:")
                font.pixelSize: Theme.fontSizeMedium
            }

            Item {
                visible: !Client.homeassistantInfo().available
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: !Client.homeassistantInfo().available
                width: parent.width
                text: Client.homeassistantInfo().error;
                font.pixelSize: Theme.fontSizeMedium
            }

            // success
            SectionHeader {
                visible: Client.homeassistantInfo().available
                text: qsTr("Informations")
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width

                text: qsTr("Location name")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width
                text: Client.homeassistantInfo().locationName

                color: Theme.secondaryColor
            }

            Item {
                visible: Client.homeassistantInfo().available
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width

                text: qsTr("Version")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width
                text: Client.homeassistantInfo().version

                color: Theme.secondaryColor
            }

            Item {
                visible: Client.homeassistantInfo().available
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width

                text: qsTr("External URL")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width
                text: Client.homeassistantInfo().externalUrl

                color: Theme.secondaryColor
            }

            Item {
                visible: Client.homeassistantInfo().available
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width

                text: qsTr("Internal URL")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                visible: Client.homeassistantInfo().available
                width: parent.width
                text: Client.homeassistantInfo().internalUrl

                color: Theme.secondaryColor
            }

            SectionHeader {
                visible: Client.homeassistantInfo().available
                text: qsTr("Configuration checks")
            }

            TestResultItem {
                visible: Client.homeassistantInfo().available
                title: qsTr("Version compatibility")
                description: qsTr("Version of Homeassistant is incompatible with this app!")
                result: Client.homeassistantInfo().versionCompatibility
            }

            TestResultItem {
                visible: Client.homeassistantInfo().available
                title: qsTr("Mobile app component")
                description: qsTr("Mobile app component is not activated! Please check your Homeassistant configuration.yaml!")
                result: Client.homeassistantInfo().components & HomeassistantInfo.ComponentMobileApp
            }

            TestResultItem {
                visible: Client.homeassistantInfo().available
                title: qsTr("Webhook component")
                description: qsTr("Webhook component is not activated! Please check your Homeassistant configuration.yaml!")
                result: Client.homeassistantInfo().components & HomeassistantInfo.ComponentWebhook
            }
        }
    }

    Connections {
        target: Client.homeassistantInfo()
        onLoadingChanged: {
            if (Client.homeassistantInfo().isInstanceValid()) {
                labelProceed.visible = true
                pageStack.pushAttached(Qt.resolvedUrl("WizardDeviceRegistrationPage.qml"))
            }

            if (!Client.homeassistantInfo().loading) column.opacity = 1
        }
    }
}
