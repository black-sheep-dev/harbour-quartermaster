import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingLarge

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

            InfoItem {
                name: qsTr("Device ID")
                value: Client.device().id()
            }

            InfoItem {
                name: qsTr("Manufacturer")
                value: Client.device().manufacturer()
            }

            InfoItem {
                name: qsTr("Model")
                value: Client.device().model()
            }

            InfoItem {
                name: qsTr("Software Name")
                value: Client.device().softwareName()
            }

            InfoItem {
                name: qsTr("Software Version")
                value: Client.device().softwareVersion()
            }

            InfoItem {
                name: qsTr("Wifi MAC")
                value: Client.device().wlanMacAddress()
            }
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        Client.device().name = nameField.text

        Client.saveSettings()
    }
}
