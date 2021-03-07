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
            busy: Client.entitiesProvider().loading
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("settings/SettingsPage.qml"))
            }
            MenuItem {
                text: qsTr("Web View")
                onClicked: pageStack.push(Qt.resolvedUrl("WebViewPage.qml"))
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: Client.entitiesProvider().refresh()
            }

            opacity: 1.0
        }

        id: listView

        opacity: busyIndicator.running ? 0.1 : 1.0

        Behavior on opacity {
            FadeAnimation {}
        }

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
                    page = "entities/PersonsListPage.qml"
                    break;

                case Entity.Sensor:
                    page = "entities/SensorsListPage.qml"
                    break;

                default:
                    page = "entities/EntitiesListViewPage.qml"
                    break;
                }

                pageStack.push(Qt.resolvedUrl(page), {
                                   title: title,
                                   icon: icon,
                                   type: type })
            }
        }
    }

    Connections {
        target: Client
        onReadyChanged: {
            if (!Client.ready) return

            if (!Client.isRegistered()) {
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("wizard/WizardIntroPage.qml"))
            }
        }
    }
}
