import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    property bool busy: false
    property string title
    property int type
    property string icon

    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: busy
        anchors.centerIn: parent
    }

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    busy = true
                    App.entitiesService().refresh()
                }
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
                title: page.title
            }

            SearchField {
                id: searchField
                width: parent.width
                height: listView.showSearch ? implicitHeight : 0
                opacity: listView.showSearch ? 1 : 0
                onTextChanged: {
                    filterModel.setFilterFixedString(text)
                }

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

            visible: !busyIndicator.running

            width: parent.width
            anchors.top: header.bottom
            anchors.bottom: parent.bottom

            clip: true

            model: EntitiesSortFilterModel {
                id: filterModel
                sourceModel: App.entitiesService().entitiesModel()
            }

            delegate: ListItem {
                id: delegate
                width: parent.width
                contentHeight: Theme.itemSizeMedium

                Row {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * x
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: Theme.paddingMedium

                    Image {
                        id: itemIcon
                        source: page.icon
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Column {
                        width: parent.width - itemIcon.width - Theme.paddingMedium
                        anchors.verticalCenter: itemIcon.verticalCenter

                        Label {
                            width: parent.width
                            text: name
                            color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                        TextSwitch {
                            text: {
                                if (entityState === "on") {
                                    return qsTr("on", "state")
                                } else if (entityState === "off") {
                                    return qsTr("off", "state")
                                } else if (entityState === "unavailable") {
                                    return qsTr("unavailable", "state")
                                }
                                return qsTr("undefined", "state")
                            }
                            checked: entityState === "on"
                            enabled: entityState === "on" || entityState === "off"

                            onClicked: {
                                page.busy = true;
                                App.entitiesService().callService("light",
                                                                  checked ? "turn_on" : "turn_off",
                                                                  entityId)
                            }
                        }
                    }
                }

                onClicked: pageStack.push(Qt.resolvedUrl("types/LightPage.qml"),
                                          { entity: filterModel.entityAt(index) })
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                text: qsTr("No entities available")
                hintText: qsTr("Check your network connection")
            }

            VerticalScrollDecorator {}
        }
    }

    Component.onCompleted: {
        filterModel.resetEntityFilter()
        filterModel.addEntityFilter(type)
    }

    Component.onDestruction: filterModel.resetEntityFilter()

    Connections {
        target: App.api()
        onRequestFinished: {
            if (requestType === Api.RequestGetApiStates) {
                busy = false
            } else if (requestType === Api.RequestPostApiServices) {
                App.entitiesService().refresh()
            }
        }
    }
}
