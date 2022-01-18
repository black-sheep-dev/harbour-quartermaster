import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                //% "Reset registration"
                text: qsTrId("id-reset-registration")
                //% "Resetting device registration & closing app"
                onClicked: resetPopup.execute(qsTrId("id-reset-registration-text"), function() {
                    App.reset()
                    Qt.quit()
                })
            }
        }

        RemorsePopup { id: resetPopup }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Device info"
                title: qsTrId("id-device-info")
            }

            TextField {
                id: nameField
                width: parent.width

                //% "Device name"
                label: qsTrId("id-device-name")
                //% "Enter device name"
                placeholderText: qsTrId("id-enter-device-name")

                text: App.deviceService().deviceName

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: nameField.focus = false
            }

            SectionHeader {
                //% "Information"
                text: qsTrId("id-information")
            }

            DetailItem {
                //% "Device ID"
                label: qsTrId("id-device-id")
                value: App.deviceService().id()
            }

            DetailItem {
                //% "Manufacturer"
                label: qsTrId("id-manufacturer")
                value: App.deviceService().manufacturer()
            }

            DetailItem {
                //% "Model"
                label: qsTrId("id-model")
                value: App.deviceService().model()
            }

            DetailItem {
                //% "Software name"
                label: qsTrId("id-software-name")
                value: App.deviceService().softwareName()
            }

            DetailItem {
                //% "Software version"
                label: qsTrId("id-software-version")
                value: App.deviceService().softwareVersion()
            }

            DetailItem {
                //% "Wifi MAC"
                label: qsTrId("id-wifi-mac")
                value: App.deviceService().wlanMacAddress()
            }
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        App.deviceService().deviceName = nameField.text
        App.saveSettings()
        App.deviceService().updateDeviceRegistration();
    }
}
