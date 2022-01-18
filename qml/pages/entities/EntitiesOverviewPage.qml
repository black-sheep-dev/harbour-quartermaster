import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property Entity entity

    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: Client.entitiesProvider().loading
        anchors.centerIn: page
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                //% "Refresh"
                text: qsTrId("id-refresh")
                onClicked: App.entitiesService().refresh()
            }
        }

        id: listView

        visible: !busyIndicator.running

        anchors.fill: parent
        header: PageHeader {
            //% "Entities"
            title: qsTrId("id-entities")
        }

        model: SortFilterModel {
            id: sortModel
            sourceModel: Client.entitiesProvider().typesModel()
        }

        delegate: ListItem {
            id: delegate
            width: parent.width
            contentHeight: Theme.itemSizeLarge

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon
                    source: icon
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width: Theme.paddingMedium
                    height: 1
                }

                Column {
                    id: data
                    width: parent.width - itemIcon.width
                    anchors.verticalCenter: itemIcon.verticalCenter
                    Label {
                        id: text
                        width: parent.width
                        text: title
                        color: pressed?Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label {
                        //% "%n entity available"
                        text: qsTrId("id-entities-available", Client.entitiesProvider().model(type).entitiesCount())
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }

            onClicked: {

                var page;

                switch (type) {
                case Entity.Person:
                    page = "PersonsListPage.qml"
                    break;

                case Entity.Sensor:
                    page = "SensorsListPage.qml"
                    break;

                default:
                    page = "EntitiesListViewPage.qml"
                    break;
                }

                pageStack.push(Qt.resolvedUrl(page), {
                                   title: title,
                                   icon: icon,
                                   type: type })
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
