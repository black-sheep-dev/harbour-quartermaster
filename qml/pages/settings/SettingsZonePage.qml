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
                text: qsTr("Reset")
                onClicked: resetPopup.execute(qsTr("Resetting access points"), function() {

                })
            }

            MenuItem {
                text: qsTr("Add Access Point")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../../dialogs/SelectWifiAccessPointsDialog.qml"), {zone: zone});

                    dialog.accepted.connect(function() {
                        if (!App.locationService().addAccessPointToZone(zone.guid,
                                                                        dialog.identifier,
                                                                        dialog.name)) {
                            return;
                        }

                        apsModel.addAccessPoint(dialog.identifer, dialog.name);
                        App.locationService().saveAccessPointSettings();
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
                text: qsTr("Wifi Access Points")
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
                            text: qsTr("Delete");
                            onClicked: remorse.execute(delegate, qsTr("Deleting access point"), function() {
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

                            text: name.length > 0 ? name : qsTr("Hidden network")
                            color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                    }
                }

                ViewPlaceholder {
                    enabled: listView.count == 0
                    text: qsTr("No wifi access points defined")
                    hintText: qsTr("Pull down to add an access point")
                }

                VerticalScrollDecorator {}
            }
        }
    }
}
