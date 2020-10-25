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

            TextSwitch {
                id: gpsTrackingSwitch
                text: qsTr("GPS Tracking")
                description: qsTr("Use GPS data to track the device position.")
                             + "\n"
                             + qsTr("Position is updated every 30 seconds when activated.")

                onCheckedChanged: {
                    if (checked)
                        Client.trackingModes |= Client.TrackingGPS
                    else
                        Client.trackingModes &= ~Client.TrackingGPS
                }
                Component.onCompleted: checked = (Client.trackingModes & Client.TrackingGPS) === Client.TrackingGPS
            }

            TextSwitch {
                id: wifiTrackingSwitch
                text: qsTr("Wifi Tracking")
                description: qsTr("WiFi networks will be used to track the device. You need to add Wifi Networks to your different zones in zone settings.")
                             + "\n"
                             + qsTr("If the device connects to a known wireless network, it will send the position information of its parent zone.")
                             + "\n"
                             + qsTr("Zones can be created in the Homeassistant web interface.")

                onCheckedChanged: {
                    if (checked)
                        Client.trackingModes |= Client.TrackingWifi
                    else
                        Client.trackingModes &= ~Client.TrackingWifi
                }
                Component.onCompleted: checked = (Client.trackingModes & Client.TrackingWifi) === Client.TrackingWifi
            }
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) Client.saveSettings()
}
