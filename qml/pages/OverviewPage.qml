import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy

    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: busy && listView.count === 0
        anchors.centerIn: page
    }

    SilicaListView {
        PullDownMenu {
            busy:  page.busy
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("settings/SettingsPage.qml"))
            }
//            MenuItem {
//                text: qsTr("Web View")
//                onClicked: pageStack.push(Qt.resolvedUrl("WebViewPage.qml"))
//            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    busy = true
                    App.entitiesService().refresh()
                }
            }
        }

        id: listView

        opacity: busy ? 0.2 : 1.0
        Behavior on opacity { FadeAnimator {} }

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Entities")
        }

        model: SortFilterModel {
            id: sortModel
            sourceModel: App.entitiesService().entityTypesModel()
            sortRole: EntityTypesModel.TitleRole
            dynamicSortFilter: true
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
                spacing: Theme.paddingMedium

                Image {
                    id: itemIcon
                    source: icon
                    anchors.verticalCenter: parent.verticalCenter
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
                        font.pixelSize: Theme.fontSizeMedium
                    }
                    Label {
                        text: qsTr("%n entity available", "0", model.count)
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
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

        ViewPlaceholder {
            enabled: !busy && listView.count === 0
            text: qsTr("No entities available")
            hintText: qsTr("Check your network connection")
        }

        VerticalScrollDecorator {}
    }

    function startSetupWizard() {
        pageStack.clear()
        pageStack.push(Qt.resolvedUrl("wizard/WizardIntroPage.qml"))
    }

    onStatusChanged: {
        if (status !== PageStatus.Active) return

        if (App.needSetup) startSetupWizard()
    }

    Connections {
        target: App.api()
        onRequestFinished: if (requestType === Api.RequestGetApiStates) busy = false
    }

    Connections {
        target: App.entitiesService()
        onStateChanged: busy = (state === EntitiesService.StateInitializing)
    }

    Component.onCompleted: sortModel.sortModel()

}
