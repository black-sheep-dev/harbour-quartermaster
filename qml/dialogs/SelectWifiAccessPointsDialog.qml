import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    property Zone zone
    property string identifier
    property string name

    id: dialog

    anchors.fill: parent

    canAccept: identifier.length > 0 && name.length > 0

    DialogHeader {
        id: header
        //% "Select Access Point"
        title: qsTrId("id-select-access-point")
    }

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: App.locationService().availableAccessPointsModel().busy
        anchors.centerIn: parent
    }

    SilicaListView {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        width: parent.width

        model: App.locationService().availableAccessPointsModel()

        delegate: ListItem {
            height: Theme.itemSizeMedium
            width: parent.width

            contentHeight: Theme.itemSizeMedium

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height

                anchors.verticalCenter: parent.verticalCenter

                spacing: Theme.paddingMedium

                Image {
                    id: wlanIcon
                    source: "image://theme/icon-m-wlan"

                    anchors.verticalCenter: parent.verticalCenter
                }

                Column {
                    width: parent.width - wlanIcon.width - selectedIcon.width - 2 * Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width
                        //% "Hidden network"
                        text: name.length > 0 ? name : qsTrId("id-hidden-network")
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeMedium
                    }

                    Label {
                        width: parent.width
                        //% "Available network"
                        text: qsTrId("id-available-network")

                        color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }

                Image {
                    id: selectedIcon

                    visible: dialog.identifier === model.identifier

                    source: "image://theme/icon-m-acknowledge"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            onClicked: {
                dialog.identifier = model.identifier
                dialog.name = model.name
            }
        }

        VerticalScrollDecorator {}
    }

    //Component.onCompleted: App.locationService().scanForAccessPoints()
}
