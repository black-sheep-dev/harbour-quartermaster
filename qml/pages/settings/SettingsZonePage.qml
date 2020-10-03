import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    property var zone

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Add Wifi Network")
                enabled: Client.trackingWifi
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsWifiPage.qml"));
            }
        }
        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin
            width: page.width - 2 * x
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Zone Settings")
            }
            Label {
                text: zone.name
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
            InfoItem {
                name: qsTr("Latitude")
                value: zone.latitude
            }
            InfoItem {
                name: qsTr("Longitude")
                value: zone.longitude
            }
            InfoItem {
                name: qsTr("Radius")
                value: zone.radius
            }

            SectionHeader {
                text: qsTr("Wifi Networks")
                font.pixelSize: Theme.fontSizeLarge
            }

        }
    }
}
