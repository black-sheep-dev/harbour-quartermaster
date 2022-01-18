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
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Device tracking"
                title: qsTrId("id-device-tracking")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                //% "Manage options to track your device location and send it to the Home Assistant server."
                //% "Activating these options leads to higher battery consumption and network traffic."
                text: qsTrId("id-device-tracking-desc");
            }

            SectionHeader {
                //% "GPS tracking"
                text: qsTrId("id-gps-tracking")
            }

            TextSwitch {
                id: gpsTrackingSwitch
                //% "GPS"
                text: qsTrId("id-gps")
                //% "Use GPS data to track the device position."
                description: qsTrId("id-gps-tracking-desc")

                onCheckedChanged: App.locationService().enableGps = checked
                Component.onCompleted: checked = App.locationService().enableGps
            }

            TextField {
                enabled: gpsTrackingSwitch.checked
                id: portField
                width: parent.width / 2

                //% "Update Interval (msec)"
                label: qsTrId("id-update-interval-msec")

                text: App.locationService().updateInterval

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1000 }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: focus = false
            }

            TextSwitch {
                enabled: gpsTrackingSwitch.checked && wifiTrackingSwitch.checked
                id: disableGpsAtHomeSwitch
                //% "Disable GPS at home"
                text: qsTrId("id-disable-gps-at-home")
                //% "GPS tracking is turned off when at home. This option needs enabled Wifi tracking."
                description: qsTrId("id-disable-gps-at-home-desc")

                onCheckedChanged: App.locationService().disableGpsAtHome = checked
                Component.onCompleted: checked = App.locationService().disableGpsAtHome
            }

            SectionHeader {
                //% "Wifi tracking"
                text: qsTrId("id-wifi-tracking")
            }

            TextSwitch {
                id: wifiTrackingSwitch
                //% "Wifi"
                text: qsTrId("id-wifi")
                //% "Available access points will be used to track the device."
                //% "If the device discovers a known access point, it will send the position information of its parent zone."
                //% "It is recommend to keep the wifi tracking enabled!"
                //% "You need to add the access points to your different zones on zone settings page. Zones can be created in the Home Assistant web interface."
                description: qsTrId("id-wifi-tracking-desc")



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
