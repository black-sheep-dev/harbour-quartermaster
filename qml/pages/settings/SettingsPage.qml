import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: ListModel {
            ListElement {
                title: qsTr("Connection");
                description: qsTr("Manage connections settings")
                icon: "image://theme/icon-m-mobile-network"
                page: "SettingsConnectionPage.qml"
            }
            ListElement {
                title: qsTr("Device Tracking");
                description: qsTr("Manage tracking options")
                icon: "image://theme/icon-m-location"
                page: "SettingsDeviceTrackingPage.qml"
            }
            ListElement {
                title: qsTr("Device Sensors");
                description: qsTr("Manage device sensors")
                icon: "image://theme/icon-m-diagnostic"
                page: "SettingsDeviceSensorsPage.qml"
            }
            ListElement {
                title: qsTr("Homezone");
                description: qsTr("Manage homezone settings")
                icon: "image://theme/icon-m-home"
                page: "SettingsHomezonePage.qml"
            }
            ListElement {
                title: qsTr("Zones");
                description: qsTr("Manage zones")
                icon: "image://theme/icon-m-dot"
                page: "SettingsZonesPage.qml"
            }
            ListElement {
                title: qsTr("Notifications");
                description: qsTr("Manage system notifications")
                icon: "image://theme/icon-m-notifications"
                page: "SettingsNotificationsPage.qml"
            }
            ListElement {
                title: qsTr("Auto Updates");
                description: qsTr("Manage automatic entity updates")
                icon: "image://theme/icon-m-sync"
                page: "SettingsAutoUpdatePage.qml"
            }
            ListElement {
                title: qsTr("Backup & Restore");
                description: qsTr("Manage backup of settings")
                icon: "image://theme/icon-m-backup"
                page: "SettingsBackupPage.qml"
            }
            ListElement {
                title: qsTr("Device Info");
                description: qsTr("Manage device informations")
                icon: "image://theme/icon-m-about"
                page: "SettingsDeviceInfoPage.qml"
            }
            ListElement {
                title: qsTr("Home Assistant");
                description: qsTr("Manage Home Assistant")
                icon: "image://theme/icon-m-about"
                page: "SettingsHomeAssistantPage.qml"
            }
            ListElement {
                title: qsTr("Developer Mode");
                description: qsTr("Manage developer options")
                icon: "image://theme/icon-m-developer-mode"
                page: "SettingsDeveloperModePage.qml"
            }
            ListElement {
                title: qsTr("About");
                description: qsTr("Infos about Quartermaster")
                icon: "image://theme/icon-m-developer-mode"
                page: "../AboutPage.qml"
            }
        }

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Settings")
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

