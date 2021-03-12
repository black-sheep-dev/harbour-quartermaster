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

                    var dialog = pageStack.push(Qt.resolvedUrl("../../dialogs/SelectWifiNetworkDialog.qml"), {zone: zone});

                    dialog.accepted.connect(function() {
                        Client.networksModel().addSelectedToModel(zone.networksModel())
                        Client.saveZonesSettings();
                    })
                }
            }
        }

        RemorsePopup { id: resetPopup }

        contentHeight: column.height

        Column {
            id: column

            width: parent.width

            spacing: Theme.paddingMedium

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

            DetailItem {
                label: qsTr("Latitude")
                value: zone.latitude
            }

            DetailItem {
                label: qsTr("Longitude")
                value: zone.longitude
            }

            DetailItem {
                label: qsTr("Radius")
                value: zone.radius
            }

            SectionHeader {
                text: qsTr("Wifi Networks")
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

                ViewPlaceholder {
                    enabled: listView.count == 0
                    text: qsTr("No wifi networks defined")
                    hintText: qsTr("Pull down to add networks")
                }

                VerticalScrollDecorator {}
            }
        }
    }
}
