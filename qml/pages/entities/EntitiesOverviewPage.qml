import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        PullDownMenu {
           MenuItem {
               text: qsTr("Refresh")
               onClicked: Client.entitiesProvider().refresh()
           }
       }


        id: listView

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Entities")
        }

        model: ListModel {
            ListElement {
                title: qsTr("Alarms");
                description: qsTr("List of all alarms")
                icon: "image://theme/icon-m-alarm"
                page: "EntitiesListView.qml"
            }
            ListElement {
                title: qsTr("Automations");
                description: qsTr("List of all automations")
                icon: "image://theme/icon-m-toy"
                page: "EntitiesListView.qml"
            }
            ListElement {
                title: qsTr("Cameras");
                description: qsTr("List of all cameras")
                icon: "image://theme/icon-m-video"
                page: "EntitiesListView.qml"
            }
            ListElement {
                title: qsTr("Climates");
                description: qsTr("List of all climates")
                icon: "image://theme/icon-m-ambience"
                page: "EntitiesListView.qml"
            }
            ListElement {
                title: qsTr("Lights");
                description: qsTr("List of all lights")
                icon: "image://theme/icon-m-day"
                page: "EntitiesListView.qml"
            }
            ListElement {
                title: qsTr("Persons");
                description: qsTr("List of all persons")
                icon: "image://theme/icon-m-users"
                page: "PersonsListPage.qml"
            }
            ListElement {
                title: qsTr("Sensors");
                description: qsTr("List of all sensors")
                icon: "image://theme/icon-m-wizard"
                page: "EntitiesListView.qml"
            }
            ListElement {
                title: qsTr("Switches");
                description: qsTr("List of all switches")
                icon: "image://theme/icon-m-charging"
                page: "EntitiesListView.qml"
            }
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

                Image {
                    id: itemIcon
                    source: icon
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width:Theme.paddingMedium
                    height:1
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
                        text: description
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }

            onClicked: if (page.length > 0 ) pageStack.push(Qt.resolvedUrl(page), {
                                                                title: title,
                                                                icon: icon,
                                                                type: index + 1
                                                            })
        }

        VerticalScrollDecorator {}
    }
}
