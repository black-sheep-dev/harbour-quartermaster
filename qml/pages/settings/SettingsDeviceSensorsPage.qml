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
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Device Sensors")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can activate / deactivate automatic sensors updates. Automatic sensor updates can lead to more network traffic and battery consumption.");
            }

            TextSwitch {
                id: toggleAllSwitch
                text: qsTr("Activate / deactivate automatic updates")
                description: qsTr("This toggles all sensors on/off")

                onCheckedChanged: Client.device().sensorAutoUpdate = checked
                Component.onCompleted: checked = Client.device().sensorAutoUpdate
            }

            SectionHeader {
                text: qsTr("Sensor List")
            }

            ListView {
                width: parent.width
                height: 600
                contentHeight: Theme.itemSizeMedium

                model: Client.device().sensorModel()

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
