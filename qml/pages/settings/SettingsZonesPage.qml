import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: Client.zonesModel().loading
        anchors.centerIn: page
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: Client.getZones()
            }
        }

        id: listView

        header: PageHeader {
            title: qsTr("Zones")
        }

        anchors.fill: parent

        opacity: Client.zonesModel().loading ? 0.0 : 1.0
        Behavior on opacity { FadeAnimator {} }

        model: Client.zonesModel()


        delegate: ListItem {
            id: delegate

            width: parent.width
            height: Theme.itemSizeLarge
            contentHeight: Theme.itemSizeLarge

            Row {
                width: parent.width - 2 * x
                x: Theme.horizontalPageMargin
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: zoneIcon

                    height: parent.height - 2 * Theme.paddingSmall
                    width: parent.height - 2 * Theme.paddingSmall
                    anchors.verticalCenter: parent.verticalCenter

                    source: "image://theme/icon-m-dot"
                }

                Item {
                    id: spacer

                    width:Theme.paddingMedium
                    height:1

                }

                Column {
                    id: data

                    width: parent.width - zoneIcon.width

                    anchors.verticalCenter: zoneIcon.verticalCenter

                    Label{
                        id: text
                        width: parent.width
                        text: name
                        color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label{
                        text: {
                            if (networks_count > 0)
                                return qsTr("%n network(s) defined", "", networks_count)
                            else
                                return qsTr("No networks defined");
                        }

                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }
            onClicked: pageStack.push(Qt.resolvedUrl("SettingsZonePage.qml"), { zone: Client.zonesModel().zoneAt(index)})
        }
        VerticalScrollDecorator {}
    }
}
