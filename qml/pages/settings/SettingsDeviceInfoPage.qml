import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Reset Registration")
                onClicked: resetPopup.execute(qsTr("Resetting device registration & closing app"), function() {
                    Client.reset()
                    Qt.quit()
                })
            }
        }

        RemorsePopup { id: resetPopup }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Device Info")
            }

            TextField {
                id: nameField
                width: parent.width

                label: qsTr("Device Name")
                placeholderText: qsTr("Enter device name")

                text: Client.device().name

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: nameField.focus = false
            }

            SectionHeader {
                text: qsTr("Informations")
            }

            DetailItem {
                label: qsTr("Device ID")
                value: Client.device().id()
            }

            DetailItem {
                label: qsTr("Manufacturer")
                value: Client.device().manufacturer()
            }

            DetailItem {
                label: qsTr("Model")
                value: Client.device().model()
            }

            DetailItem {
                label: qsTr("Software Name")
                value: Client.device().softwareName()
            }

            DetailItem {
                label: qsTr("Software Version")
                value: Client.device().softwareVersion()
            }

            DetailItem {
                label: qsTr("Wifi MAC")
                value: Client.device().wlanMacAddress()
            }
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        Client.device().name = nameField.text
        Client.saveSettings()
        Client.updateRegistration();
    }
}
