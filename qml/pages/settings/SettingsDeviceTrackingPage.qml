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
                font.pixelSize: Theme.fontSizeSmall

                text: qsTr("Turn on these options will track your device location and send it to the Home Assistant server.")
                      + "\n"
                      +
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
                enabled: gpsTrackingSwitch.checked
                id: portField
                width: parent.width / 2

                label: qsTr("Update Interval (msec)")

                text: App.locationService().updateInterval

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1000 }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: focus = false
            }

            TextSwitch {
                enabled: gpsTrackingSwitch.checked && wifiTrackingSwitch.checked
                id: disableGpsAtHomeSwitch
                text: qsTr("Disable GPS at home")
                description: qsTr("GPS tracking is turned off when at home. This option needs enabled Wifi tracking.")

                onCheckedChanged: App.locationService().disableGpsAtHome = checked
                Component.onCompleted: checked = App.locationService().disableGpsAtHome
            }

            SectionHeader {
                text: qsTr("Wifi Tracking")
            }

            TextSwitch {
                id: wifiTrackingSwitch
                text: qsTr("Wifi")
                description: qsTr("Available access points will be used to track the device.")
                             + qsTr("If the device discovers a known access point, it will send the position information of its parent zone.")
                             + "\n"
                             + qsTr("It is recommend to keep the wifi tracking enabled!")
                             + "\n"
                             + qsTr("You need to add the access points to your different zones on zone settings page. Zones can be created in the Home Assistant web interface.")



                onCheckedChanged: App.locationService().enableWifi = checked
                Component.onCompleted: checked = App.locationService().enableWifi
            }

//            TextSwitch {
//                enabled: false
//                id: wifiTrackConnectedApsOnlySwitch
//                text: qsTr("Connected only")
//                description: qsTr("Only track access points the device is connected to.")


//                onCheckedChanged: App.locationService().trackConnectedApsOnly = checked
//                Component.onCompleted: checked = App.locationService().trackConnectedApsOnly
//            }
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) App.saveSettings()
}
