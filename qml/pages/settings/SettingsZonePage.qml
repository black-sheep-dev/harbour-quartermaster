import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    property Zone zone

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                enabled: (Client.trackingModes & Client.TrackingWifi) === Client.TrackingWifi
                text: qsTr("Reset")
                onClicked: resetPopup.execute(qsTr("Resetting networks"), function() {
                    zone.networksModel().reset()
                    Client.saveZonesSettings()
                })
            }

            MenuItem {
                enabled: Client.trackingWifi
                text: qsTr("Add Wifi Network")
                onClicked: {
                    Client.updateNetworksModel();
                    Client.networksModel().setSelected(zone.networksModel())
                    pageStack.push(Qt.resolvedUrl("../../dialogs/SelectWifiNetworkDialog.qml"), {zone: zone});
                }
            }
        }

        RemorsePopup { id: resetPopup }

        contentHeight: column.height

        Column {
            id: column

            width: parent.width

            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Zone Settings")
            }
            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                text: zone.name
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
            InfoItem {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                name: qsTr("Latitude")
                value: zone.latitude
            }
            InfoItem {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                name: qsTr("Longitude")
                value: zone.longitude
            }
            InfoItem {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                name: qsTr("Radius")
                value: zone.radius
            }

            SectionHeader {
                text: qsTr("Wifi Networks")
                font.pixelSize: Theme.fontSizeLarge
            }

            SilicaListView {
                id: listView

                width: parent.width
                height: 600

                model: zone.networksModel()

                delegate: ListItem {
                    id: delegate
                    width: parent.width
                    contentHeight: Theme.itemSizeMedium

                    ListView.onRemove: animateRemoval(delegate)

                    menu: ContextMenu {
                        MenuItem {
                            enabled: Client.trackingWifi
                            text: qsTr("Delete");
                            onClicked: remorse.execute(delegate, qsTr("Deleting network"), function() {
                                zone.networksModel().removeNetwork(identifier)
                                Client.saveZonesSettings()
                            })
                        }
                    }

                    RemorseItem { id: remorse }

                    Row {
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2 * x
                        height: parent.height

                        anchors.verticalCenter: parent.verticalCenter

                        spacing: Theme.paddingMedium

                        Image {
                            id: wlanIcon
                            source: "image://theme/icon-m-wlan"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Label {
                            width: parent.width - wlanIcon.width - 2 * Theme.paddingMedium
                            anchors.verticalCenter: parent.verticalCenter

                            text: name.length > 0 ? name : qsTr("Hidden network")
                            color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                    }
                }

                VerticalScrollDecorator {}
            }
        }
    }
}
