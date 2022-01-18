import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy: App.api().runningRequests & Api.RequestWebhookGetZones

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
                //% "Refresh"
                text: qsTrId("id-refresh")
                onClicked: App.api().sendWebhookRequest(Api.RequestWebhookGetZones)
            }
            MenuItem {
                //% "Search"
                text: qsTrId("id-search")
                onClicked: listView.showSearch = true
            }
        }

        anchors.fill: parent

        Column {
            id: header
            width: parent.width

            PageHeader {
                //% "Zones"
                title: qsTrId("id-zones")
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
                sourceModel: App.locationService().zonesModel()
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
                                if (model.networkCount > 0)
                                    //% "%n network(s) defined"
                                    return qsTrId("id-networks-defined", model.networkCount)
                                else
                                    //% "No networks defined"
                                    return qsTrId("id-no-networks-defined");
                            }

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsZonePage.qml"), { zone: App.locationService().zonesModel().zoneAt(index)})
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                //% "No zones available"
                text: qsTrId("id-no-zones-available")
                //% "Create new zones in Home Assistant web interface"
                hintText: qsTrId("id-no-zones-available-desc")
            }

            VerticalScrollDecorator {}
        }
    }

    Component.onCompleted: App.api().sendWebhookRequest(Api.RequestWebhookGetZones)
}
