import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    property Zone zone
    property AccessPointsModel apsModel: App.locationService().accessPointsModel(zone.guid)

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            visible: App.locationService().enableWifi
            MenuItem {
                //% "Reset"
                text: qsTrId("id-reset")
                //% "Resetting access points"
                onClicked: resetPopup.execute(qsTrId("id-resetting-access-points"), function() {

                })
            }

            MenuItem {
                //% "Add access point"
                text: qsTrId("id-add-access-point")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../../dialogs/SelectWifiAccessPointsDialog.qml"), {zone: zone});

                    dialog.accepted.connect(function() {
                        if (!App.locationService().addAccessPointToZone(zone.guid,
                                                                        dialog.identifier,
                                                                        dialog.name)) {
                            return;
                        }

                        apsModel.addAccessPoint(dialog.identifer, dialog.name);
                        App.locationService().saveSettings();
                    })
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
                //% "Zone settings"
                title: qsTrId("id-zone-settings")
            }
            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                text: zone.name
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            DetailItem {
                //% "Latitude"
                label: qsTrId("id-latitude")
                value: zone.latitude
            }

            DetailItem {
                //% "Longitude"
                label: qsTrId("id-longitude")
                value: zone.longitude
            }

            DetailItem {
                //% "Radius"
                label: qsTrId("id-radius")
                value: zone.radius
            }

            SectionHeader {
                //% "Wifi access points"
                text: qsTrId("id-wifi-access-points")
            }

            SilicaListView {    
                id: listView

                width: parent.width
                height: 600

                model: apsModel

                delegate: ListItem {
                    id: delegate
                    width: parent.width
                    contentHeight: Theme.itemSizeMedium

                    ListView.onRemove: animateRemoval(delegate)

                    menu: ContextMenu {
                        MenuItem {
                            //% "Delete"
                            text: qsTrId("id-delete");
                            //% "Deleting access point"
                            onClicked: remorse.execute(delegate, qsTrId("id-deleting-access-point"), function() {
                                var ok = App.locationService().removeAccessPointFromZone(zone.guid, identifier)

                                if (!ok) return;

                                apsModel.removeAccessPoint(identifier)
                                App.locationService().saveAccessPointSettings()
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

                            //% "Hidden network"
                            text: name.length > 0 ? name : qsTrId("id-hidden-network")
                            color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                    }
                }

                ViewPlaceholder {
                    enabled: listView.count == 0
                    //% "No wifi access points defined"
                    text: qsTrId("id-no-wifi-access-points-defined")
                    //% "Pull down to add an access point"
                    hintText: qsTrId("id-no-wifi-access-points-defined-desc")
                }

                VerticalScrollDecorator {}
            }
        }
    }
}
