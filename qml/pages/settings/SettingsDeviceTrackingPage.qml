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
            width: parent.width - 2 * x

            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Device Tracking")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can activate / deactivate different device tracking options.")
                      + "\n"
                      + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }

            SectionHeader {
                text: qsTr("GPS Tracking")
            }

            TextSwitch {
                id: gpsTrackingSwitch
                text: qsTr("GPS")
                description: qsTr("Use GPS data to track the device position.")

                onCheckedChanged: App.locationService().enableGps = checked
                Component.onCompleted: checked = App.locationService().enableGps
            }

            TextField {
                id: portField
                width: parent.width / 2

                label: qsTr("Update Interval (msec)")

                text: App.locationService().updateInterval

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1000 }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: focus = false
            }

            SectionHeader {
                text: qsTr("Wifi Tracking")
            }

            TextSwitch {
                id: wifiTrackingSwitch
                text: qsTr("Wifi")
                description: qsTr("WiFi networks will be used to track the device. You need to add Wifi Networks to your different zones in zone settings.")
                             + "\n"
                             + qsTr("If the device connects to a known wireless network, it will send the position information of its parent zone.")
                             + "\n"
                             + qsTr("Zones can be created in the Home Assistant web interface.")
                             + "\n"
                             + qsTr("It is recommend to keep this option enabled!")

                onCheckedChanged: App.locationService().enableWifi = checked
                Component.onCompleted: checked = App.locationService().enableWifi
            }
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) Client.saveSettings()
}
