import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: ListModel {
            ListElement {
                title: qsTr("Connection");
                description: qsTr("Edit connections settings")
                icon: "image://theme/icon-m-mobile-network"
                page: "SettingsConnectionPage.qml"
            }
            ListElement {
                title: qsTr("Device Info");
                description: qsTr("Show and edit device informations")
                icon: "image://theme/icon-m-device"
                page: "SettingsDeviceInfoPage.qml"
            }
        }

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Settings")
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

            onClicked: pageStack.push(Qt.resolvedUrl(page))
        }

        VerticalScrollDecorator {}
    }
}

