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
                text: qsTr("Refresh")
                onClicked: Client.entitiesProvider().refresh()
            }
        }

        id: listView

        visible: !busyIndicator.running

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Entities")
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
                        text: qsTr("%n entity available", "0", Client.entitiesProvider().model(type).entitiesCount())
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
    }
}
