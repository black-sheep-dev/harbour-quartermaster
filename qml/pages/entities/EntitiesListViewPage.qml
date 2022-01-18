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
                //% "Refresh"
                text: qsTrId("id-refresh")
                onClicked: {
                    busy = true
                    App.entitiesService().refresh()
                }
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
                    width: parent.width - 2*x
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: Theme.paddingMedium

                    IconButton {
                        id: itemIcon
                        anchors.verticalCenter: parent.verticalCenter
                        icon.source: page.icon + (model.entityState === "on"  ? "?#64dd17" : "")
                        onClicked: {
                            if (model.entityState !== "on" && model.entityState !== "off") return

                            App.entitiesService().callService("homeassistant",
                                model.entityState === "on" ? "turn_off" : "turn_on",
                                model.entityId)
                            filterModel.entityAt(index).state = (model.entityState === "on" ? "off" : "on")
                        }
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
                        Label {
                            text: entityState

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }

                onClicked: {
                    switch (type) {
                    case Entity.Automation:
                        pageStack.push(Qt.resolvedUrl("types/AutomationPage.qml"), { entity: filterModel.entityAt(index) })
                        break;

                    case Entity.Camera:
                        pageStack.push(Qt.resolvedUrl("types/CameraPage.qml"), { entity: filterModel.entityAt(index) })
                        break;

                    case Entity.Climate:
                        pageStack.push(Qt.resolvedUrl("types/ClimatePage.qml"), { entity: filterModel.entityAt(index) })
                        break;

                    case Entity.Group:
                        pageStack.push(Qt.resolvedUrl("types/GroupPage.qml"), { entity: filterModel.entityAt(index) })
                        break;

                    case Entity.Light:
                        pageStack.push(Qt.resolvedUrl("types/LightPage.qml"), { entity: filterModel.entityAt(index) })
                        break;

                    case Entity.Switch:
                        pageStack.push(Qt.resolvedUrl("types/SwitchPage.qml"), { entity: filterModel.entityAt(index) })
                        break;

                    default:
                        pageStack.push(Qt.resolvedUrl("types/EntityPage.qml"), { entity: filterModel.entityAt(index) })
                        break;
                    }
                }
            }

            ViewPlaceholder {
                enabled: listView.count == 0

                //% "No entities available"
                text: qsTrId("id-no-entities-available")
                //% "Check your network connection"
                hintText: qsTrId("id-check-network")
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
        onRequestFinished: if (requestType === Api.RequestGetApiStates) busy = false
    }
}
