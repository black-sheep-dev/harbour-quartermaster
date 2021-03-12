import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy: App.api().runningRequests & ApiConnector.RequestWebhookGetZones

    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: busy
        anchors.centerIn: page
    }

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: App.api().sendWebhookRequest(ApiConnector.RequestWebhookGetZones)
            }
            MenuItem {
                text: qsTr("Search")
                onClicked: listView.showSearch = true
            }
        }

        anchors.fill: parent

        Column {
            id: header
            width: parent.width

            PageHeader {
                title: qsTr("Zones")
            }

            SearchField {
                id: searchField
                width: parent.width
                height: listView.showSearch ? implicitHeight : 0
                opacity: listView.showSearch ? 1 : 0
                onTextChanged: {
                    filterModel.setFilterFixedString(text)
                }

                EnterKey.onClicked: focus = false

                Connections {
                    target: listView
                    onShowSearchChanged: {
                        searchField.forceActiveFocus()
                    }
                }

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }
                Behavior on opacity {
                    NumberAnimation { duration: 300 }
                }
            }
        }


        SilicaListView {
            property bool showSearch: false

            id: listView

            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            width: parent.width

            clip: true

            opacity: busy ? 0.2 : 1.0
            Behavior on opacity { FadeAnimator {} }

            model: SortFilterModel {
                id: filterModel
                sourceModel: App.locationTracker().zonesModel()
                sortRole: ZonesModel.NameRole
                filterRole: ZonesModel.NameRole
                filterCaseSensitivity: Qt.CaseInsensitive
            }

            delegate: ListItem {
                id: delegate

                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium

                Row {
                    width: parent.width - 2 * x
                    x: Theme.horizontalPageMargin
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: Theme.paddingMedium

                    Image {
                        id: zoneIcon
                        anchors.verticalCenter: parent.verticalCenter

                        source: model.isHome ? "image://theme/icon-m-home" : "image://theme/icon-m-dot"
                    }

                    Column {
                        width: parent.width - zoneIcon.width - parent.spacing

                        anchors.verticalCenter: zoneIcon.verticalCenter

                        Label{
                            width: parent.width
                            text: model.name
                            color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                        Label{
                            text: {
                                if (model.networksCount > 0)
                                    return qsTr("%n network(s) defined", "", model.networksCount)
                                else
                                    return qsTr("No networks defined");
                            }

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsZonePage.qml"), { zone: App.locationTracker().zonesModel().zoneAt(index)})
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                text: qsTr("No Zones available")
                hintText: qsTr("Create new zones in Home Assistant web interface. If zones are defined but are not displayed here, then there is a problem with the connection or the access token. In this case it helps to reset and renew the device registration.")
            }

            VerticalScrollDecorator {}
        }
    }
}
