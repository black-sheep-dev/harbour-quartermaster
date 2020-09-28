import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: Client.device().zonesModel().loading
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: Client.device().updateZones()
            }
        }

        opacity: Client.device().zonesModel().loading ? 0.1 : 1.0

        id: listView

        header: PageHeader {
            title: qsTr("Zones")
        }

        anchors.fill: parent

        model: Client.device().zonesModel()


        delegate: ListItem {
            id: delegate

            width: parent.width
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
                        elide: Text.ElideRight
                        text: name
                        color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label{
                        text: entity_id
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }
        }
        VerticalScrollDecorator {}
    }
}
