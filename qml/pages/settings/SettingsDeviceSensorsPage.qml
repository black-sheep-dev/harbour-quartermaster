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

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Device Sensors")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Device sensors can send their current state to the Home Assistant server. This can lead to high network traffic and battery consumption.");
            }

            TextSwitch {
                id: toggleAllSwitch
                text: qsTr("Live Updates")
                description: qsTr("This toggles all sensors on/off")

                onCheckedChanged: App.deviceService().sensorLiveUpdates = checked
                Component.onCompleted: checked = App.deviceService().sensorLiveUpdates
            }

            SectionHeader {
                text: qsTr("Sensor List")
            }

            ListView {
                width: parent.width
                height: 600
                contentHeight: Theme.itemSizeMedium

                model: App.deviceService().sensorsModel()

                delegate: Item {
                    width: parent.width
                    height: Theme.itemSizeMedium

                    Label {
                        width: parent.width
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
