import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                //% "Device sensors"
                title: qsTrId("id-device-sensors")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                //% "Device sensors can send their current state to the Home Assistant server."
                //% "Activating these options leads to higher battery consumption and network traffic."
                text: qsTrId("id-device-sensors-desc");
            }

            TextSwitch {
                id: toggleAllSwitch
                //% "Live updates"
                text: qsTrId("id-live-updates")
                //% "This toggles all sensors on/off"
                description: qsTrId("id-sensor-update-on-off")

                onCheckedChanged: App.deviceService().sensorLiveUpdates = checked
                Component.onCompleted: checked = App.deviceService().sensorLiveUpdates
            }

            SectionHeader {
                //% "Sensor list"
                text: qsTrId("id-sensor-list")
            }

            Repeater {
                model: App.deviceService().sensorsModel()

                BackgroundItem {
                    width: parent.width
                    height: Theme.itemSizeMedium

                    Label {
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2*x
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
