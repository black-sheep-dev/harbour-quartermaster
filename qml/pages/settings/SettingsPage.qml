import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: ListModel {
            ListElement {
                //% "Connections"
                title: qsTrId("id-connections");
                //% "Manage connections settings"
                description: qsTrId("id-connections-list-desc")
                icon: "image://theme/icon-m-mobile-network"
                page: "SettingsConnectionPage.qml"
            }
            ListElement {
                //% "Device tracking"
                title: qsTrId("id-device-tracking");
                //% "Manage tracking options"
                description: qsTrId("id-device-tracking-list-desc")
                icon: "image://theme/icon-m-location"
                page: "SettingsDeviceTrackingPage.qml"
            }
            ListElement {
                //% "Device sensors"
                title: qsTrId("id-device-sensors");
                //% "Manage device sensors"
                description: qsTrId("id-device-sensors-list-desc")
                icon: "image://theme/icon-m-diagnostic"
                page: "SettingsDeviceSensorsPage.qml"
            }
            ListElement {
                //% "Zones"
                title: qsTrId("id-zones");
                //% "Manage zones and access points"
                description: qsTrId("id-zones-list-desc")
                icon: "image://theme/icon-m-dot"
                page: "SettingsZonesPage.qml"
            }
            ListElement {
                //% "Notifications"
                title: qsTrId("id-notifications");
                //% "Manage system notifications"
                description: qsTrId("id-notifications-list-desc")
                icon: "image://theme/icon-m-notifications"
                page: "SettingsNotificationsPage.qml"
            }
            ListElement {
                //% "Live updates"
                title: qsTrId("id-live-updates");
                //% "Manage live updates"
                description: qsTrId("id-live-updates-list-desc")
                icon: "image://theme/icon-m-sync"
                page: "SettingsLiveUpdatePage.qml"
            }
            ListElement {
                //% "Backup & Restore"
                title: qsTrId("id-backup-restore");
                //% "Manage backup of settings"
                description: qsTrId("id-backup-restore-list-desc")
                icon: "image://theme/icon-m-backup"
                page: "SettingsBackupPage.qml"
            }
            ListElement {
                //% "Device info"
                title: qsTrId("id-device-info");
                //% "Manage device informations"
                description: qsTrId("id-device-info-list-desc")
                icon: "image://theme/icon-m-about"
                page: "SettingsDeviceInfoPage.qml"
            }
            ListElement {
                //% "Home Assistant"
                title: qsTrId("id-home-assistant");
                //% "Manage Home Assistant"
                description: qsTrId("id-home-assistant-list-desc")
                icon: "image://theme/icon-m-about"
                page: "SettingsHomeAssistantPage.qml"
            }
            ListElement {
                //% "Developer mode"
                title: qsTrId("id-developer-mode");
                //% "Manage developer options"
                description: qsTrId("id-developer-mode-list-desc")
                icon: "image://theme/icon-m-developer-mode"
                page: "SettingsDeveloperModePage.qml"
            }
            ListElement {
                //% "About"
                title: qsTrId("id-about");
                //% "Infos about Quartermaster"
                description: qsTrId("id-about-list-desc")
                icon: "image://theme/icon-m-about"
                page: "../AboutPage.qml"
            }
        }

        anchors.fill: parent
        header: PageHeader {
            //% "Settings"
            title: qsTrId("id-settings")
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
                        color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                    Label {
                        text: description
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl(page))
        }

        VerticalScrollDecorator {}
    }
}

