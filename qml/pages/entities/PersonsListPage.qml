import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    property string title
    property int type

    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: Client.entitiesProvider().loading
    }

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: Client.entitiesProvider().updateModel(type)
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
                sourceModel: Client.entitiesProvider().model(type)
            }

            delegate: ListItem {
                id: delegate
                width: parent.width
                contentHeight: Theme.itemSizeExtraLarge

                Row {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * x
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter

                    CircleImage {
                        id: avatar
                        width: parent.height - 2 * Theme.paddingSmall
                        source: Client.baseUrl() + attributes.entity_picture
                        anchors.verticalCenter: parent.verticalCenter

                        fallbackText: listView.getInitials(name)
                        fallbackItemVisible: attributes.entity_picture ? false : true
                    }

                    Item {
                        width: Theme.paddingMedium
                        height: 1
                    }

                    Column {
                        width: parent.width - avatar.width - Theme.paddingMedium
                        anchors.verticalCenter: avatar.verticalCenter

                        Label {
                            width: parent.width
                            text: name
                            color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                            font.pixelSize: Theme.fontSizeLarge
                        }
                        Label {
                            text: {
                                if (entity_state === "home") return qsTr("At home")
                                if (entity_state === "not_home") return qsTr("Not at home")
                                if (entity_state === "unknown") return qsTr("Unkown")

                                return entity_state
                            }

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                    }
                }
            }

            function getInitials(name) {
                var initials = name.match(/\b\w/g) || [];
                initials = ((initials.shift() || '') + (initials.pop() || '')).toUpperCase();
                return initials
            }
        }
    }

    Component.onCompleted: {
        if (Client.updateEntityModel) Client.entitiesProvider().updateModel(type)
    }
}
