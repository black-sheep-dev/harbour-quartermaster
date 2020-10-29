import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property ErrorLogModel logModel

    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: logModel.loading
        anchors.centerIn: parent
    }

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: log = Client.refreshErrorLog()
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

            model: SortFilterModel {
                id: filterModel
                sourceModel: logModel
                sortRole: ErrorLogModel.HeaderRole

                Component.onCompleted: setSortOrder(Qt.DescendingOrder)
            }

            delegate: ListItem {
                id: delegate
                width: parent.width
                contentHeight: Theme.itemSizeHuge

                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height

                    Label {
                        width: parent.width

                        font.pixelSize: Theme.fontSizeExtraSmall
                        font.bold: true
                        wrapMode: Text.Wrap

                        text: header
                    }

                    Label {
                        width: parent.width

                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: Text.Wrap

                        text: preview
                    }
                }

                onClicked: pageStack.push(Qt.resolvedUrl("ErrorLogEntryPage.qml"), {
                                              header: header,
                                              message: message
                                          })
            }
        }
    }
}
