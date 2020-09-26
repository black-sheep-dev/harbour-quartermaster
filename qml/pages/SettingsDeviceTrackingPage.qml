import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../components/"

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
                title: qsTr("Device Tracking")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can activate / deactivate different device tracking options.") + "\n" + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }

            TextSwitch {
                id: gpsTrackingSwitch
                text: qsTr("GPS Tracking")
                description: qsTr("Use GPS data to track the device position.")

                onCheckedChanged: Client.device().trackingGPS = checked
                Component.onCompleted: checked = Client.device().trackingGPS
            }

            TextSwitch {
                id: wifiTrackingSwitch
                text: qsTr("WiFi SSID Tracking")
                description: qsTr("WiFi networks will be used to track the device. You need to add WiFi SSID informations to your different zones. If the device connects to a known WiFi network, it will use the position info of the zone.")

                onCheckedChanged: Client.device().trackingWifi = checked
                Component.onCompleted: checked = Client.device().trackingWifi
            }

        }
    }
}
