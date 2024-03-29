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

                    CircleImage {
                        id: avatar
                        width: parent.height - 2 * Theme.paddingSmall
                        source: App.api().baseUrl() + attributes.entity_picture
                        anchors.verticalCenter: parent.verticalCenter

                        fallbackText: listView.getInitials(name)
                        fallbackItemVisible: attributes.entity_picture ? false : true
                    }


                    Column {
                        width: parent.width - avatar.width - Theme.paddingMedium
                        anchors.verticalCenter: avatar.verticalCenter

                        Label {
                            width: parent.width
                            text: name
                            color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                        Label {
                            text: {
                                //% "At home"
                                if (entityState === "home") return qsTrId("id-at-home")
                                //% "Not at home"
                                if (entityState === "not_home") return qsTrId("id-not-at-home")
                                //% "Unkown"
                                if (entityState === "unknown") return qsTrId("id-unkown")

                                return entityState
                            }

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
            }

            ViewPlaceholder {
                enabled: listView.count == 0
                //% "No persons available"
                text: qsTrId("id-no-persons-available")
                //% "Create new persons in Home Assistant web interface"
                hintText: qsTrId("id-no-persons-available-desc")
            }

            VerticalScrollDecorator {}

            function getInitials(name) {
                var initials = name.match(/\b\w/g) || [];
                initials = ((initials.shift() || '') + (initials.pop() || '')).toUpperCase();
                return initials
            }  
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

//    Component.onCompleted: {
//        if ((Client.updateModes & Client.UpdateModeEntityModel) === Client.UpdateModeEntityModel) Client.entitiesProvider().updateModel(type)
//    }
}
