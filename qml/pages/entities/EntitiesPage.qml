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
               onClicked: Client.getStates()
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
                page: ""
            }
            ListElement {
                title: qsTr("Automations");
                description: qsTr("List of all automations")
                icon: "image://theme/icon-m-toy"
                page: ""
            }
            ListElement {
                title: qsTr("Climates");
                description: qsTr("List of all climates")
                icon: "image://theme/icon-m-ambience"
                page: ""
            }
            ListElement {
                title: qsTr("Lights");
                description: qsTr("List of all lights")
                icon: "image://theme/icon-m-day"
                page: ""
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
                page: ""
            }
            ListElement {
                title: qsTr("Switches");
                description: qsTr("List of all switches")
                icon: "image://theme/icon-m-charging"
                page: ""
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

            onClicked: if (page.length > 0 ) pageStack.push(Qt.resolvedUrl(page))
        }

        VerticalScrollDecorator {}
    }
}
